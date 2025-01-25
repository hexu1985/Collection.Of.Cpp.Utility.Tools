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
