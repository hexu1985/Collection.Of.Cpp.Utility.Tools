
2. StatelessReader::processDataMsg


```cpp
bool StatelessReader::processDataMsg(
        CacheChange_t* change)
{
    assert(change);

    std::unique_lock<RecursiveTimedMutex> lock(mp_mutex);

    if (acceptMsgFrom(change->writerGUID, change->kind))
    {
        // Always assert liveliness on scope exit
        auto assert_liveliness_lambda = [&lock, this, change](void*)
                {
                    lock.unlock(); // Avoid deadlock with LivelinessManager.
                    assert_writer_liveliness(change->writerGUID);
                };
        std::unique_ptr<void, decltype(assert_liveliness_lambda)> p{ this, assert_liveliness_lambda };

        logInfo(RTPS_MSG_IN, IDSTRING "Trying to add change " << change->sequenceNumber << " TO reader: " << m_guid);

        // Check rejection by history
        if (!thereIsUpperRecordOf(change->writerGUID, change->sequenceNumber))
        {
            bool will_never_be_accepted = false;
            if (!mp_history->can_change_be_added_nts(change->writerGUID, change->serializedPayload.length, 0,
                    will_never_be_accepted))
            {
                if (will_never_be_accepted)
                {
                    update_last_notified(change->writerGUID, change->sequenceNumber);
                }
                return false;
            }

            if (data_filter_ && !data_filter_->is_relevant(*change, m_guid))
            {
                update_last_notified(change->writerGUID, change->sequenceNumber);
                return true;
            }

            // Ask the pool for a cache change
            CacheChange_t* change_to_add = nullptr;
            if (!change_pool_->reserve_cache(change_to_add))
            {
                logWarning(RTPS_MSG_IN,
                        IDSTRING "Reached the maximum number of samples allowed by this reader's QoS. Rejecting change for reader: " <<
                        m_guid );
                return false;
            }

            // Copy metadata to reserved change
            change_to_add->copy_not_memcpy(change);

            // Ask payload pool to copy the payload
            IPayloadPool* payload_owner = change->payload_owner();

            bool is_datasharing = std::any_of(matched_writers_.begin(), matched_writers_.end(),
                            [&change](const RemoteWriterInfo_t& writer)
                            {
                                return (writer.guid == change->writerGUID) && (writer.is_datasharing);
                            });

            if (is_datasharing)
            {
                //We may receive the change from the listener (with owner a ReaderPool) or intraprocess (with owner a WriterPool)
                ReaderPool* datasharing_pool = dynamic_cast<ReaderPool*>(payload_owner);
                if (!datasharing_pool)
                {
                    datasharing_pool = datasharing_listener_->get_pool_for_writer(change->writerGUID).get();
                }
                if (!datasharing_pool)
                {
                    logWarning(RTPS_MSG_IN, IDSTRING "Problem copying DataSharing CacheChange from writer "
                            << change->writerGUID);
                    change_pool_->release_cache(change_to_add);
                    return false;
                }

                datasharing_pool->get_payload(change->serializedPayload, payload_owner, *change_to_add);
            }
            else if (payload_pool_->get_payload(change->serializedPayload, payload_owner, *change_to_add))
            {
                change->payload_owner(payload_owner);
            }
            else
            {
                logWarning(RTPS_MSG_IN, IDSTRING "Problem copying CacheChange, received data is: "
                        << change->serializedPayload.length << " bytes and max size in reader "
                        << m_guid << " is "
                        << (fixed_payload_size_ > 0 ? fixed_payload_size_ : std::numeric_limits<uint32_t>::max()));
                change_pool_->release_cache(change_to_add);
                return false;
            }

            // Perform reception of cache change
            if (!change_received(change_to_add))
            {
                logInfo(RTPS_MSG_IN, IDSTRING "MessageReceiver not add change " << change_to_add->sequenceNumber);
                change_to_add->payload_owner()->release_payload(*change_to_add);
                change_pool_->release_cache(change_to_add);
                return false;
            }
        }
    }

    return true;
}
```

---

2. ReaderHistory::add_change

```cpp
bool ReaderHistory::add_change(
        CacheChange_t* a_change)
{
    if (mp_reader == nullptr || mp_mutex == nullptr)
    {
        logError(RTPS_READER_HISTORY, "You need to create a Reader with this History before adding any changes");
        return false;
    }

    std::lock_guard<RecursiveTimedMutex> guard(*mp_mutex);
    if (m_att.memoryPolicy == PREALLOCATED_MEMORY_MODE && a_change->serializedPayload.length > m_att.payloadMaxSize)
    {
        logError(RTPS_READER_HISTORY,
                "Change payload size of '" << a_change->serializedPayload.length <<
                "' bytes is larger than the history payload size of '" << m_att.payloadMaxSize <<
                "' bytes and cannot be resized.");
        return false;
    }
    if (a_change->writerGUID == c_Guid_Unknown)
    {
        logError(RTPS_READER_HISTORY, "The Writer GUID_t must be defined");
    }

    eprosima::utilities::collections::sorted_vector_insert(m_changes, a_change, fastdds::rtps::history_order_cmp);
    logInfo(RTPS_READER_HISTORY,
            "Change " << a_change->sequenceNumber << " added with " << a_change->serializedPayload.length << " bytes");

    return true;
}
```