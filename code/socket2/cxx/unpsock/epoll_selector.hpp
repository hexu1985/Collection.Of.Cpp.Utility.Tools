// epoll_selector.hpp
#pragma once

#include "selector.hpp"

#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>
#include <cerrno>
#include <system_error>

namespace mysock {

class EpollSelector : public Selector {
public:
    // 构造失败通过静态工厂返回 ec 更自然，这里用默认构造 + 内部记录初始化错误
    EpollSelector() : epfd_(-1) {
        epfd_ = ::epoll_create1(EPOLL_CLOEXEC);
        // 构造阶段无法通过 ec 出参返回，这里记录一个标志，第一次使用时暴露
        if (epfd_ < 0) init_errno_ = errno;
    }

    bool check_init() const {
    }

    bool check_init(std::error_code& ec) const {
        if (epfd_ < 0) {
            ec = std::error_code(init_errno_ ? init_errno_ : EBADF,
                                 std::system_category());
            return false;
        }
        return true;
    }

    // 供工厂使用的静态工厂：带 ec 的构造
    static std::unique_ptr<EpollSelector> create(std::error_code& ec) {
        ec.clear();
        auto p = std::unique_ptr<EpollSelector>(new EpollSelector());
        if (p->epfd_ < 0) {
            ec = std::error_code(p->init_errno_, std::system_category());
            p.reset();
        }
        return p;
    }

    ~EpollSelector() override {
        if (epfd_ >= 0) ::close(epfd_);
    }

    EpollSelector(const EpollSelector&) = delete;
    EpollSelector& operator=(const EpollSelector&) = delete;

private:
    // ============================================================
    // add
    // ============================================================
    void add_impl(int fd, Event events, std::error_code& ec) override {
        ec.clear();
        if (!check_init(ec)) return;
        if (fd < 0) {
            ec = std::make_error_code(std::errc::invalid_argument);
            return;
        }
        if (map_.count(fd)) {
            ec = make_selector_ec(SelectorErrc::AlreadyRegistered);
            return;
        }

        struct epoll_event ev{};
        ev.events  = to_epoll(events);
        ev.data.fd = fd;

        if (::epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) != 0) {
            ec = std::error_code(errno, std::system_category());
            return;
        }
        map_[fd] = events;
    }

    // ============================================================
    // modify
    // ============================================================
    void modify(int fd, Event events, std::error_code& ec) override {
        ec.clear();
        if (!check_init(ec)) return;
        if (fd < 0) {
            ec = std::make_error_code(std::errc::invalid_argument);
            return;
        }
        auto it = map_.find(fd);
        if (it == map_.end()) {
            ec = make_selector_ec(SelectorErrc::NotRegistered);
            return;
        }

        struct epoll_event ev{};
        ev.events  = to_epoll(events);
        ev.data.fd = fd;

        if (::epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) != 0) {
            ec = std::error_code(errno, std::system_category());
            return;
        }
        it->second = events;
    }

    // ============================================================
    // remove
    // ============================================================
    void remove(int fd, std::error_code& ec) override {
        ec.clear();
        if (!check_init(ec)) return;
        auto it = map_.find(fd);
        if (it == map_.end()) {
            ec = make_selector_ec(SelectorErrc::NotRegistered);
            return;
        }

        // EPOLL_CTL_DEL：老内核上参数不能为 nullptr，所以传一个 dummy
        struct epoll_event dummy{};
        if (::epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &dummy) != 0) {
            ec = std::error_code(errno, std::system_category());
            return;
        }
        map_.erase(it);
    }

    // ============================================================
    // wait
    // ============================================================
    std::vector<ReadyEvent> wait(
        std::optional<std::chrono::milliseconds> timeout,
        std::error_code& ec) override
    {
        ec.clear();
        if (!check_init(ec)) return {};

        int timeout_ms = -1;
        if (timeout.has_value()) {
            auto c = timeout->count();
            timeout_ms = c <= 0 ? 0 : static_cast<int>(c);
        }

        constexpr int MAX_EVENTS = 64;
        struct epoll_event evs[MAX_EVENTS];

        int n;
        do {
            n = ::epoll_wait(epfd_, evs, MAX_EVENTS, timeout_ms);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return {};
        }
        if (n == 0) return {};

        std::vector<ReadyEvent> result;
        result.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; ++i) {
            result.push_back({evs[i].data.fd, from_epoll(evs[i].events)});
        }
        return result;
    }

private:
    int epfd_ = -1;
    int init_errno_ = 0;
    std::unordered_map<int, Event> map_;

    static uint32_t to_epoll(Event e) {
        uint32_t v = 0;
        // 默认加 EPOLLET？这里不用 ET，和 poll/select 语义保持一致（水平触发）
        if (has_event(e, Event::Read))  v |= EPOLLIN;
        if (has_event(e, Event::Write)) v |= EPOLLOUT;
        // EPOLLERR / EPOLLHUP 总是被报告，无需显式请求
        return v;
    }

    static Event from_epoll(uint32_t e) {
        Event ev = Event::None;
        if (e & (EPOLLIN | EPOLLPRI)) ev = ev | Event::Read;
        if (e & EPOLLOUT)             ev = ev | Event::Write;
        if (e & EPOLLERR)             ev = ev | Event::Error;
        if (e & EPOLLHUP)             ev = ev | Event::Hangup;
        // EPOLLRDHUP 需要注册，这里不处理
        return ev;
    }
};

} // namespace mysock
