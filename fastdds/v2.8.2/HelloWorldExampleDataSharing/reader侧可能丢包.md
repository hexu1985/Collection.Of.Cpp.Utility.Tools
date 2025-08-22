
Fast-DDS/src/cpp/rtps/DataSharing/ReaderPool.hpp:167
```cpp
   void get_next_unread_payload(
            CacheChange_t& cache_change,
            SequenceNumber_t& last_sequence_number,
            uint64_t until)
    {
        last_sequence_number = last_sn_;

        while (next_payload_ < until)
        {
            // First ensure we are not too far behind
            // This may move the next_payload_ past the until value
            if (!ensure_reading_reference_is_in_bounds() && next_payload_ >= until)
            {
                break;
            }

            // history_[next_payload_] contains the offset to the payload
            PayloadNode* payload = static_cast<PayloadNode*>(
                segment_->get_address_from_offset(history_[static_cast<uint32_t>(next_payload_)]));
            if (!read_from_shared_history(cache_change, payload))
            {
                // Overriden while retrieving. Discard and continue
                advance(next_payload_);
                logWarning(RTPS_READER, "Dirty data detected on datasharing writer " << writer());
                continue;
            }

            if (last_sn_ != c_SequenceNumber_Unknown && last_sn_ >= cache_change.sequenceNumber)
            {
                // Sequence number went backwards, it was most probably overriden.
                continue;
            }

            if (!ensure_reading_reference_is_in_bounds())
            {
                // We may have been taken over and read a payload that is too far forward. Discard and continue
                continue;
            }
```


Fast-DDS/src/cpp/rtps/DataSharing/ReaderPool.hpp:217
```cpp
    bool read_from_shared_history(
            CacheChange_t& cache_change,
            PayloadNode* payload)
    {
        // The sequence number can be unknown already, but we defer the check to the end
        cache_change.sequenceNumber = payload->sequence_number();

        cache_change.serializedPayload.data = payload->data();
        cache_change.serializedPayload.max_size = payload->data_length();
        cache_change.serializedPayload.length = payload->data_length();

        cache_change.kind = static_cast<ChangeKind_t>(payload->status());
        cache_change.writerGUID = payload->writer_GUID();
        cache_change.instanceHandle = payload->instance_handle();
        cache_change.sourceTimestamp = payload->source_timestamp();
        cache_change.write_params.sample_identity(payload->related_sample_identity());

        SequenceNumber_t check = payload->sequence_number();
        if (check == c_SequenceNumber_Unknown || check != cache_change.sequenceNumber)
        {
            // data override while processing
            return false;
        }

        cache_change.payload_owner(this);
        return true;
    }
```
