
writer_->write(&hello_);
bool DataWriter::write(void* data)
    - return impl_->write(data);
      bool DataWriterImpl::write(void* data)
        + create_new_change(ALIVE, data);
          ReturnCode_t DataWriterImpl::create_new_change(ChangeKind_t changeKind, void* data)
            - create_new_change_with_params(changeKind, data, wparams);
              ReturnCode_t DataWriterImpl::create_new_change_with_params(ChangeKind_t changeKind, void* data, WriteParams& wparams)
                + check_new_change_preconditions(changeKind, data);
                + perform_create_new_change(changeKind, data, wparams, handle);
                  ReturnCode_t DataWriterImpl::perform_create_new_change(ChangeKind_t change_kind, void* data, WriteParams& wparams, const InstanceHandle_t& handle)
                    - check_and_remove_loan(data, payload);
                    - get_free_payload_from_pool(type_->getSerializedSizeProvider(data), payload)
                      template<typename SizeFunctor> 
                      bool get_free_payload_from_pool(const SizeFunctor& size_getter, PayloadInfo_t& payload) //DataWriterImpl
                        + payload_pool_->get_payload(size, change)
                          bool get_payload(uint32_t size, CacheChange_t& cache_change) override   // TopicPayloadPoolProxy
                            - inner_pool_->get_payload(size, cache_change);
                              bool get_payload(uint32_t /* size */, CacheChange_t& cache_change) override   // PreallocatedTopicPayloadPool
                                + do_get_payload(payload_size_, cache_change, false);
                                  bool TopicPayloadPool::do_get_payload(uint32_t size, CacheChange_t& cache_change, bool resizeable)
                        + payload.move_from_change(change);
                          void move_from_change(CacheChange_t& change)  // PayloadInfo_t
                    - type_->serialize(data, &payload.payload)
                      bool HelloWorldPubSubType::serialize(void* data, SerializedPayload_t* payload)
                    - CacheChange_t* ch = writer_->new_change(change_kind, handle);
                      CacheChange_t* RTPSWriter::new_change(ChangeKind_t changeKind, InstanceHandle_t handle)
                        + change_pool_->reserve_cache(reserved_change)
                          bool CacheChangePool::reserve_cache(CacheChange_t*& cache_change)
                    - payload.move_into_change(*ch);
                      void move_into_change(CacheChange_t& change)  // PayloadInfo_t
                    - added = history_.add_pub_change_with_commit_hook(ch, wparams, filter_hook, lock, max_blocking_time);
                      template<typename PreCommitHook>
                      bool add_pub_change_with_commit_hook(fastrtps::rtps::CacheChange_t* change, fastrtps::rtps::WriteParams& wparams, PreCommitHook pre_commit, std::unique_lock<fastrtps::RecursiveTimedMutex>& lock, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)   // DataWriterHistory
                        + prepare_change(change, lock, max_blocking_time);
                          bool DataWriterHistory::prepare_change(CacheChange_t* change, std::unique_lock<RecursiveTimedMutex>& lock, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)
                        + this->add_change_with_commit_hook(change, wparams, pre_commit, time_point)
                          template<typename PreCommitHook>
                          bool add_change_with_commit_hook(CacheChange_t* a_change, WriteParams& wparams, PreCommitHook pre_commit, std::chrono::time_point<std::chrono::steady_clock> max_blocking_time)   // WriterHistory
                            - prepare_and_add_change(a_change, wparams)
                              bool WriterHistory::prepare_and_add_change(CacheChange_t* a_change, WriteParams& wparams)
                                + set_fragments(a_change);
                                  bool WriterHistory::do_reserve_cache(CacheChange_t** change, uint32_t size)
                                + m_changes.push_back(a_change);
                            - notify_writer(a_change, max_blocking_time)
                              void WriterHistory::notify_writer(CacheChange_t* a_change, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)
                                + mp_writer->unsent_change_added_to_history(a_change, max_blocking_time);
                                  void StatefulWriter::unsent_change_added_to_history(CacheChange_t* change, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)
                                    - flow_controller_->add_new_sample(this, change, max_blocking_time)
                                      bool add_new_sample(fastrtps::rtps::RTPSWriter* writer, fastrtps::rtps::CacheChange_t* change, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time) override
                                        + add_new_sample_impl(writer, change, max_blocking_time);
                                          template<typename PubMode = PublishMode>
                                          typename std::enable_if<std::is_base_of<FlowControllerPureSyncPublishMode, PubMode>::value, bool>::type
                                          add_new_sample_impl(fastrtps::rtps::RTPSWriter* writer, fastrtps::rtps::CacheChange_t* change, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)   // FlowControllerImpl
                                            - fastrtps::rtps::LocatorSelectorSender& locator_selector = writer->get_general_locator_selector();
                                            - fastrtps::rtps::RTPSMessageGroup group(participant_, writer, &locator_selector);
                                            - writer->deliver_sample_nts(change, group, locator_selector, max_blocking_time)
                                              DeliveryRetCode StatefulWriter::deliver_sample_nts(CacheChange_t* cache_change, RTPSMessageGroup& group, LocatorSelectorSender& locator_selector, const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)

                                                + deliver_sample_to_intraprocesses(cache_change);
                                                + deliver_sample_to_datasharing(cache_change);
                                                + deliver_sample_to_network(cache_change, group, locator_selector, max_blocking_time);
                                                  DeliveryRetCode StatefulWriter::deliver_sample_to_network( CacheChange_t* change, RTPSMessageGroup& group, LocatorSelectorSender& locator_selector, // Object locked by FlowControllerImpl const std::chrono::time_point<std::chrono::steady_clock>& max_blocking_time)
                                                    - group.add_data(*change, inline_qos)
                                                      bool RTPSMessageGroup::add_data( const CacheChange_t& change, bool expectsInlineQos)

