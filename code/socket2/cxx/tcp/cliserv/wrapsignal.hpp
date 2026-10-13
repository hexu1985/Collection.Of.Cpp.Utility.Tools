#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <system_error>

using Sigfunc = void(int);

// 底层：出错返回 SIG_ERR 并设置 ec，成功返回旧 handler 并清空 ec
static
Sigfunc *signal(int signo, Sigfunc* func, std::error_code& ec)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
#endif
    }

    if (sigaction(signo, &act, &oact) < 0) {
        ec = std::error_code(errno, std::system_category());
        return SIG_ERR;
    }

    ec.clear();                 // 成功：清空错误码
    return oact.sa_handler;
}

// 上层：出错时抛出 std::system_error
static
Sigfunc* Signal(int signo, Sigfunc* func)
{
    std::error_code ec;
    Sigfunc *oldfunc = signal(signo, func, ec);
    if (ec)
        throw std::system_error(ec, "signal error");
    return oldfunc;
}
