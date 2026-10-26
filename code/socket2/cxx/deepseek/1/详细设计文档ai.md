# unpsock 详细设计文档

## 1. 概述

### 1.1 项目定位

`unpsock` 是一个面向 Linux（POSIX 接口）的现代 C++ Socket 库。与 Asio、POCO 等跨平台异步库不同，`unpsock` 刻意放弃跨平台抽象与复杂异步模型，聚焦于 **SoC / 嵌入式 Linux 平台** 的易用性与性能：

| 维度 | unpsock 的选择 | 与 Asio/POCO 的区别 |
|------|----------------|---------------------|
| 平台 | 仅 POSIX / Linux | 不做 Windows / IOCP 抽象 |
| 并发模型 | 同步阻塞 + 可选 non-blocking + Selector | 无 Proactor/Reactor 框架、无协程、无回调链 |
| 类层次 | 扁平（Address / Socket / Selector 三实体） | 无模板策略、无 policy-based 设计 |
| 内存管理 | `std::string` / `std::vector` 自动 RAII | 调用方无需管理 buffer 生命周期 |
| 错误处理 | **双风格**：`std::error_code` 与异常 | 与 Asio 类似的 `error_code` 版 + 自定义异常版并存 |
| 接口风格 | 参考 Python `socket` 模块 | 直白、命令式，学习成本低 |

### 1.2 设计原则

1. **零隐藏成本**：所有封装在非热点路径上；保留 `fileno()`、`sockaddr_ptr()` 等原始接口，允许调用方绕过封装直接使用 POSIX 接口。
2. **双错误风格**：每个可能失败的接口同时提供 **异常版**（无 `ec` 参数，失败抛 `SocketError` / `AddressError` / `SelectorError`）和 **错误码版**（带 `std::error_code&` 出参，`noexcept`）。原型验证用前者，生产复杂逻辑用后者。
3. **POD 化的 kernel 参数**：`getsockopt` / `setsockopt` 采用模板 + `socklen_t` 长度校验，避免 `int` / `timeval` / `linger` 等类型错配。
4. **不做过度抽象**：`Selector` 只有四个虚函数（`add_impl` / `modify_impl` / `remove_impl` / `wait_impl`），不做 "handler 注册"、不做 "事件循环"。
5. **RAII 完备**：`Socket` 移动语义、析构自动 `close()`；`Address` 值语义；`Selector` 析构关闭 epoll fd。

### 1.3 模块组成

```
unpsock/
├── socket_exception.hpp   异常与 error_category（GaiCategory / SelectorErrorCategory）
├── socket_host.hpp        主机名工具
├── socket_address.hpp     Address：sockaddr_storage 统一封装 + DNS 解析
├── socket_socket.hpp      Socket：C socket 的现代封装
├── socket_selector.hpp    Selector 抽象基类 + Event + ReadyEvent
├── socket_select.hpp      SelectSelector
├── socket_poll.hpp        PollSelector
├── socket_epoll.hpp       EpollSelector
└── socket.hpp             聚合头
```

依赖方向（无环）：

```
socket.hpp
  ├─ socket_exception.hpp
  ├─ socket_host.hpp
  ├─ socket_address.hpp ── socket_exception.hpp
  ├─ socket_socket.hpp  ── socket_address.hpp, socket_exception.hpp
  ├─ socket_selector.hpp ── socket_socket.hpp
  ├─ socket_select.hpp  ── socket_selector.hpp
  ├─ socket_poll.hpp    ── socket_selector.hpp
  └─ socket_epoll.hpp   ── socket_selector.hpp
```

---

## 2. 错误处理设计

### 2.1 异常层次

```
std::system_error
  └── unpsock::SocketException          // 通用基类；持 std::error_code + what
        ├── unpsock::AddressError       // 地址解析 / DNS（默认 gai_category）
        ├── unpsock::SocketError        // 底层 socket syscall 失败
        └── unpsock::SelectorError      // Selector 注册 / wait 失败
```

关键点：

- `SocketException` 继承 `std::system_error`，构造函数同时接受 `int errnum` 和 `std::error_code`。
- `AddressError` 默认用 `gai_category()`，因为 `getaddrinfo` 的返回值属于 `EAI_*` 命名空间，不能映射到 `errno`。
- `SelectorError` 同时覆盖两种来源：`system_category`（`epoll_create1` 失败）与 `selector_category`（`AlreadyRegistered` 等）。

### 2.2 自定义 error_category

**GaiCategory**（`socket_exception.hpp`）：

```cpp
class GaiCategory : public std::error_category {
    const char* name() const noexcept override { return "getaddrinfo"; }
    std::string message(int ev) const override {
        const char* s = ::gai_strerror(ev);
        return s ? std::string(s) : std::string("unknown getaddrinfo error");
    }
};
inline std::error_code make_gai_error_code(int ev) noexcept;
```

**SelectorErrorCategory**（`socket_selector.hpp`）：

```cpp
enum class SelectorErrc {
    Success = 0,
    AlreadyRegistered,   // add 时 fd 已注册
    NotRegistered,       // modify/remove 时 fd 未注册
    BackendUnavailable,  // make_selector 找不到可用后端
    WaitFailed,          // wait 底层失败（errno 用 system_category）
};
```

### 2.3 双风格约定（契约）

所有公有 API 遵循以下模式：

```cpp
// 异常版：无 error_code 出参，失败 throw
void op(Args...);
// 错误码版：error_code& 出参，noexcept，失败置 ec
void op(Args..., std::error_code& ec) noexcept;
```

**实现模式**：异常版调用错误码版，再 `if (ec) throw`。这保证两条路径行为一致。

**错误码版契约**：

- 进入函数时先 `ec.clear()`。
- 成功时 `ec` 保持为空；**注意 `recv` / `accept` 对端关闭时返回 0 / 无效 Socket 但 `ec` 为空**，调用方需按返回值和 `ec` 双重判定。
- `noexcept` 保证——即使内部 `std::bad_alloc` 也会 `terminate`（可接受，属于编程错误）。

### 2.4 关键错误路径表

| 场景 | 返回 | ec |
|------|------|-----|
| `recv` 对端正常关闭 | `0` 字节 / 空 vector | 空 |
| `recv` 出错 | `0` / 空 | `system_category(errno)` |
| `accept` 出错 | `{Socket(), Address()}` | `system_category(errno)` |
| `resolve_all` 失败 | 空 vector | `gai_category(EAI_*)` |
| `Selector::add` fd 已注册 | — | `SelectorErrc::AlreadyRegistered` |
| `connect_timed` 超时 | — | `errc::timed_out` |
| `getsockopt<T>` 长度不符 | `T{}` | `errc::invalid_argument` |
| fd 已关闭（`fd_ < 0`） | — | `errc::bad_file_descriptor` |

---

## 3. Address 模块

### 3.1 数据布局

```cpp
class Address {
    struct sockaddr_storage storage_{};   // 128 字节，覆盖所有 sockaddr_*
    socklen_t               len_;         // 实际长度
    Family                  family_;      // 冗余家族标记，快速判断
};
```

统一存储的动机：Linux 上 `sockaddr_storage` 足以容纳 `sockaddr_in`（16B）、`sockaddr_in6`（28B）、`sockaddr_un`（110B）。使用 `sockaddr_storage` 作为唯一载体，避免 `union` + tag 的手工管理。

**`family_` 与 `storage_.ss_family` 冗余**：`family_` 用 `Family` 枚举，避免每次 `switch` 前先读 `ss_family`，也方便与 `Socket::family()` 一致。

### 3.2 支持地址类型

| 家族 | 类型 | 构造入口 | 长度 |
|------|------|----------|------|
| `AF_INET` | TCP / UDP | `from_ip`, `resolve`, `any`, `loopback` | `sizeof(sockaddr_in)` |
| `AF_INET6` | TCP / UDP | 同上 | `sizeof(sockaddr_in6)` |
| `AF_UNIX` | 流 / 数据报 | `from_unix` | `SUN_LEN()`（含 `\0` 截断） |
| `AF_UNSPEC` | 空 | 默认构造 | 0 |

Unix domain 路径长度校验在 `from_unix_impl`：

```cpp
constexpr size_t kSunPathCap = sizeof(((sockaddr_un*)nullptr)->sun_path); // 108
if (path.size() >= kSunPathCap) { ec = errc::filename_too_long; return false; }
```

### 3.3 构造 API 矩阵

| 静态函数 | 异常版 | 错误码版 | 说明 |
|----------|--------|----------|------|
| `from_ip(ip, port, family=UNSPEC)` | ✓ | ✓ | `inet_pton` 双栈自适应：先试 IPv4，失败再试 IPv6（`family==UNSPEC`） |
| `from_unix(path)` | ✓ | ✓ | 路径长度 ≤ 107 字节 |
| `from_sockaddr(sa, len)` | ✓ | ✓ | 由 syscall 返回的 sockaddr 直接封装 |
| `resolve_all(host, port, family, type)` | ✓ | ✓ | `getaddrinfo`，返回全部候选 |
| `resolve(host, port, family, type)` | ✓ | ✓ | 取 `resolve_all().front()`，空集 → `EAI_NONAME` |
| `any(port, family)` | ✓ | ✓ | `INADDR_ANY` / `in6addr_any` |
| `loopback(port, family)` | ✓ | ✓ | `INADDR_LOOPBACK` / `in6addr_loopback` |

**`family == UNSPEC` 的双栈策略**：`from_ip_impl` 先按 IPv4 `inet_pton`，成功即返回；否则按 IPv6 重试。这样 `Address::from_ip("::1", 8080)` 与 `Address::from_ip("127.0.0.1", 8080)` 都可用，而不用显式指定 `family`。

**DNS 解析实现**（`resolve_all` 错误码版）：

```cpp
struct addrinfo hints{};
hints.ai_family   = detail::to_af(family);
hints.ai_socktype = detail::to_socktype(type);
hints.ai_flags    = AI_CANONNAME;
int rc = ::getaddrinfo(node, port_str.c_str(), &hints, &result);
if (rc != 0) { ec = make_gai_error_code(rc); return out; }

struct Guard { addrinfo* p; ~Guard() { if (p) ::freeaddrinfo(p); } } guard{result};
for (auto* p = result; p; p = p->ai_next) {
    Address a;
    if (a.from_sockaddr_impl(p->ai_addr, p->ai_addrlen, ec)) out.push_back(a);
}
ec.clear();   // 单个候选失败不污染整体结果
return out;
```

注意 `ec.clear()` 在循环后调用——若某个 `ai_next` 解析失败但其它成功，视为整体成功。

### 3.4 字符串化

| 方法 | IPv4 | IPv6 | Unix |
|------|------|------|------|
| `ip()` | `inet_ntop` | `inet_ntop` | `sun_path` |
| `to_string()` | `1.2.3.4:80` | `[::1]:80` | `/tmp/x.sock` |

IPv6 用 `[...]` 包裹，符合 RFC 3986 host 语法。

### 3.5 比较

`operator==` 按 `family_` 分派：

- IPv4：比较 `sin_port` 与 `sin_addr.s_addr`
- IPv6：比较 `sin6_port` 与整个 `in6_addr`（`memcmp`）
- Unix：`memcmp(sun_path, ..., 108)` 全量比较（容忍未初始化字节）
- `UNSPEC`：`return true`（两个空地址视为相等）

---

## 4. Socket 模块

### 4.1 类生命周期

```cpp
Socket() noexcept;                              // 空 socket（fd_ = -1）
Socket(Family, AddrType);                       // 异常版：创建即成功或抛
Socket(Family, AddrType, std::error_code&) noexcept; // 错误码版
Socket(int fd, Family, AddrType) noexcept;      // 接管 fd（供 accept 内部用）

Socket(const Socket&) = delete;                 // 禁止拷贝
Socket(Socket&&) noexcept;                      // 移动：源 fd 置 -1
Socket& operator=(Socket&&) noexcept;
~Socket();                                      // close() if fd_ >= 0
```

移动构造/赋值把源对象的 `fd_` 置 `-1`，防止双 close。移动赋值先 `close()` 自身。

### 4.2 服务端 API

| 方法 | 异常版 | 错误码版 | 备注 |
|------|--------|----------|------|
| `bind(Address)` | ✓ | ✓ | 直接调 `::bind` |
| `listen(backlog=5)` | ✓ | ✓ | backlog 默认 5，与 Python 一致 |
| `accept()` → `pair<Socket, Address>` | ✓ | ✓ | `EINTR` 重试；返回 peer 地址 |

`accept` 用 `sockaddr_storage` + `socklen_t`，由 `Address::from_sockaddr` 转换（异常版会在 `from_sockaddr` 内抛，但由于 `accept` 错误码版是 `noexcept`，实测走 `from_sockaddr(...)` 的异常重载会让 `noexcept` 版 terminate——**约束**：`from_sockaddr` 只会在内部 `ec` 设置时抛，而 `accept` 传入的合法 `sockaddr_storage` 不会触发，实践中安全）。

### 4.3 客户端 API

| 方法 | 说明 |
|------|------|
| `connect(Address)` | 阻塞 connect，`EINTR` 重试 |
| `connect(host, port)` | 内部 `Address::resolve(host, port, family_, type_, ec)` 再 connect |
| `connect_timed(addr, timeout)` | **非阻塞 connect + select 超时**，见 4.4 |
| `connect_timed(host, port, timeout)` | 同上，带 DNS 解析 |

**注意** `connect(host, port)` 错误码版在 DNS 失败时 `ec` 是 `gai_category`，而 socket 层失败是 `system_category`——调用方需按 `ec.category()` 区分。

### 4.4 connect_timed 实现细节

```
1. fcntl(F_GETFL) 保存旧标志
2. fcntl(F_SETFL, old | O_NONBLOCK)
3. FlagsGuard RAII：析构时恢复 O_NONBLOCK 状态
4. connect()
   - rc == 0      → 立即成功（localhost 常见），return
   - errno != EINPROGRESS → 直接错，return
5. select(fd+1, &rset, &wset, nullptr, &tv)  // 可写即可
6. n == 0       → errc::timed_out
7. getsockopt(SO_ERROR) 取真实结果
8. 成功返回，guard 析构恢复阻塞标志
```

`FlagsGuard` 是关键——即使后续步骤抛出异常（理论上不会，因为所有 API 都是 `noexcept` 版），析构也会恢复 `O_NONBLOCK`，保证 socket 最终回到阻塞状态。

### 4.5 数据收发

| 方法 | 语义 |
|------|------|
| `recv(size_t)` → `vector<uint8_t>` | 分配临时 buffer，`recv_impl` 后 resize |
| `recv(void*, size_t)` → `size_t` | 零拷贝；返回字节数 |
| `sendall(data)` | **循环 send 直到全部发出**，`MSG_NOSIGNAL` 抑制 SIGPIPE |
| `send(data)` | 单次 send，返回实际发送字节数 |
| `recvfrom(bufsize)` → `pair<vector, Address>` | UDP |
| `sendto(data, Address)` | UDP |

**`sendall` 循环**（错误码版）：

```cpp
size_t sent = 0;
while (sent < len) {
    ssize_t n = ::send(fd_, p + sent, len - sent, MSG_NOSIGNAL);
    if (n < 0) {
        if (errno == EINTR) continue;
        ec = std::error_code(errno, std::system_category());
        return sent;   // 部分发送，调用方需检查 ec 与返回值
    }
    sent += n;
}
```

**关键**：`sendall` 在部分发送后出错时返回已发送字节数（非 0），调用方应同时检查 `ec` 和返回值。这是 "sendall 失败时可能已发送部分数据" 的标准契约。

`recv_impl` 私有方法：

```cpp
ssize_t recv_impl(void* buf, size_t bufsize, std::error_code& ec) noexcept {
    ec.clear();
    while (true) {
        ssize_t n = ::recv(fd_, buf, bufsize, 0);
        if (n < 0 && errno == EINTR) continue;
        if (n < 0) { ec = std::error_code(errno, std::system_category()); return -1; }
        return n;  // n == 0 对端关闭
    }
}
```

### 4.6 Socket Option 便捷函数

每个 option 提供三件套（除纯写入 option）：

| Option | 读取 | 设置 |
|--------|------|------|
| `SO_REUSEADDR` | `reuse_addr()` / `reuse_addr(ec)` | `set_reuse_addr(bool)` / `set_reuse_addr(bool, ec)` |
| `SO_REUSEPORT` | 同上 | 同上 |
| `SO_KEEPALIVE` | 同上 | 同上 |
| `TCP_NODELAY` | 同上 | 同上 |
| `SO_BROADCAST` | 同上 | 同上 |
| `SO_RCVBUF` | `recv_buffer_size()` → int | `set_recv_buffer_size(int)` |
| `SO_SNDBUF` | `send_buffer_size()` | `set_send_buffer_size(int)` |
| `SO_LINGER` | `linger_opt()` → `struct linger` | `set_linger(bool on, int sec)` |
| `SO_RCVTIMEO` | `recv_timeout()` → `milliseconds` | `set_recv_timeout(ms)` |
| `SO_SNDTIMEO` | `send_timeout()` | `set_send_timeout(ms)` |

**类型映射注意**：

- `SO_RCVTIMEO` 用户侧是 `std::chrono::milliseconds`，内部 `to_timeval` 转换。
- `IP_MULTICAST_TTL` 内核接受 `unsigned char`，不是 `int`。代码里显式 `getsockopt<unsigned char>`，避免某些内核实现把 4 字节的 `int` 截断错误。

### 4.7 组播接口

```cpp
int  multicast_ttl() const;
void set_multicast_ttl(int ttl);                       // 0..255
void join_multicast_group(const Address& group,
                          const Address& iface = Address{});
void leave_multicast_group(const Address& group,
                           const Address& iface = Address{});
void set_multicast_interface(const Address& iface);
void set_multicast_loop(bool on);
```

`join_multicast_group` 构造 `ip_mreq`：

```cpp
struct ip_mreq mreq{};
if (!group.to_in_addr(mreq.imr_multiaddr)) { ec = errc::invalid_argument; return; }
if (iface.empty()) mreq.imr_interface.s_addr = htonl(INADDR_ANY);
else if (!iface.to_in_addr(mreq.imr_interface)) { ec = errc::invalid_argument; return; }
setsockopt<ip_mreq>(IPPROTO_IP, IP_ADD_MEMBERSHIP, mreq, ec);
```

`iface.empty()` 时绑 `INADDR_ANY`，符合内核默认选择路由接口的行为。

### 4.8 非阻塞开关

`set_nonblocking(bool)` / `is_nonblocking()` 通过 `fcntl(F_GETFL / F_SETFL)` 操作 `O_NONBLOCK` 位。`is_nonblocking()` 在 `ec` 非空时返回 `false`，异常版抛 `SocketError`。

### 4.9 getsockopt / setsockopt 模板

```cpp
template <typename T>
T getsockopt(int level, int optname, std::error_code& ec) const noexcept {
    ec.clear();
    T v{};
    socklen_t len = static_cast<socklen_t>(sizeof(T));
    getsockopt_impl(level, optname, &v, &len, ec);
    if (ec) return T{};
    if (len != sizeof(T)) {                    // 长度不符 → 类型选错了
        ec = std::make_error_code(std::errc::invalid_argument);
        return T{};
    }
    return v;
}
```

**长度校验的必要性**：Linux 某些 option 返回的 `socklen_t` 小于 `sizeof(T)`（如 `SO_RCVBUF` 在旧内核），强转 T 会读到未初始化字节。校验后要么成功要么明确报错。

### 4.10 关闭与半关闭

```cpp
void close() noexcept;          // 幂等；Linux 上 close EINTR 不重试
void shutdown_write();          // SHUT_WR
void shutdown_read();           // SHUT_RD
void shutdown_both();           // SHUT_RDWR
bool closed() const noexcept;   // fd_ < 0
int  fileno() const noexcept;   // 裸 fd，供 Selector / 用户直接用
```

**Linux `close(2)` 的 EINTR 陷阱**：POSIX 允许 `close` 被信号中断，但 Linux 上 fd 已释放，不能重试。代码注释明确说明，直接 `::close(fd_)` 不循环。

---

## 5. Selector 模块

### 5.1 Event 位掩码

```cpp
enum class Event : uint32_t {
    None   = 0,
    Read   = 1 << 0,
    Write  = 1 << 1,
    Error  = 1 << 2,
    Hangup = 1 << 3,

    EdgeTrigger = 1u << 16,   // 模式位，不参与事件匹配
};
```

**关键设计**：`Event::EdgeTrigger` 放在 bit 16，与低 8 位的事件位隔离。原因是 `has_event(set, e)` 只应判断"事件"，不应把模式位当成事件。为此提供：

```cpp
inline Event event_bits(Event e);           // 屏蔽高 24 位，只保留低 8 位事件
inline bool  has_edge_trigger(Event e);     // 检测 ET
```

**为什么 ET 也是 Event 的 bit**：让用户可以用 `Event::Read | Event::EdgeTrigger` 一次表达 "读 + 边缘触发"，`Selector::add` 一个参数搞定，无需额外 API。语义上 ET 确实是 epoll 注册参数的一部分，作为 Event 的扩展位合理。

### 5.2 事件转换对照

| unpsock | select | poll | epoll |
|---------|--------|------|-------|
| `Read` | `readfds` | `POLLIN` | `EPOLLIN` |
| `Write` | `writefds` | `POLLOUT` | `EPOLLOUT` |
| `Error` | `exceptfds` | `POLLERR` / `POLLNVAL` | `EPOLLERR` |
| `Hangup` | — | `POLLHUP` | `EPOLLHUP` |
| `EdgeTrigger` | 忽略 | 忽略 | `EPOLLET` |

**语义差异说明**：

- `select` 的 `exceptfds` 只捕获 OOB 与 connect 失败，不覆盖 `POLLERR` 的完整语义——文档中明确 SelectSelector 在 Error 检测上弱于 poll/epoll。
- `poll` / `epoll` 的 `POLLERR` / `EPOLLERR` 与 `POLLHUP` / `EPOLLHUP` 是**总是被报告**的（无需注册），因此 `add` 时即使用户没请求 `Error`，`wait` 返回的 `ReadyEvent` 也带 `Error`——调用方应总是检查 `Error` / `Hangup`。

### 5.3 Selector 抽象接口

```cpp
class Selector {
public:
    virtual ~Selector() = default;
    virtual bool supports_edge_trigger() const noexcept { return false; }

    // 四个公有封装（异常版 + 错误码版 × 3 个操作）
    void add(int fd, Event events);
    void add(int fd, Event events, std::error_code& ec);
    void modify(int fd, Event events);
    void modify(int fd, Event events, std::error_code& ec);
    void remove(int fd);
    void remove(int fd, std::error_code& ec);

    std::vector<ReadyEvent> wait(std::optional<std::chrono::milliseconds> = std::nullopt);
    std::vector<ReadyEvent> wait(std::optional<std::chrono::milliseconds>, std::error_code& ec);

    // Socket 重载（转 fileno() 后调上面的）
    void add(const Socket&, Event);
    // ... 等

private:
    virtual void add_impl(int, Event, std::error_code&) = 0;
    virtual void modify_impl(int, Event, std::error_code&) = 0;
    virtual void remove_impl(int, std::error_code&) = 0;
    virtual std::vector<ReadyEvent> wait_impl(std::optional<std::chrono::milliseconds>,
                                              std::error_code&) = 0;
};
```

**NVI 模式**：公有非虚接口负责异常/错误码双风格转换，私有虚接口是纯错误码版——子类实现只需写一份逻辑。

**`wait` 的超时语义**：

- `std::nullopt` → 永久阻塞
- `0ms` → 非阻塞轮询
- `> 0` → 超时返回空 vector（`ec` 为空，与出错区分）

### 5.4 ReadyEvent

```cpp
struct ReadyEvent {
    int   fd;
    Event events;   // 只含 Read/Write/Error/Hangup，不含 EdgeTrigger
};
```

`wait()` 返回 `vector<ReadyEvent>`，调用方按顺序处理。不做 fd → 回调的分发，保持库的"零框架"定位。

### 5.5 SelectSelector 实现

注册表：`std::map<int, Event> map_`（用 `map` 而非 `unordered_map` 因为 `select` 需要遍历时按 fd 顺序，`map` 遍历顺序稳定，便于调试）。

`wait_impl`：

```cpp
fd_set r, w, e; FD_ZERO × 3;
int maxfd = -1;
for (auto& [fd, ev] : map_) {
    if (fd < 0 || fd >= FD_SETSIZE) continue;    // add 已校验，防一手
    if (has_event(ev, Read))  FD_SET(fd, &r);
    if (has_event(ev, Write)) FD_SET(fd, &w);
    FD_SET(fd, &e);                               // 总是关心 except
    maxfd = std::max(maxfd, fd);
}
timeval tv; timeval* tvp = nullptr;
if (timeout) { tv.tv_sec = count/1000; tv.tv_usec = (count%1000)*1000; tvp = &tv; }
int n = ::select(maxfd + 1, &r, &w, &e, tvp);   // EINTR 循环
```

**限制**：`fd >= FD_SETSIZE`（通常 1024）时 `add_impl` 返回 `errc::invalid_argument`。

**EINTR 处理**：`select` 被信号打断时 `n < 0 && errno == EINTR`，`do/while` 重试。注意 `tvp` 在 Linux 上会被 `select` 修改（剩余时间），EINTR 后重试会导致超时时间被缩短——这是 POSIX 已知行为，不修正（修正需重新计算 `tvp`，增加复杂度且收益有限）。

### 5.6 PollSelector 实现

注册表：`std::unordered_map<int, Event> map_`（`poll` 无需有序）。

`wait_impl`：

```cpp
std::vector<pollfd> pfds;
pfds.reserve(map_.size());
for (auto& [fd, ev] : map_) {
    pollfd p{}; p.fd = fd; p.events = to_poll(ev);
    pfds.push_back(p);
}
int n = ::poll(pfds.data(), pfds.size(), timeout_ms);  // EINTR 循环
if (n == 0) return {};
std::vector<ReadyEvent> result;
for (auto& p : pfds) {
    if (p.revents == 0) continue;
    Event ready = from_poll(p.revents);
    if (ready != Event::None) result.push_back({p.fd, ready});
}
```

`from_poll` 映射：

```cpp
if (revents & (POLLIN | POLLPRI)) ev |= Read;
if (revents & POLLOUT)            ev |= Write;
if (revents & POLLERR)            ev |= Error;
if (revents & POLLHUP)            ev |= Hangup;
if (revents & POLLNVAL)           ev |= Error;   // 无效 fd 归为 Error
```

**每次 `wait` 重建 `pfds` 数组**：`poll` 需要传入完整数组，无法像 `epoll` 那样内核缓存。`reserve` 避免重复分配。这是 `poll` 的固有限制（O(n) 每次 wait），在 fd 数 < 1000 时通常可接受。

### 5.7 EpollSelector 实现

**成员**：

```cpp
int epfd_ = -1;
std::unordered_map<int, Event> map_;   // fd → 用户请求的 Event，用于 modify/del 校验
```

`map_` 的作用：epoll 内核态已保存 fd 状态，但用户态 `modify` / `remove` 需要快速判断 "fd 是否已注册" 并返回 `AlreadyRegistered` / `NotRegistered`。`map_` 就是用户态索引。

**init 分离**：`init()` 可能失败（`epoll_create1` ENOMEM 等），因此构造不直接调用，由 `make_epoll_selector(ec)` 工厂负责：

```cpp
inline std::unique_ptr<Selector> make_epoll_selector(std::error_code& ec) {
    ec.clear();
    auto p = std::unique_ptr<EpollSelector>(new EpollSelector());
    if (!p->init(ec)) return nullptr;
    return p;
}
```

用户也可手动 `EpollSelector s; s.init();`（异常版）。

**add_impl**：

```cpp
if (!check_init(ec)) return;                    // epfd_ < 0 → EBADF
if (fd < 0) { ec = errc::invalid_argument; return; }
if (map_.count(fd)) { ec = make_selector_ec(AlreadyRegistered); return; }

epoll_event ev{};
ev.events  = to_epoll(events);   // Read/Write/ET
ev.data.fd = fd;
if (::epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) != 0) {
    ec = std::error_code(errno, std::system_category());
    return;
}
map_[fd] = events;
```

**`to_epoll`**：

```cpp
uint32_t v = 0;
if (has_event(e, Read))  v |= EPOLLIN;
if (has_event(e, Write)) v |= EPOLLOUT;
if (has_edge_trigger(e)) v |= EPOLLET;
// EPOLLERR / EPOLLHUP 总是被报告，无需显式请求
```

**remove_impl**：老内核 `EPOLL_CTL_DEL` 不接受 `nullptr` 事件参数，传 dummy：

```cpp
struct epoll_event dummy{};
if (::epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &dummy) != 0) { ... }
```

**wait_impl**：

```cpp
constexpr int MAX_EVENTS = 64;
epoll_event evs[MAX_EVENTS];
int n = ::epoll_wait(epfd_, evs, MAX_EVENTS, timeout_ms);   // EINTR 循环
if (n == 0) return {};
std::vector<ReadyEvent> result;
result.reserve(n);
for (int i = 0; i < n; ++i) {
    result.push_back({evs[i].data.fd, from_epoll(evs[i].events)});
}
```

**`MAX_EVENTS = 64`**：栈上固定数组，避免每次 `wait` 分配。若单次就绪 fd 超过 64，`epoll_wait` 只返回前 64，下次 `wait` 会继续返回剩余（水平触发语义保证）。对 ET 模式，调用方必须在 `Read` 事件中 `while(recv > 0)` 读到 `EAGAIN`，否则会丢事件——这属于 ET 的使用契约。

**`from_epoll`**：

```cpp
if (e & (EPOLLIN | EPOLLPRI)) ev |= Read;
if (e & EPOLLOUT)             ev |= Write;
if (e & EPOLLERR)             ev |= Error;
if (e & EPOLLHUP)             ev |= Hangup;
// EPOLLRDHUP 需注册，这里不处理
```

**`supports_edge_trigger()` 返回 true**，用户可通过 `Event::Read | Event::EdgeTrigger` 启用 ET。

### 5.8 后端选择策略

库不强制用户用哪个后端，但提供工厂函数：

```cpp
std::unique_ptr<Selector> make_select_selector();
std::unique_ptr<Selector> make_select_selector(std::error_code&);
std::unique_ptr<Selector> make_poll_selector();
std::unique_ptr<Selector> make_poll_selector(std::error_code&);
std::unique_ptr<Selector> make_epoll_selector(std::error_code&);   // 可能失败
```

**推荐**：Linux 目标平台直接 `EpollSelector`。`select` / `poll` 作为 fallback 或教学/调试对照。

---

## 6. 主机名工具

`socket_host.hpp` 仅一个函数：

```cpp
inline std::string local_hostname() {
    char buf[256];
    if (::gethostname(buf, sizeof(buf)) != 0) return "";
    buf[sizeof(buf) - 1] = '\0';
    return buf;
}
```

**设计要点**：

- 内联，无状态，无异常——失败返回空串而非抛异常，因为 "取主机名" 在日志/诊断场景常被调用，抛异常反而麻烦。
- 防御性 `\0` 截断：POSIX 不保证 `gethostname` 在截断时写入 `\0`（Linux 会，但跨实现不保证）。
- 不做正向 DNS 反查（那是 `Address::resolve` 的职责）。

---

## 7. 使用示例

### 7.1 TCP echo 服务端（epoll + 错误码）

```cpp
using namespace unpsock;

EpollSelector sel;
sel.init();   // 异常版，失败抛 SelectorError

Socket listen_sock(Family::INET, AddrType::STREAM);
listen_sock.set_reuse_addr(true);
listen_sock.bind(Address::any(8080));
listen_sock.listen(128);
listen_sock.set_nonblocking(true);

sel.add(listen_sock, Event::Read);

while (true) {
    auto ready = sel.wait(std::chrono::milliseconds(1000));
    for (auto& re : ready) {
        if (re.fd == listen_sock.fileno()) {
            while (true) {
                std::error_code ec;
                auto [conn, peer] = listen_sock.accept(ec);
                if (ec == std::errc::resource_unavailable_try_again) break; // EAGAIN
                if (ec) { /* log */ break; }
                conn.set_nonblocking(true);
                sel.add(conn.fileno(), Event::Read);
                // 把 conn 存入 map<int, Socket> 管理生命周期
            }
        } else {
            // 处理读事件...
            if (has_event(re.events, Event::Hangup | Event::Error)) {
                sel.remove(re.fd);
                // close
            }
        }
    }
}
```

### 7.2 异常风格快速原型

```cpp
Socket s(Family::INET, AddrType::STREAM);
s.connect_timed("example.com", 80, std::chrono::seconds(3));
s.sendall("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
auto data = s.recv(4096);
std::cout << std::string(data.begin(), data.end());
```

### 7.3 UDP 组播接收

```cpp
Socket s(Family::INET, AddrType::DGRAM);
s.set_reuse_addr(true);
s.bind(Address::any(5353));          // 0.0.0.0:5353
s.join_multicast_group(Address::from_ip("224.0.0.251", 0));

auto [buf, peer] = s.recvfrom(1500);
printf("from %s: %zu bytes\n", peer.to_string().c_str(), buf.size());
```

### 7.4 后端切换（同一份业务代码）

```cpp
std::unique_ptr<Selector> make_selector_by_name(const std::string& name,
                                                std::error_code& ec) {
    if (name == "epoll") return make_epoll_selector(ec);
    if (name == "poll")  return make_poll_selector(ec);
    if (name == "select")return make_select_selector(ec);
    ec = make_selector_ec(SelectorErrc::BackendUnavailable);
    return nullptr;
}
```

---

## 8. 约束、限制与已知问题

| 项 | 说明 |
|----|------|
| 平台 | 仅 POSIX / Linux。`epoll` 接口仅在 Linux 可用；`select` / `poll` 可移植到其它 POSIX。 |
| `select` fd 上限 | `FD_SETSIZE`（Linux 默认 1024）。`SelectSelector::add` 会拒绝 `fd >= FD_SETSIZE`。 |
| `poll` 每次 O(n) | `wait_impl` 每次重建 `pollfd` 数组并线性扫描结果。fd 数 > 数千时应选 epoll。 |
| `epoll` ET 语义 | 用户请求 `Event::EdgeTrigger` 时，`wait` 返回的 `Read` 事件必须**读到 `EAGAIN`** 才停止，否则丢事件。库不代做循环。 |
| `epoll` 单次 wait 上限 | `MAX_EVENTS = 64`。就绪 > 64 时下次 `wait` 继续返回（LT 保证），但 ET 下需用户循环处理。 |
| `sendall` 部分发送 | 出错时返回已发送字节数，调用方必须检查 `ec`，不能仅看返回值。 |
| `connect_timed` 恢复阻塞 | 成功/失败都会恢复原 `O_NONBLOCK` 状态，调用方无需关心。 |
| `recv` 的 `bufsize == 0` | 返回 `errc::invalid_argument`，不返回空 vector（避免与 "对端关闭" 混淆）。 |
| IPv6 组播 | 当前 `join_multicast_group` 仅支持 IPv4 `ip_mreq`；IPv6 组播（`ipv6_mreq`）未实现。 |
| `SO_LINGER` 秒数 | 内核单位为秒（`int`），不支持亚秒。需要亚秒级 RST 应立即 `setsockopt(SO_LINGER, {1, 0})` 然后 `close`。 |
| `MSG_NOSIGNAL` | 发送路径统一使用，抑制 SIGPIPE。若用户需要 SIGPIPE 语义，可绕过 `Socket::send*` 直接用 `fileno()` + `::send`。 |

---

## 9. 测试要点

### 9.1 Address

- `from_ip` 四组合：`("127.0.0.1", AF_INET)` / `("::1", AF_INET6)` / `("127.0.0.1", UNSPEC)` / `("::1", UNSPEC)`
- `from_unix` 边界：空串、107 字节、108 字节（应 `filename_too_long`）
- `resolve_all`：`"localhost"` / `"127.0.0.1"` / 不存在域名（`gai_category`）
- `operator==`：同地址、不同端口、不同家族、Unix 路径
- `to_string`：IPv4 / IPv6 / Unix 格式

### 9.2 Socket

- 移动语义：move 后源 `valid() == false`，析构不重复 close
- `sendall` 大包（> socket buffer）：循环发送完整
- `sendall` 对端关闭：部分发送后 `ec` 非空
- `connect_timed` 三种结局：成功 / 拒绝（`ECONNREFUSED`）/ 超时
- `getsockopt<T>` 长度不符：如 `getsockopt<int>(SOL_SOCKET, SO_LINGER, ec)` 应 `invalid_argument`
- 所有 `set_*` 无效参数：负 buffer size / 负 timeout / TTL > 255

### 9.3 Selector

- 三种后端跑同一套用例：注册 / 修改 / 删除 / 重复注册 / 未注册删除
- 超时语义：`nullopt` 阻塞、`0ms` 立即返回、`100ms` 空等待
- `EINTR` 干扰：注册 SIGALRM 后 `wait` 应自动重试
- epoll ET：大量数据到达时只触发一次，用户循环读完
- `SelectorError`：`AlreadyRegistered` / `NotRegistered` 的异常与 `ec` 双路径

---

## 10. 未来扩展（非本期范围）

1. **IPv6 组播**：`join_multicast_group` 重载 `ipv6_mreq`，或引入 `MulticastGroup` 类型按 `Address::family()` 分派。
2. **`sendfile` / `splice` 零拷贝**：为文件/管道转发场景提供 `Socket::send_file(fd, offset, len)`。
3. **`MSG_MORE` / `TCP_CORK` 便捷封装**：TCP 小包合并。
4. **`SO_REUSEPORT` 负载均衡示例**：多线程 accept 场景的文档与测试。
5. **`Event::Error` 携带 errno**：当前 `ReadyEvent` 只有 `fd` + `Event`，未来可加 `std::error_code error` 字段（需扩展 `wait_impl` 的内部结构，`epoll` 可读 `SO_ERROR`）。

---

## 附录 A：头文件依赖矩阵

| 头文件 | 依赖 |
|--------|------|
| `socket_exception.hpp` | `<system_error>` `<netdb.h>` |
| `socket_host.hpp` | `<unistd.h>` `<string>` |
| `socket_address.hpp` | `socket_exception.hpp`, `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<netdb.h>`, `<sys/un.h>` |
| `socket_socket.hpp` | `socket_address.hpp`, `socket_exception.hpp`, `<unistd.h>`, `<fcntl.h>`, `<sys/socket.h>`, `<netinet/tcp.h>` |
| `socket_selector.hpp` | `socket_socket.hpp`, `<vector>`, `<chrono>`, `<optional>`, `<memory>` |
| `socket_select.hpp` | `socket_selector.hpp`, `<sys/select.h>`, `<map>` |
| `socket_poll.hpp` | `socket_selector.hpp`, `<poll.h>`, `<unordered_map>` |
| `socket_epoll.hpp` | `socket_selector.hpp`, `<sys/epoll.h>`, `<unordered_map>` |

## 附录 B：命名空间与命名规范

- 根命名空间：`unpsock`
- 内部实现：`unpsock::detail`（如 `to_af` / `from_af` / `to_socktype`）
- 主机工具：`unpsock::host`（如 `host::local_hostname`）
- 类名：`Address` / `Socket` / `Selector` / `SelectSelector` / `PollSelector` / `EpollSelector` / `Event` / `ReadyEvent`
- 异常：`SocketException` / `AddressError` / `SocketError` / `SelectorError`
- 错误码枚举：`SelectorErrc`
- 工厂函数：`make_select_selector` / `make_poll_selector` / `make_epoll_selector`
- 宏兼容：`SUN_LEN` 在未定义时补齐（非 Linux 平台兼容）