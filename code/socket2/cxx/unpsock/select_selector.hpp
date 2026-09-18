// select_selector.hpp
#pragma once

#include "socket_selector.hpp"

#include <sys/select.h>
#include <map>
#include <algorithm>
#include <cerrno>
#include <system_error>
#include <optional>

namespace unpsock {

class SelectSelector : public Selector {
public:
    SelectSelector() = default;
    ~SelectSelector() override = default;

private:
    // ============================================================
    // add
    // ============================================================
    void add_impl(int fd, Event events, std::error_code& ec) override {
        ec.clear();
        if (fd < 0 || fd >= FD_SETSIZE) {
            ec = std::make_error_code(std::errc::invalid_argument);
            return;
        }
        if (map_.count(fd)) {
            ec = make_selector_ec(SelectorErrc::AlreadyRegistered);
            return;
        }
        map_[fd] = events;
    }

    // ============================================================
    // modify
    // ============================================================
    void modify_impl(int fd, Event events, std::error_code& ec) override {
        ec.clear();
        if (fd < 0 || fd >= FD_SETSIZE) {
            ec = std::make_error_code(std::errc::invalid_argument);
            return;
        }
        auto it = map_.find(fd);
        if (it == map_.end()) {
            ec = make_selector_ec(SelectorErrc::NotRegistered);
            return;
        }
        it->second = events;
    }

    // ============================================================
    // remove
    // ============================================================
    void remove_impl(int fd, std::error_code& ec) override {
        ec.clear();
        auto it = map_.find(fd);
        if (it == map_.end()) {
            ec = make_selector_ec(SelectorErrc::NotRegistered);
            return;
        }
        map_.erase(it);
    }

    // ============================================================
    // wait
    // ============================================================
    std::vector<ReadyEvent> wait_impl(
        std::optional<std::chrono::milliseconds> timeout,
        std::error_code& ec) override
    {
        ec.clear();

        // 没有注册任何 fd：如果 timeout 是 0 立即返回；否则按 timeout 睡眠
        // 这里直接走 select(0, nullptr, ...) 也可以，但语义上更清晰的做法是：
        // 若无 fd 且无 timeout → 直接返回空（不阻塞）；若有 timeout → sleep 后返回空。
        if (map_.empty()) {
            if (timeout.has_value() && timeout->count() > 0) {
                struct timespec ts;
                ts.tv_sec  = timeout->count() / 1000;
                ts.tv_nsec = (timeout->count() % 1000) * 1000000L;
                ::nanosleep(&ts, nullptr);
            }
            return {};
        }

        fd_set readfds, writefds, exceptfds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);

        int maxfd = -1;
        for (const auto& [fd, ev] : map_) {
            // add 时已检查过，这里再防一手
            if (fd < 0 || fd >= FD_SETSIZE) continue;

            if (has_event(ev, Event::Read))  FD_SET(fd, &readfds);
            if (has_event(ev, Event::Write)) FD_SET(fd, &writefds);
            FD_SET(fd, &exceptfds);   // 总是关心异常
            maxfd = std::max(maxfd, fd);
        }

        struct timeval tv;
        struct timeval* tvp = nullptr;
        if (timeout.has_value()) {
            tv.tv_sec  = timeout->count() / 1000;
            tv.tv_usec = (timeout->count() % 1000) * 1000;
            tvp = &tv;
        }

        int n;
        do {
            n = ::select(maxfd + 1, &readfds, &writefds, &exceptfds, tvp);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return {};
        }
        if (n == 0) return {};   // 超时

        std::vector<ReadyEvent> result;
        result.reserve(static_cast<size_t>(n));
        for (const auto& [fd, ev] : map_) {
            Event ready = Event::None;
            if (FD_ISSET(fd, &readfds))   ready = ready | Event::Read;
            if (FD_ISSET(fd, &writefds))  ready = ready | Event::Write;
            if (FD_ISSET(fd, &exceptfds)) ready = ready | Event::Error;
            if (ready != Event::None) {
                result.push_back({fd, ready});
            }
        }
        return result;
    }

    SelectSelector(const SelectSelector&) = delete;
    SelectSelector& operator=(const SelectSelector&) = delete;

    SelectSelector(SelectSelector&& other) = delete;
    SelectSelector& operator=(SelectSelector&& other) = delete;

private:
    std::map<int, Event> map_;
};

inline
std::unique_ptr<Selector> make_select_selector() {
    return std::make_unique<SelectSelector>();
}

} // namespace unpsock
