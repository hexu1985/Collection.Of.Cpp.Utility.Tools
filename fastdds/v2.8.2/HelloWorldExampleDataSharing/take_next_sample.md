ReturnCode_t DataReader::take_next_sample(void* data, SampleInfo* info)
    - impl_->take_next_sample(data, info);
      ReturnCode_t DataReaderImpl::take_next_sample(void* data, SampleInfo* info)
        + read_or_take_next_sample(data, info, true);
          ReturnCode_t DataReaderImpl::read_or_take_next_sample(void* data, SampleInfo* info, bool should_take)
            - auto it = history_.lookup_available_instance(HANDLE_NIL, false);
              std::pair<bool, DataReaderHistory::instance_info> DataReaderHistory::lookup_available_instance(const InstanceHandle_t& handle, bool exact)
            - detail::ReadTakeCommand cmd(*this, data_values, sample_infos, 1, states, it.second, false, false);
              ReadTakeCommand::ReadTakeCommand(DataReaderImpl& reader, LoanableCollection& data_values, SampleInfoSeq& sample_infos, int32_t max_samples, const StateFilter& states, const history_type::instance_info& instance, bool single_instance, bool loop_for_data)
            - cmd.add_instance(should_take);
              bool add_instance(bool take_samples)  // ReadTakeCommand
                + reader_->begin_sample_access_nts(change, wp, is_future_change)
                  bool StatelessReader::begin_sample_access_nts(CacheChange_t* /*change*/, WriterProxy*& /*wp*/, bool& is_future_change)
                    - matched_writer_lookup(writer_guid, &wp)
                      bool StatefulReader::matched_writer_lookup(const GUID_t& writerGUID, WriterProxy** WP)
                        + bool returnedValue = findWriterProxy(writerGUID, WP);
                          bool StatefulReader::findWriterProxy(const GUID_t& writerGUID, WriterProxy** WP) const
                    - seq = wp->available_changes_max();
                      const SequenceNumber_t WriterProxy::available_changes_max() const
                + remove_change = !check_datasharing_validity(change, data_values_.has_ownership());
                  bool check_datasharing_validity(CacheChange_t* change, bool has_ownership)    // ReadTakeCommand
                + add_sample(*it, remove_change);
                  bool add_sample(const DataReaderCacheChange& item, bool& deserialization_error)   // ReadTakeCommand
                    - generate_info(item);
                      void generate_info(const DataReaderCacheChange& item) // ReadTakeCommand
                        + SampleInfo& info = sample_infos_[current_slot_];
                        + generate_info(info, *instance_->second, item);
                          static void generate_info(SampleInfo& info, const DataReaderInstance& instance, const DataReaderCacheChange& item)    // ReadTakeCommand
                    - deserialize_sample(item)
                      bool deserialize_sample(CacheChange_t* change)    // ReadTakeCommand
                        + type_->deserialize(payload, data_values_.buffer()[current_slot_])
                          bool HelloWorldPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                + history_.change_was_processed_nts(change, added);
                  void DataReaderHistory::change_was_processed_nts(CacheChange_t* const change, bool is_going_to_be_mark_as_read)
                + reader_->end_sample_access_nts(change, wp, added);
                  void StatelessReader::end_sample_access_nts(CacheChange_t* change, WriterProxy*& wp, bool mark_as_read)
                    - change_read_by_user(change, wp, mark_as_read);
                      void StatelessReader::change_read_by_user(CacheChange_t* change, WriterProxy* /*writer*/, bool mark_as_read)
                        + send_ack_if_datasharing(this, mp_history, writer, change->sequenceNumber);
                          static void send_ack_if_datasharing(StatefulReader* reader, ReaderHistory* history, WriterProxy* writer, const SequenceNumber_t& sequence_number)
                            - reader->send_acknack(writer, sns, writer, false);
                              void StatefulReader::send_acknack(const WriterProxy* writer, const SequenceNumberSet_t& sns, RTPSMessageSenderInterface* sender, bool is_final)
                                + RTPSMessageGroup group(getRTPSParticipant(), this, sender);
                                + group.add_acknack(sns, acknack_count_, is_final);
                + history_.remove_change_sub(change, it);
                  bool DataReaderHistory::remove_change_sub(CacheChange_t* change, DataReaderInstance::ChangeCollection::iterator& it)
                    - const_iterator chit = find_change_nts(change);
                      History::const_iterator History::find_change_nts(CacheChange_t* ch)
                    - ReaderHistory::remove_change_nts(chit);
                      History::iterator ReaderHistory::remove_change_nts(const_iterator removal, bool release)
                        + mp_reader->change_removed_by_history(change);
                          bool StatefulReader::change_removed_by_history(CacheChange_t* a_change, WriterProxy* wp)
                            - findWriterProxy(a_change->writerGUID, &proxy)
                              bool StatefulReader::findWriterProxy(const GUID_t& writerGUID, WriterProxy** WP) const
                            - send_ack_if_datasharing(this, mp_history, proxy, a_change->sequenceNumber);
                              static void send_ack_if_datasharing(StatefulReader* reader, ReaderHistory* history, WriterProxy* writer, const SequenceNumber_t& sequence_number)
                            - reader->send_acknack(writer, sns, writer, false);
                              void StatefulReader::send_acknack(const WriterProxy* writer, const SequenceNumberSet_t& sns, RTPSMessageSenderInterface* sender, bool is_final)
                                + RTPSMessageGroup group(getRTPSParticipant(), this, sender);
                                + group.add_acknack(sns, acknack_count_, is_final);
                        + mp_reader->releaseCache(change);
                          void RTPSReader::releaseCache(CacheChange_t* change)
                            - pool->release_payload(*change)
                              bool release_payload(CacheChange_t& cache_change) override    // ReaderPool
                                + DataSharingPayloadPool::release_payload(cache_change);
                            - change_pool_->release_cache(change);
                              bool CacheChangePool::release_cache(CacheChange_t* cache_change)
                                + return_cache_to_pool(cache_change);
                                  void CacheChangePool::return_cache_to_pool(CacheChange_t* ch)
                + history_.instance_viewed_nts(instance_->second);
                  void DataReaderHistory::instance_viewed_nts(const InstanceCollection::mapped_type& instance)
                + next_instance();
                  bool next_instance()  // ReadTakeCommand
                    - history_.check_and_remove_instance(instance_);
                      void DataReaderHistory::check_and_remove_instance(DataReaderHistory::instance_info& instance_info)
                      + instance_info = data_available_instances_.erase(instance_info);
                    - auto result = history_.next_available_instance_nts(handle_, instance_);
                      std::pair<bool, DataReaderHistory::instance_info> DataReaderHistory::next_available_instance_nts(const InstanceHandle_t& handle, const DataReaderHistory::instance_info& current_info)
            - try_notify_read_conditions();
              void DataReaderImpl::try_notify_read_conditions() noexcept

