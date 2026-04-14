
void DataSharingListener::run()
    - process_new_data();
      void DataSharingListener::process_new_data ()
        + pool->get_next_unread_payload(ch, last_sequence, last_payload);
          void get_next_unread_payload(CacheChange_t& cache_change, SequenceNumber_t& last_sequence_number, uint64_t until)     // ReaderPool
            - ensure_reading_reference_is_in_bounds()
              bool ensure_reading_reference_is_in_bounds()  // ReaderPool
            - segment_->get_address_from_offset(history_[static_cast<uint32_t>(next_payload_)]);
              void* get_address_from_offset(SharedSegment::Offset offset) const override    // SharedSegment
            - read_from_shared_history(cache_change, payload)
              bool read_from_shared_history(CacheChange_t& cache_change, PayloadNode* payload)  // ReaderPool
        + reader_->processDataMsg(&ch)
          bool StatefulReader::processDataMsg(CacheChange_t* change)
            - acceptMsgFrom(change->writerGUID, &pWP)
              bool StatefulReader::acceptMsgFrom(const GUID_t& writerId, WriterProxy** wp) const
            - pWP->change_was_received(change->sequenceNumber)
              bool WriterProxy::change_was_received(const SequenceNumber_t& seq_num) const
            - mp_history->can_change_be_added_nts(change->writerGUID, change->serializedPayload.length, unknown_missing_changes_up_to, will_never_be_accepted)
              bool DataReaderHistory::can_change_be_added_nts(const GUID_t& writer_guid, uint32_t total_payload_size, size_t unknown_missing_changes_up_to, bool& will_never_be_accepted) const
                + ReaderHistory::can_change_be_added_nts(writer_guid, total_payload_size, unknown_missing_changes_up_to, will_never_be_accepted);
                  bool ReaderHistory::can_change_be_added_nts(const GUID_t& writer_guid, uint32_t total_payload_size, size_t unknown_missing_changes_up_to, bool& will_never_be_accepted) const
            - change_pool_->reserve_cache(change_to_add)
              bool CacheChangePool::reserve_cache(CacheChange_t*& cache_change)
            - change_to_add->copy_not_memcpy(change);
              void copy_not_memcpy(const CacheChange_t* ch_ptr)     // CacheChange_t
                + setFragmentSize(ch_ptr->fragment_size_, false);
                  void setFragmentSize(uint16_t fragment_size, bool create_fragment_list = false)   // CacheChange_to
            - datasharing_pool->get_payload(change->serializedPayload, payload_owner, *change_to_add);
              bool get_payload(SerializedPayload_t& data, IPayloadPool*& data_owner, CacheChange_t& cache_change) override  // ReaderPool
            - change_received(change_to_add, pWP, unknown_missing_changes_up_to)
              bool StatefulReader::change_received(CacheChange_t* a_change, WriterProxy* prox, size_t unknown_missing_changes_up_to)
                + mp_history->received_change(a_change, unknown_missing_changes_up_to, rejection_reason)
                  bool DataReaderHistory::received_change(CacheChange_t* a_change, size_t unknown_missing_changes_up_to, SampleRejectedStatusKind& rejection_reason)
                    - ret_value =  receive_fn_(a_change, unknown_missing_changes_up_to, rejection_reason);
                      bool DataReaderHistory::received_change_keep_last(CacheChange_t* a_change, size_t, SampleRejectedStatusKind& rejection_reason)
                        + ret_value = add_received_change_with_key(a_change, *vit->second, rejection_reason);
                          bool DataReaderHistory::add_received_change_with_key(CacheChange_t* a_change, DataReaderInstance& instance, SampleRejectedStatusKind& rejection_reason)
                            - add_to_reader_history_if_not_full(a_change, rejection_reason);
                              bool DataReaderHistory::add_to_reader_history_if_not_full(CacheChange_t* a_change, SampleRejectedStatusKind& rejection_reason)
                                + add_change(a_change);
                                  bool ReaderHistory::add_change(CacheChange_t* a_change)
                                    - eprosima::utilities::collections::sorted_vector_insert(m_changes, a_change, fastdds::rtps::history_order_cmp);
                            - add_to_instance(a_change, instance);
                              void DataReaderHistory::add_to_instance(CacheChange_t* a_change, DataReaderInstance& instance)
                                + eprosima::utilities::collections::sorted_vector_insert(instance.cache_changes, item, rtps::history_order_cmp);
                                + data_available_instances_[a_change->instanceHandle] = instances_[a_change->instanceHandle];
                + NotifyChanges(prox);
                  void StatefulReader::NotifyChanges(WriterProxy* prox)
                    - listener->on_data_available(this, proxGUID, first_seq, max_seq, notify_individual)
                      void DataReaderImpl::InnerDataReaderListener::on_data_available(RTPSReader* /*reader*/, const GUID_t& writer_guid, const SequenceNumber_t& first_sequence, const SequenceNumber_t& last_sequence, bool& should_notify_individual_changes)
                        + data_reader_->on_data_available(writer_guid, first_sequence, last_sequence)
                          bool DataReaderImpl::on_data_available(const GUID_t& writer_guid, const SequenceNumber_t& first_sequence, const SequenceNumber_t& last_sequence)
                            - history_.get_change(seq, writer_guid, &change)
                              bool History::get_change(const SequenceNumber_t& seq, const GUID_t& guid, CacheChange_t** change) const
                                + get_change_nts(seq, guid, change, m_changes.cbegin());
                                  History::const_iterator History::get_change_nts(const SequenceNumber_t& seq, const GUID_t& guid, CacheChange_t** change, History::const_iterator hint) const
                            - ret_val |= on_new_cache_change_added(change);
                              bool DataReaderImpl::on_new_cache_change_added(const CacheChange_t* const change)
                    - listener->on_data_available(user_reader);
                      void HelloWorldSubscriber::SubListener::on_data_available(DataReader* reader)
        + pool->release_payload(ch);
          bool release_payload(CacheChange_t& cache_change) override // ReaderPool
        + pool->advance_to_next_payload();
          bool advance_to_next_payload()    // ReaderPool
