
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
                      

