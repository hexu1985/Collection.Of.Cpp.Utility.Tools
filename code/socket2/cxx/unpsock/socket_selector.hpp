// selector.hpp（顶部新增）
#pragma once

#include "socket_socket.hpp"

#include <vector>
#include <chrono>
#include <optional>
#include <memory>
#include <system_error>

namespace unpsock {

// ---------- 事件类型 ----------
enum class Event : uint32_t {
    None   = 0,
    Read   = 1 << 0,
    Write  = 1 << 1,
    Error  = 1 << 2,
    Hangup = 1 << 3,
};

inline Event operator|(Event a, Event b) {
    return static_cast<Event>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline Event operator&(Event a, Event b) {
    return static_cast<Event>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
inline bool has_event(Event set, Event e) {
    return (static_cast<uint32_t>(set) & static_cast<uint32_t>(e)) != 0;
}

struct ReadyEvent {
    int fd;
    Event events;
};

// ---------- Selector 专用错误码 ----------
enum class SelectorErrc {
    Success = 0,
    AlreadyRegistered,   // add 时 fd 已注册
    NotRegistered,       // modify/remove 时 fd 未注册
    BackendUnavailable,  // make_selector 找不到可用后端
    WaitFailed,          // wait 底层失败（errno 会用 system_category）
};

struct SelectorErrorCategory : std::error_category {
    const char* name() const noexcept override { return "Selector"; }
    std::string message(int ev) const override {
        switch (static_cast<SelectorErrc>(ev)) {
            case SelectorErrc::Success:          return "success";
            case SelectorErrc::AlreadyRegistered:return "fd already registered";
            case SelectorErrc::NotRegistered:    return "fd not registered";
            case SelectorErrc::BackendUnavailable:return "no selector backend available";
            case SelectorErrc::WaitFailed:       return "selector wait failed";
        }
        return "unknown selector error";
    }
};

inline const std::error_category& selector_category() {
    static SelectorErrorCategory cat;
    return cat;
}

inline std::error_code make_selector_ec(SelectorErrc e) {
    return std::error_code(static_cast<int>(e), selector_category());
}

// ---------- Selector 异常 ----------
class SelectorError : public SocketException {
public:
    explicit SelectorError(const std::string& what, int errnum = 0)
        : SocketException(errnum, what) {}

    explicit SelectorError(std::error_code ec, const std::string& what)
        : SocketException(ec, what) {}
};

// ---------- Selector 抽象接口 ----------
class Selector {
public:
    virtual ~Selector() = default;

    // ============================================================
    // add
    // ============================================================
    // 异常版
    void add(int fd, Event events) {
        std::error_code ec;
        add(fd, events, ec);
        if (ec) throw SelectorError(ec, "selector add failed: " + ec.message());
    }
    // 错误码版（纯虚，由后端实现）
    virtual void add(int fd, Event events, std::error_code& ec) = 0;

    // ============================================================
    // modify
    // ============================================================
    void modify(int fd, Event events) {
        std::error_code ec;
        modify(fd, events, ec);
        if (ec) throw SelectorError(ec, "selector modify failed: " + ec.message());
    }

    virtual void modify(int fd, Event events, std::error_code& ec) = 0;

    // ============================================================
    // remove
    // ============================================================
    void remove(int fd) {
        std::error_code ec;
        remove(fd, ec);
        if (ec) throw SelectorError(ec, "selector remove failed: " + ec.message());
    }

    virtual void remove(int fd, std::error_code& ec) = 0;

    // ============================================================
    // wait
    // ============================================================
    // 异常版：底层失败抛异常
    std::vector<ReadyEvent> wait(
        std::optional<std::chrono::milliseconds> timeout = std::nullopt) {
        std::error_code ec;
        auto r = wait(timeout, ec);
        if (ec) throw SelectorError(ec, "selector wait failed: " + ec.message());
        return r;
    }

    // 错误码版：失败返回空 vector，ec 出参
    virtual std::vector<ReadyEvent> wait(
        std::optional<std::chrono::milliseconds> timeout,
        std::error_code& ec) = 0;

    // ============================================================
    // 便捷：用 Socket 注册（成对提供）
    // ============================================================
    void add(const Socket& s, Event events) {
        add(s.fileno(), events);
    }

    void add(const Socket& s, Event events, std::error_code& ec) {
        add(s.fileno(), events, ec);
    }

    void modify(const Socket& s, Event events) {
        modify(s.fileno(), events);
    }

    void modify(const Socket& s, Event events, std::error_code& ec) {
        modify(s.fileno(), events, ec);
    }

    void remove(const Socket& s) {
        remove(s.fileno());
    }

    void remove(const Socket& s, std::error_code& ec) {
        remove(s.fileno(), ec);
    }
};

} // namespace mysock
