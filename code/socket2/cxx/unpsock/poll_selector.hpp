// poll_selector.hpp
#pragma once

#include "socket_selector.hpp"

#include <poll.h>
#include <unordered_map>
#include <algorithm>
#include <cerrno>
#include <system_error>

namespace unpsock {

class PollSelector : public Selector {
public:
    PollSelector() = default;
    ~PollSelector() override = default;

private:
    // ============================================================
    // add
    // ============================================================
    void add_impl(int fd, Event events, std::error_code& ec) override {
        ec.clear();
        if (fd < 0) {
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
        if (fd < 0) {
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

        std::vector<struct pollfd> pfds;
        pfds.reserve(map_.size());
        for (const auto& [fd, ev] : map_) {
            struct pollfd p{};
            p.fd = fd;
            p.events = to_poll(ev);
            pfds.push_back(p);
        }

        int timeout_ms = timeout.has_value()
            ? static_cast<int>(timeout->count())
            : -1;

        int n;
        do {
            n = ::poll(pfds.data(), pfds.size(), timeout_ms);
        } while (n < 0 && errno == EINTR);

        if (n < 0) {
            ec = std::error_code(errno, std::system_category());
            return {};
        }
        if (n == 0) return {};   // 超时

        std::vector<ReadyEvent> result;
        result.reserve(static_cast<size_t>(n));
        for (const auto& p : pfds) {
            if (p.revents == 0) continue;
            Event ready = from_poll(p.revents);
            if (ready != Event::None) {
                result.push_back({p.fd, ready});
            }
        }
        return result;
    }

    PollSelector(const PollSelector&) = delete;
    PollSelector& operator=(const PollSelector&) = delete;

    PollSelector(PollSelector&& other) = delete;
    PollSelector& operator=(PollSelector&& other) = delete;

private:
    std::unordered_map<int, Event> map_;

    static short to_poll(Event e) {
        short v = 0;
        if (has_event(e, Event::Read))   v |= POLLIN;
        if (has_event(e, Event::Write))  v |= POLLOUT;
        // POLLERR/POLLHUP 总是被报告，不需要显式请求
        return v;
    }

    static Event from_poll(short revents) {
        Event ev = Event::None;
        if (revents & (POLLIN | POLLPRI)) ev = ev | Event::Read;
        if (revents & POLLOUT)            ev = ev | Event::Write;
        if (revents & POLLERR)            ev = ev | Event::Error;
        if (revents & POLLHUP)            ev = ev | Event::Hangup;
        if (revents & POLLNVAL)           ev = ev | Event::Error;
        return ev;
    }
};

inline
std::unique_ptr<Selector> make_poll_selector() {
    return std::make_unique<PollSelector>();
}

inline
std::unique_ptr<Selector> make_poll_selector(std::error_code& ec) {
    ec.clear();
    return std::make_unique<PollSelector>();
}

} // namespace unpsock
