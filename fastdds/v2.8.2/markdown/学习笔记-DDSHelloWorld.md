

### DDSHelloWorldPublisher

Fast-DDS/src/cpp/rtps/history/TopicPayloadPoolRegistry_impl/TopicPayloadPoolProxy.hpp
Fast-DDS/src/cpp/rtps/history/TopicPayloadPool.cpp


- SharedMemWatchdog::run
Fast-DDS/src/cpp/utils/shared_memory/SharedMemWatchdog.hpp

```cpp
    SharedMemWatchdog()
        : wake_run_(false)
        , exit_thread_(false)
    {
        thread_run_ = std::thread(&SharedMemWatchdog::run, this);
    }
```

- ResourceEvent::event_service
Fast-DDS/src/cpp/rtps/resources/ResourceEvent.cpp

```cpp
void ResourceEvent::init_thread()
{
    std::lock_guard<TimedMutex> lock(mutex_);

    allow_vector_manipulation_ = false;
    stop_.store(false);
    resize_collections();

    thread_ = std::thread(&ResourceEvent::event_service, this);
}
```

- UDPChannelResource::perform_listen_operation  (多个线程)
Fast-DDS/src/cpp/rtps/transport/UDPChannelResource.cpp

```cpp
UDPChannelResource::UDPChannelResource(
        UDPTransportInterface* transport,
        eProsimaUDPSocket& socket,
        uint32_t maxMsgSize,
        const Locator& locator,
        const std::string& sInterface,
        TransportReceiverInterface* receiver)
    : ChannelResource(maxMsgSize)
    , message_receiver_(receiver)
    , socket_(moveSocket(socket))
    , only_multicast_purpose_(false)
    , interface_(sInterface)
    , transport_(transport)
{
    thread(std::thread(&UDPChannelResource::perform_listen_operation, this, locator));
}
```

- SharedMemChannelResource::perform_listen_operation (多个线程)
Fast-DDS/src/cpp/rtps/transport/shared_mem/SharedMemChannelResource.hpp

```cpp
    void init_thread(
            const Locator& locator)
    {
        this->thread(std::thread(&SharedMemChannelResource::perform_listen_operation, this, locator));
    }
```

- FlowControllerImpl::run 
Fast-DDS/src/cpp/rtps/flowcontrol/FlowControllerImpl.hpp

```cpp
    template<typename PubMode = PublishMode>
    typename std::enable_if<!std::is_same<FlowControllerPureSyncPublishMode, PubMode>::value, void>::type
    initialize_async_thread()
    {
        bool expected = false;
        if (async_mode.running.compare_exchange_strong(expected, true))
        {
            // Code for initializing the asynchronous thread.
            async_mode.thread = std::thread(&FlowControllerImpl::run, this);
        }
    }
```

### locator类型

dvpn-lib-fastdds/include/fastdds/rtps/common/Locator.h
```cpp
/// Reserved locator kind
#define LOCATOR_KIND_RESERVED 0
/// UDP over IPv4 locator kind
#define LOCATOR_KIND_UDPv4 1
/// UDP over IPv6 locator kind
#define LOCATOR_KIND_UDPv6 2
/// TCP over IPv4 kind
#define LOCATOR_KIND_TCPv4 4
/// TCP over IPv6 locator kind
#define LOCATOR_KIND_TCPv6 8
/// Shared memory locator kind
#define LOCATOR_KIND_SHM 16
```
