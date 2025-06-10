车载消息中间件FastDDS 源码解析（十）发送第一条PDP消息（上）
https://juejin.cn/post/7408851018501636150


1. UDP发送丢包


dvpn-lib-fastdds/src/cpp/rtps/transport/UDPTransportInterface.cpp
```cpp
bool UDPTransportInterface::send(
        const octet* send_buffer,
        uint32_t send_buffer_size,
        eProsimaUDPSocket& socket,
        const Locator& remote_locator,
        bool only_multicast_purpose,
        bool whitelisted,
        const std::chrono::microseconds& timeout)
{
    // ...
            if (!!ec)
            {
                if ((ec.value() == asio::error::would_block) ||
                        (ec.value() == asio::error::try_again))
                {
                    logWarning(RTPS_MSG_OUT, "UDP send would have blocked. Packet is dropped.");
                    return true;
                }
    //...
}
```

---

2. RTPSWriter::new_change

change的缓存：

change_pool_ 和 payload_pool_这儿有两个缓存池

dvpn-lib-fastdds/src/cpp/rtps/writer/RTPSWriter.cpp
```cpp
CacheChange_t* RTPSWriter::new_change(
        const std::function<uint32_t()>& dataCdrSerializedSize,
        ChangeKind_t changeKind,
        InstanceHandle_t handle)
{
    logInfo(RTPS_WRITER, "Creating new change");

    std::lock_guard<RecursiveTimedMutex> guard(mp_mutex);
    CacheChange_t* reserved_change = nullptr;
    if (!change_pool_->reserve_cache(reserved_change))
    {
        logWarning(RTPS_WRITER, "Problem reserving cache from pool");
        return nullptr;
    }

    uint32_t payload_size = fixed_payload_size_ ? fixed_payload_size_ : dataCdrSerializedSize();
    if (!payload_pool_->get_payload(payload_size, *reserved_change))
    {
        change_pool_->release_cache(reserved_change);
        logWarning(RTPS_WRITER, "Problem reserving payload from pool");
        return nullptr;
    }

    reserved_change->kind = changeKind;
    if (m_att.topicKind == WITH_KEY && !handle.isDefined())
    {
        logWarning(RTPS_WRITER, "Changes in KEYED Writers need a valid instanceHandle");
    }
    reserved_change->instanceHandle = handle;
    reserved_change->writerGUID = m_guid;
    reserved_change->writer_info.previous = nullptr;
    reserved_change->writer_info.next = nullptr;
    reserved_change->writer_info.num_sent_submessages = 0;
    return reserved_change;
}
```

3. WriterHistory::add_change_

dvpn-lib-fastdds/src/cpp/rtps/history/WriterHistory.cpp
```cpp
bool WriterHistory::add_change_(
        CacheChange_t* a_change,
        WriteParams& wparams,
        std::chrono::time_point<std::chrono::steady_clock> max_blocking_time)
{
    if (mp_writer == nullptr || mp_mutex == nullptr)
    {
        logError(RTPS_WRITER_HISTORY, "You need to create a Writer with this History before adding any changes");
        return false;
    }

    std::lock_guard<RecursiveTimedMutex> guard(*mp_mutex);
    if (!prepare_and_add_change(a_change, wparams))
    {
        return false;
    }

    notify_writer(a_change, max_blocking_time);

    return true;
}

bool WriterHistory::prepare_and_add_change(
        CacheChange_t* a_change,
        WriteParams& wparams)
{
    if (a_change->writerGUID != mp_writer->getGuid())
    {
        logError(RTPS_WRITER_HISTORY,
                "Change writerGUID " << a_change->writerGUID << " different than Writer GUID " <<
                mp_writer->getGuid());
        return false;
    }
    if ((m_att.memoryPolicy == PREALLOCATED_MEMORY_MODE) && a_change->serializedPayload.length > m_att.payloadMaxSize)
    {
        logError(RTPS_WRITER_HISTORY,
                "Change payload size of '" << a_change->serializedPayload.length <<
                "' bytes is larger than the history payload size of '" << m_att.payloadMaxSize <<
                "' bytes and cannot be resized.");
        return false;
    }

    if (m_isHistoryFull)
    {
        logWarning(RTPS_WRITER_HISTORY, "History full for writer " << a_change->writerGUID);
        return false;
    }

    ++m_lastCacheChangeSeqNum;
    a_change->sequenceNumber = m_lastCacheChangeSeqNum;
    if (wparams.source_timestamp().seconds() < 0)
    {
        Time_t::now(a_change->sourceTimestamp);
    }
    else
    {
        a_change->sourceTimestamp = wparams.source_timestamp();
    }
    a_change->writer_info.num_sent_submessages = 0;

    a_change->write_params = wparams;
    // Updated sample and related sample identities on the user's write params
    wparams.sample_identity().writer_guid(a_change->writerGUID);
    wparams.sample_identity().sequence_number(a_change->sequenceNumber);
    wparams.related_sample_identity(wparams.sample_identity());
    set_fragments(a_change);

    m_changes.push_back(a_change);

    if (static_cast<int32_t>(m_changes.size()) == m_att.maximumReservedCaches)
    {
        m_isHistoryFull = true;
    }

    logInfo(RTPS_WRITER_HISTORY,
            "Change " << a_change->sequenceNumber << " added with " << a_change->serializedPayload.length << " bytes");

    return true;
}

```

---

4. 流控

dvpn-lib-fastdds/src/cpp/rtps/flowcontrol/FlowControllerImpl.hpp

```cpp
    template<typename PubMode = PublishMode>
    typename std::enable_if<!std::is_same<FlowControllerPureSyncPublishMode, PubMode>::value, bool>::type
    enqueue_new_sample_impl(
            fastrtps::rtps::RTPSWriter* writer,
            fastrtps::rtps::CacheChange_t* change,
            const std::chrono::time_point<std::chrono::steady_clock>& /* TODO max_blocking_time*/)
    {
        assert(!change->writer_info.is_linked.load());
        // Sync delivery failed. Try to store for asynchronous delivery.
        std::unique_lock<std::mutex> lock(async_mode.changes_interested_mutex);
        sched.add_new_sample(writer, change);
        async_mode.cv.notify_one();

        return true;
    }

    /*! This function is used when PublishMode = FlowControllerPureSyncPublishMode.
     *  In this case there is no async mechanism.
     */
    template<typename PubMode = PublishMode>
    typename std::enable_if<std::is_same<FlowControllerPureSyncPublishMode, PubMode>::value, bool>::type
    constexpr enqueue_new_sample_impl(
            fastrtps::rtps::RTPSWriter*,
            fastrtps::rtps::CacheChange_t*,
            const std::chrono::time_point<std::chrono::steady_clock>&) const
    {
        // Do nothing. Return false.
        return false;
    }

```

--- 

5. DataWriterImpl


dvpn-lib-fastdds/src/cpp/fastdds/publisher/DataWriterImpl.cpp
```cpp

ReturnCode_t DataWriterImpl::perform_create_new_change(
        ChangeKind_t change_kind,
        void* data,
        WriteParams& wparams,
        const InstanceHandle_t& handle)
{
    // Block lowlevel writer
    auto max_blocking_time = steady_clock::now() +
            microseconds(::TimeConv::Time_t2MicroSecondsInt64(qos_.reliability().max_blocking_time));

#if HAVE_STRICT_REALTIME
    std::unique_lock<RecursiveTimedMutex> lock(writer_->getMutex(), std::defer_lock);
    if (!lock.try_lock_until(max_blocking_time))
    {
        return ReturnCode_t::RETCODE_TIMEOUT;
    }
#else
    std::unique_lock<RecursiveTimedMutex> lock(writer_->getMutex());
#endif // if HAVE_STRICT_REALTIME

    PayloadInfo_t payload;
    bool was_loaned = check_and_remove_loan(data, payload);
    if (!was_loaned)
    {
        if (!get_free_payload_from_pool(type_->getSerializedSizeProvider(data), payload))
        {
            return ReturnCode_t::RETCODE_OUT_OF_RESOURCES;
        }

        if ((ALIVE == change_kind) && !type_->serialize(data, &payload.payload))
        {
            logWarning(DATA_WRITER, "Data serialization returned false");
            return_payload_to_pool(payload);
            return ReturnCode_t::RETCODE_ERROR;
        }
    }
```

---

6. WriterHistory::prepare_and_add_change

```cpp
bool WriterHistory::prepare_and_add_change(
        CacheChange_t* a_change,
        WriteParams& wparams)
{
    if (a_change->writerGUID != mp_writer->getGuid())
    {
        logError(RTPS_WRITER_HISTORY,
                "Change writerGUID " << a_change->writerGUID << " different than Writer GUID " <<
                mp_writer->getGuid());
        return false;
    }
    if ((m_att.memoryPolicy == PREALLOCATED_MEMORY_MODE) && a_change->serializedPayload.length > m_att.payloadMaxSize)
    {
        logError(RTPS_WRITER_HISTORY,
                "Change payload size of '" << a_change->serializedPayload.length <<
                "' bytes is larger than the history payload size of '" << m_att.payloadMaxSize <<
                "' bytes and cannot be resized.");
        return false;
    }

    if (m_isHistoryFull)
    {
        logWarning(RTPS_WRITER_HISTORY, "History full for writer " << a_change->writerGUID);
        return false;
    }

    ++m_lastCacheChangeSeqNum;
    a_change->sequenceNumber = m_lastCacheChangeSeqNum;
    if (wparams.source_timestamp().seconds() < 0)
    {
        Time_t::now(a_change->sourceTimestamp);
    }
    else
    {
        a_change->sourceTimestamp = wparams.source_timestamp();
    }
    a_change->writer_info.num_sent_submessages = 0;

    a_change->write_params = wparams;
    // Updated sample and related sample identities on the user's write params
    wparams.sample_identity().writer_guid(a_change->writerGUID);
    wparams.sample_identity().sequence_number(a_change->sequenceNumber);
    wparams.related_sample_identity(wparams.sample_identity());
    set_fragments(a_change);

    m_changes.push_back(a_change);

    if (static_cast<int32_t>(m_changes.size()) == m_att.maximumReservedCaches)
    {
        m_isHistoryFull = true;
    }

    logInfo(RTPS_WRITER_HISTORY,
            "Change " << a_change->sequenceNumber << " added with " << a_change->serializedPayload.length << " bytes");

    return true;
}
```