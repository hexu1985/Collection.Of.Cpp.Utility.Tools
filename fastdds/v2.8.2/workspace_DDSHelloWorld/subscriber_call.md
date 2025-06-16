
void UDPChannelResource::perform_listen_operation(Locator input_locator)
    - auto& msg = message_buffer();
      inline fastrtps::rtps::CDRMessage_t& message_buffer() // ChannelResource
    - Receive(msg.buffer, msg.max_size, msg.length, remote_locator)
      bool UDPChannelResource::Receive(octet* receive_buffer, uint32_t receive_buffer_capacity, uint32_t& receive_buffer_size, Locator& remote_locator)
        + size_t bytes = socket()->receive_from(asio::buffer(receive_buffer, receive_buffer_capacity), senderEndpoint)
        + transport_->endpoint_to_locator(senderEndpoint, remote_locator) 
          void UDPv4Transport::endpoint_to_locator(ip::udp::endpoint& endpoint, Locator& locator)
    - message_receiver()->OnDataReceived(msg.buffer, msg.length, input_locator, remote_locator);
      void ReceiverResource::OnDataReceived(const octet* data, const uint32_t size, const Locator_t& localLocator, const Locator_t& remoteLocator)
        + rcv->processCDRMsg(remoteLocator, localLocator, &msg);
          void MessageReceiver::processCDRMsg(const Locator_t& source_locator, const Locator_t& reception_locator, CDRMessage_t* msg)
            - valid = proc_Submsg_Data(submessage, &submsgh);
              bool MessageReceiver::proc_Submsg_Data(CDRMessage_t* msg, SubmessageHeader_t* smh) const
                + process_data_message_function_(readerID, ch);
                  process_data_message_function_ = std::bind(&MessageReceiver::process_data_message_without_security, this, std::placeholders::_1, std::placeholders::_2);
                  void MessageReceiver::process_data_message_without_security(const EntityId_t& reader_id, CacheChange_t& change)
                    - findAllReaders(reader_id, process_message);
                      template<typename Functor>
                      void MessageReceiver::findAllReaders(const EntityId_t& readerID, const Functor& callback) const
                        + reader->processDataMsg(&change);
                          bool StatelessReader::processDataMsg(CacheChange_t* change)
                            - !mp_history->can_change_be_added_nts(change->writerGUID, change->serializedPayload.length, 0, will_never_be_accepted)
                              bool ReaderHistory::can_change_be_added_nts(const GUID_t& writer_guid, uint32_t total_payload_size, size_t unknown_missing_changes_up_to, bool& will_never_be_accepted) const
                            - !change_pool_->reserve_cache(change_to_add)
                              bool CacheChangePool::reserve_cache(CacheChange_t*& cache_change)
                            - change_to_add->copy_not_memcpy(change);
                              void copy_not_memcpy(const CacheChange_t* ch_ptr)     // CacheChange_t
                            - payload_pool_->get_payload(change->serializedPayload, payload_owner, *change_to_add);
                              bool get_payload(SerializedPayload_t& data, IPayloadPool*& data_owner, CacheChange_t& cache_change) override  // TopicPayloadPoolProxy
                                + inner_pool_->get_payload(data, data_owner, cache_change);
                                  bool TopicPayloadPool::get_payload(SerializedPayload_t& data, IPayloadPool*& data_owner, CacheChange_t& cache_change)
                            - change_received(change_to_add)
                              bool StatelessReader::change_received(CacheChange_t* change)
                                + mp_history->received_change(change, 0)
                                  bool ReaderHistory::received_change(CacheChange_t* change, size_t)
                                    - add_change(change);
                                      bool ReaderHistory::add_change(CacheChange_t* a_change)
                                        + eprosima::utilities::collections::sorted_vector_insert(m_changes, a_change, fastdds::rtps::history_order_cmp);
                                          template<typename CollectionType, typename ValueType, typename LessThanPredicate = std::less<ValueType>>
                                          void sorted_vector_insert(CollectionType& collection, const ValueType& item, const LessThanPredicate& pred = LessThanPredicate())
                                + listener->on_data_available(this, guid, seq, seq, notify_single);
                                  void DataReaderImpl::InnerDataReaderListener::on_data_available(RTPSReader* /*reader*/, const GUID_t& writer_guid, const SequenceNumber_t& first_sequence, const SequenceNumber_t& last_sequence, bool& should_notify_individual_changes)
                                    - listener->on_data_available(user_reader);
                                      void on_data_available(DataReader* reader) override   // DataReaderListener(SubListener)
                                        + reader->take_next_sample(&hello_, &info)
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
                                                        + add_sample(*it, remove_change);
                                                          bool add_sample(const DataReaderCacheChange& item, bool& deserialization_error)   // ReadTakeCommand
                                                            - deserialize_sample(item)
                                                              bool add_sample(const DataReaderCacheChange& item, bool& deserialization_error)   // ReadTakeCommand
                                                                + type_->deserialize(payload, data_values_.buffer()[current_slot_])
                                                                  bool HelloWorldPubSubType::deserialize(SerializedPayload_t* payload, void* data)
                                                        + history_.change_was_processed_nts(change, added);
                                                          void DataReaderHistory::change_was_processed_nts(CacheChange_t* const change, bool is_going_to_be_mark_as_read)
                                                        + reader_->end_sample_access_nts(change, wp, added);
                                                          void StatelessReader::end_sample_access_nts(CacheChange_t* change, WriterProxy*& wp, bool mark_as_read)
                                                            - change_read_by_user(change, wp, mark_as_read);
                                                              void StatelessReader::change_read_by_user(CacheChange_t* change, WriterProxy* /*writer*/, bool mark_as_read)
                                                        + history_.remove_change_sub(change, it);
                                                          bool DataReaderHistory::remove_change_sub(CacheChange_t* change, DataReaderInstance::ChangeCollection::iterator& it)
                                                            - const_iterator chit = find_change_nts(change);
                                                              History::const_iterator History::find_change_nts(CacheChange_t* ch)
                                                            - ReaderHistory::remove_change_nts(chit);
                                                              History::iterator ReaderHistory::remove_change_nts(const_iterator removal, bool release)
                                                        + history_.instance_viewed_nts(instance_->second);
                                                          void DataReaderHistory::instance_viewed_nts(const InstanceCollection::mapped_type& instance)
                                                        + next_instance();
                                                          bool next_instance()  // ReadTakeCommand
                                                            - history_.check_and_remove_instance(instance_);
                                                              void DataReaderHistory::check_and_remove_instance(DataReaderHistory::instance_info& instance_info)
                                                              + instance_info = data_available_instances_.erase(instance_info);
                                                            - auto result = history_.next_available_instance_nts(handle_, instance_);

