#pragma once

#include <cassert>
#include <functional>

class Task {
private:
    std::function<void()> func;

public:
    Task() = default;
    ~Task() = default;

    template <typename F>
    explicit Task(F&& f): func(std::forward<F>(f)) {}

    Task(const Task&) = delete;
    Task(Task&&) = default;

    Task& operator=(const Task&) = delete;
    Task& operator=(Task&&) = default;

    void Run() {
        if (func) {
            func();
        } else {
            assert(false && "func is empty!");
        }
    }
};

template <typename Fn, typename... Args>
Task MakeTask(Fn&& fn, Args&&... args)
{
    return Task(std::bind<void>(std::forward<Fn>(fn), std::forward<Args>(args)...));
}

