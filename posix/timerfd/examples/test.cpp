#include <sys/timerfd.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

class TimerFD {
private:
    int fd_;
    bool active_;

public:
    TimerFD() : fd_(-1), active_(false) {}
    
    ~TimerFD() {
        stop();
        if (fd_ != -1) {
            close(fd_);
        }
    }

    bool start(long initial_sec, long initial_nsec, 
               long interval_sec, long interval_nsec) {
        // 创建 timerfd
        fd_ = timerfd_create(CLOCK_MONOTONIC, 0);
        if (fd_ == -1) {
            perror("timerfd_create");
            return false;
        }

        // 设置定时器参数
        struct itimerspec timer_spec;
        timer_spec.it_value.tv_sec = initial_sec;
        timer_spec.it_value.tv_nsec = initial_nsec;
        timer_spec.it_interval.tv_sec = interval_sec;
        timer_spec.it_interval.tv_nsec = interval_nsec;

        if (timerfd_settime(fd_, 0, &timer_spec, NULL) == -1) {
            perror("timerfd_settime");
            close(fd_);
            fd_ = -1;
            return false;
        }

        active_ = true;
        return true;
    }

    bool stop() {
        if (fd_ == -1) return false;

        // 清除定时器
        struct itimerspec timer_spec = {};
        if (timerfd_settime(fd_, 0, &timer_spec, NULL) == -1) {
            perror("timerfd_settime stop");
            return false;
        }

        active_ = false;
        return true;
    }

    int getFD() const { return fd_; }
    bool isActive() const { return active_; }

    bool waitForTimeout() {
        if (fd_ == -1) return false;

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(fd_, &readfds);

        int ret = select(fd_ + 1, &readfds, NULL, NULL, NULL);
        if (ret == -1) {
            if (errno == EINTR) {
                return false; // 被信号中断
            }
            perror("select");
            return false;
        }

        if (FD_ISSET(fd_, &readfds)) {
            uint64_t expirations;
            ssize_t s = read(fd_, &expirations, sizeof(uint64_t));
            if (s != sizeof(uint64_t)) {
                perror("read");
                return false;
            }
            return true;
        }

        return false;
    }
};

int main() {
    TimerFD timer;

    // 启动定时器：1 秒后开始，每 1 秒触发一次
    if (!timer.start(1, 0, 1, 0)) {
        fprintf(stderr, "Failed to start timer\n");
        return 1;
    }

    printf("Timer started. Press Ctrl+C to stop.\n");

    int count = 0;
    while (count < 10) {
        if (timer.waitForTimeout()) {
            printf("Timer tick %d\n", ++count);
        }
    }

    timer.stop();
    printf("Timer stopped.\n");

    return 0;
}
