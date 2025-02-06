### 可用性

现在让我们假设我们的 service-example 是服务，并且我们想要编写一个想要使用该服务的客户端。 第一步，我们必须触发应用程序提供服务实例。 这可以通过在我们的第一个示例中添加offer_service命令来完成：

service-example.cpp with offer

```cpp
#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678

std::shared_ptr< vsomeip::application > app;

int main() {

    app = vsomeip::runtime::get()->create_application("World");
    app->init();
    app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->start();
}
```

在下一步中，我们编写一个应用程序来检查正在运行的 “World” 应用程序是否可用。 考虑下面的 client-example 代码，它创建了一个名为 Hello 的应用程序：

client-example.cpp

```cpp
#include <iomanip>
#include <iostream>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678

std::shared_ptr< vsomeip::application > app;

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is " << (_is_available ? "available." : "NOT available.")  << std::endl;
}

int main() {

    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    app->start();
}
```

为了使其尽可能简单，我们省略了所有可能的检查，例如注册是否成功。 作为客户端，您必须告诉 vsomeip 您想要使用该服务，并且您需要注册一个回调以便在该服务可用时获得呼叫。 客户端输出现在应该类如下：

```
2025-01-25 20:59:39.772803  [info] Parsed vsomeip configuration in 0ms
2025-01-25 20:59:39.773158  [info] Configuration module loaded.
2025-01-25 20:59:39.773207  [info] Security disabled!
2025-01-25 20:59:39.773234  [info] Initializing vsomeip (3.5.4) application "Hello".
2025-01-25 20:59:39.773356  [info] Instantiating routing manager [Host].
2025-01-25 20:59:39.774608  [info] create_routing_root: Routing root @ /tmp/vsomeip-0
2025-01-25 20:59:39.775219  [info] Service Discovery enabled. Trying to load module.
2025-01-25 20:59:39.776982  [info] Service Discovery module loaded.
2025-01-25 20:59:39.777398  [info] Application(Hello, 0100) is initialized (11, 100).
2025-01-25 20:59:39.777753  [info] REQUEST(0100): [1234.5678:255.4294967295]
2025-01-25 20:59:39.778767  [info] create_local_server: Listening @ /tmp/vsomeip-100
2025-01-25 20:59:39.779164  [info] Starting vsomeip application "Hello" (0100) using 2 threads I/O nice 0
2025-01-25 20:59:39.780140  [info] Client [0100] routes unicast:127.0.0.1, netmask:255.255.255.0
2025-01-25 20:59:39.780518  [info] shutdown thread id from application: 0100 (Hello) is: 7fa762a006c0 TID: 5068
2025-01-25 20:59:39.781305  [info] Watchdog is disabled!
2025-01-25 20:59:39.780508  [info] main dispatch thread id from application: 0100 (Hello) is: 7fa7634006c0 TID: 5067
Service [1234.5678] is NOT available.
2025-01-25 20:59:39.782223  [info] io thread id from application: 0100 (Hello) is: 7fa764b57b80 TID: 5065
2025-01-25 20:59:39.782293  [info] io thread id from application: 0100 (Hello) is: 7fa7616006c0 TID: 5070
2025-01-25 20:59:39.782921  [info] vSomeIP 3.5.4 | (default)
2025-01-25 20:59:39.783155  [warning] Network interface "lo" state changed: up
2025-01-25 20:59:49.080946  [info] Application/Client 0101 is registering.
2025-01-25 20:59:49.081470  [info] emb::find_or_create_local: create_client 101
2025-01-25 20:59:49.081695  [info] Client [100] is connecting to [101] at /tmp/vsomeip-101 endpoint > 0x7fa748000e70
2025-01-25 20:59:49.083269  [info] REGISTERED_ACK(0101)
2025-01-25 20:59:49.083664  [info] rmi::offer_service added service: 1234 to pending_sd_offers_.size = 1
2025-01-25 20:59:49.083927  [info] OFFER(0101): [1234.5678:0.0] (true)
Service [1234.5678] is available.
2025-01-25 20:59:49.794669  [info] vSomeIP 3.5.4 | (default)
```

当 app->start() 启动 vsomeip 事件循环时，将调用可用性回调 (on_availability(...) 函数)。

在服务端打印如下语句（log）

```
2025-01-25 20:59:49.074384  [info] Parsed vsomeip configuration in 0ms
2025-01-25 20:59:49.074738  [info] Configuration module loaded.
2025-01-25 20:59:49.074788  [info] Security disabled!
2025-01-25 20:59:49.074816  [info] Initializing vsomeip (3.5.4) application "World".
2025-01-25 20:59:49.074942  [info] Instantiating routing manager [Proxy].
2025-01-25 20:59:49.075142  [info] Client [ffff] is connecting to [0] at /tmp/vsomeip-0 endpoint > 0x5e1f168d55c0
2025-01-25 20:59:49.075191  [info] Application(World, ffff) is initialized (11, 100).
2025-01-25 20:59:49.075274  [info] Starting vsomeip application "World" (ffff) using 2 threads I/O nice 0
2025-01-25 20:59:49.076679  [info] main dispatch thread id from application: ffff (World) is: 7600438006c0 TID: 5073
2025-01-25 20:59:49.076928  [info] shutdown thread id from application: ffff (World) is: 760042e006c0 TID: 5074
2025-01-25 20:59:49.077288  [info] io thread id from application: ffff (World) is: 760044553b80 TID: 5072
2025-01-25 20:59:49.077844  [info] io thread id from application: ffff (World) is: 7600424006c0 TID: 5075
2025-01-25 20:59:49.078392  [info] assign_client: (ffff:World)
2025-01-25 20:59:49.080275  [info] create_local_server: Listening @ /tmp/vsomeip-101
2025-01-25 20:59:49.080448  [info] Client 0101 (World) successfully connected to routing  ~> registering..
2025-01-25 20:59:49.080505  [info] Registering to routing manager @ vsomeip-0
2025-01-25 20:59:49.082902  [info] Application/Client 0101 (World) is registered.
```

