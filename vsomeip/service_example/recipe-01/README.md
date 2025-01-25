### First Application（首次申请）

创建第一个 vsomeip 程序，我们称它为 service-example:

```cpp
/*service-example.cpp*/

#include <vsomeip/vsomeip.hpp>

std::shared_ptr< vsomeip::application > app;

int main() {

    app = vsomeip::runtime::get()->create_application("World");
    app->init();
    app->start();
}
```

这很容易：您必须首先创建一个应用程序对象，然后初始化并启动它。 创建 vsomeip 应用程序后必须首先调用 init 方法，并执行以下步骤对其进行初始化：

- Loading the configuration （加载配置）
- Determining routing configuration and initialization of the routing （确定路由配置和路由初始化）
- Installing signal handlers （安装信号处理程序）

> The start method has to be called after init in order to start the message processing. The received messages are processed via sockets and registered callbacks are used to pass them to the user application.

为了启动消息处理，必须在 init 之后调用 start 方法。 接收到的消息通过套接字进行处理，并使用注册的回调将它们传递给用户应用程序。

构建并运行程序。 您应该在控制台上获得以下输出（或类似输出）：

```
2025-01-25 10:51:40.184775  [info] Parsed vsomeip configuration in 0ms
2025-01-25 10:51:40.185149  [info] Configuration module loaded.
2025-01-25 10:51:40.185198  [info] Security disabled!
2025-01-25 10:51:40.185225  [info] Initializing vsomeip (3.5.4) application "World".
2025-01-25 10:51:40.185565  [info] Instantiating routing manager [Host].
2025-01-25 10:51:40.186381  [info] create_routing_root: Routing root @ /tmp/vsomeip-0
2025-01-25 10:51:40.188346  [info] Service Discovery enabled. Trying to load module.
2025-01-25 10:51:40.198275  [info] Service Discovery module loaded.
2025-01-25 10:51:40.198557  [info] Application(World, 0100) is initialized (11, 100).
2025-01-25 10:51:40.198627  [info] Starting vsomeip application "World" (0100) using 2 threads I/O nice 0
2025-01-25 10:51:40.199587  [info] Client [0100] routes unicast:127.0.0.1, netmask:255.255.255.0
2025-01-25 10:51:40.199585  [info] main dispatch thread id from application: 0100 (World) is: 713833c006c0 TID: 3747
2025-01-25 10:51:40.200450  [info] shutdown thread id from application: 0100 (World) is: 7138332006c0 TID: 3748
2025-01-25 10:51:40.201063  [info] Watchdog is disabled!
2025-01-25 10:51:40.201514  [info] io thread id from application: 0100 (World) is: 7138352ecb80 TID: 3745
2025-01-25 10:51:40.202004  [info] io thread id from application: 0100 (World) is: 713831e006c0 TID: 3750
2025-01-25 10:51:40.202870  [warning] Network interface "lo" state changed: up
2025-01-25 10:51:40.204169  [info] vSomeIP 3.5.4 | (default)
```

请注意：

- 这些步骤对于服务和客户端是相同的； 没有区别。 它只是一个 vsomeip 应用程序。
- 到目前为止，您不需要任何配置文件。


让我们详细讨论一些要点：

- 首先你看到已经加载的配置；您没有配置，因此使用默认值。

- 您没有为您的应用程序配置客户端 ID；因此，vsomeip 功能（自动配置）会找到适当的client ID。第一个数字是 0x0001。

- 路由管理器也没有配置；因此，路由管理器会随着系统中的第一个 vsomeip 应用程序自动启动，这就是 service-example。

- 默认情况下启用服务发现，没有静态路由。这将需要一些配置参数。

- 最后一个 init() 输出是 Application(World, 1) is initialized (11, 100)。最后的两个数字意味着如果回调阻塞超过 100 毫秒，则 vsomeip 使用的最大调度程序数量为 11。可以配置这些参数。

- 默认创建两个线程来接收 SOME/IP 消息；这允许 vsomeip 并行处理长消息。

- 然后您会看到当前的 vsomeip 版本，并且 SOME/IP 路由已准备就绪。
