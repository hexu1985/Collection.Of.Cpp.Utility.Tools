#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <iostream>

// 打印 DataReaderQos 的所有属性
void printDataReaderQos(const eprosima::fastdds::dds::DataReaderQos& qos)
{
    std::cout << "=== DataReaderQos ===" << std::endl;
    
    // Durability QoS
    std::cout << "durability.kind: ";
    switch (qos.durability().kind) {
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS:
            std::cout << "VOLATILE_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS:
            std::cout << "TRANSIENT_LOCAL_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS:
            std::cout << "TRANSIENT_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::PERSISTENT_DURABILITY_QOS:
            std::cout << "PERSISTENT_DURABILITY_QOS" << std::endl; break;
    }

    // Deadline QoS
    std::cout << "deadline.period: " << qos.deadline().period.seconds << "s " 
              << qos.deadline().period.nanosec << "ns" << std::endl;

    // Latency Budget QoS
    std::cout << "latency_budget.duration: " << qos.latency_budget().duration.seconds << "s "
              << qos.latency_budget().duration.nanosec << "ns" << std::endl;

    // Liveliness QoS
    std::cout << "liveliness.kind: ";
    switch (qos.liveliness().kind) {
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS:
            std::cout << "AUTOMATIC_LIVELINESS_QOS" << std::endl; break;
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS:
            std::cout << "MANUAL_BY_PARTICIPANT_LIVELINESS_QOS" << std::endl; break;
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::MANUAL_BY_TOPIC_LIVELINESS_QOS:
            std::cout << "MANUAL_BY_TOPIC_LIVELINESS_QOS" << std::endl; break;
    }
    std::cout << "liveliness.lease_duration: " << qos.liveliness().lease_duration.seconds << "s "
              << qos.liveliness().lease_duration.nanosec << "ns" << std::endl;
    std::cout << "liveliness.announcement_period: " << qos.liveliness().announcement_period.seconds << "s "
              << qos.liveliness().announcement_period.nanosec << "ns" << std::endl;

    // Reliability QoS
    std::cout << "reliability.kind: ";
    switch (qos.reliability().kind) {
        case eprosima::fastdds::dds::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS:
            std::cout << "BEST_EFFORT_RELIABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS:
            std::cout << "RELIABLE_RELIABILITY_QOS" << std::endl; break;
    }
    std::cout << "reliability.max_blocking_time: " << qos.reliability().max_blocking_time.seconds << "s "
              << qos.reliability().max_blocking_time.nanosec << "ns" << std::endl;

    // Destination Order QoS
    std::cout << "destination_order.kind: ";
    switch (qos.destination_order().kind) {
        case eprosima::fastdds::dds::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS:
            std::cout << "BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DestinationOrderQosPolicyKind::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS:
            std::cout << "BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS" << std::endl; break;
    }

    // History QoS
    std::cout << "history.kind: ";
    switch (qos.history().kind) {
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
            std::cout << "KEEP_LAST_HISTORY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS:
            std::cout << "KEEP_ALL_HISTORY_QOS" << std::endl; break;
    }
    std::cout << "history.depth: " << qos.history().depth << std::endl;

    // Resource Limits QoS
    std::cout << "resource_limits.max_samples: " << qos.resource_limits().max_samples << std::endl;
    std::cout << "resource_limits.max_instances: " << qos.resource_limits().max_instances << std::endl;
    std::cout << "resource_limits.max_samples_per_instance: " << qos.resource_limits().max_samples_per_instance << std::endl;
    std::cout << "resource_limits.allocated_samples: " << qos.resource_limits().allocated_samples << std::endl;

    // User Data QoS
    std::cout << "user_data.size: " << qos.user_data().size() << std::endl;

    // Ownership QoS
    std::cout << "ownership.kind: ";
    switch (qos.ownership().kind) {
        case eprosima::fastdds::dds::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS:
            std::cout << "SHARED_OWNERSHIP_QOS" << std::endl; break;
        case eprosima::fastdds::dds::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS:
            std::cout << "EXCLUSIVE_OWNERSHIP_QOS" << std::endl; break;
    }

    // Time Based Filter QoS
    std::cout << "time_based_filter.minimum_separation: " << qos.time_based_filter().minimum_separation.seconds << "s "
              << qos.time_based_filter().minimum_separation.nanosec << "ns" << std::endl;

    // Reader Data Lifecycle QoS
    std::cout << "reader_data_lifecycle.autopurge_no_writer_samples_delay: " 
              << qos.reader_data_lifecycle().autopurge_no_writer_samples_delay.seconds << "s "
              << qos.reader_data_lifecycle().autopurge_no_writer_samples_delay.nanosec << "ns" << std::endl;
    std::cout << "reader_data_lifecycle.autopurge_disposed_samples_delay: " 
              << qos.reader_data_lifecycle().autopurge_disposed_samples_delay.seconds << "s "
              << qos.reader_data_lifecycle().autopurge_disposed_samples_delay.nanosec << "ns" << std::endl;

    // Durability Service QoS
    std::cout << "durability_service.service_cleanup_delay: " 
              << qos.durability_service().service_cleanup_delay.seconds << "s "
              << qos.durability_service().service_cleanup_delay.nanosec << "ns" << std::endl;
    std::cout << "durability_service.history_kind: ";
    switch (qos.durability_service().history_kind) {
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
            std::cout << "KEEP_LAST_HISTORY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS:
            std::cout << "KEEP_ALL_HISTORY_QOS" << std::endl; break;
    }
    std::cout << "durability_service.history_depth: " << qos.durability_service().history_depth << std::endl;
    std::cout << "durability_service.max_samples: " << qos.durability_service().max_samples << std::endl;
    std::cout << "durability_service.max_instances: " << qos.durability_service().max_instances << std::endl;
    std::cout << "durability_service.max_samples_per_instance: " << qos.durability_service().max_samples_per_instance << std::endl;

    // Lifespan QoS
    std::cout << "lifespan.duration: " << qos.lifespan().duration.seconds << "s "
              << qos.lifespan().duration.nanosec << "ns" << std::endl;

    // Data Representation QoS
    std::cout << "representation.m_value.size: " << qos.type_consistency().representation.m_value.size() << std::endl;
    for (const auto& rep : qos.type_consistency().representation.m_value) {
        std::cout << "  - representation: ";
        switch (rep) {
            case eprosima::fastdds::dds::DataRepresentationId_t::XCDR_DATA_REPRESENTATION:
                std::cout << "XCDR_DATA_REPRESENTATION" << std::endl; break;
            case eprosima::fastdds::dds::DataRepresentationId_t::XML_DATA_REPRESENTATION:
                std::cout << "XML_DATA_REPRESENTATION" << std::endl; break;
            case eprosima::fastdds::dds::DataRepresentationId_t::XCDR2_DATA_REPRESENTATION:
                std::cout << "XCDR2_DATA_REPRESENTATION" << std::endl; break;
        }
    }

    // Type Consistency Enforcement QoS
    std::cout << "type_consistency.kind: ";
    switch (qos.type_consistency().type_consistency.m_kind) {
        case eprosima::fastdds::dds::TypeConsistencyKind::DISALLOW_TYPE_COERCION:
            std::cout << "DISALLOW_TYPE_COERCION" << std::endl; break;
        case eprosima::fastdds::dds::TypeConsistencyKind::ALLOW_TYPE_COERCION:
            std::cout << "ALLOW_TYPE_COERCION" << std::endl; break;
    }
    std::cout << "type_consistency.ignore_sequence_bounds: " << qos.type_consistency().type_consistency.m_ignore_sequence_bounds << std::endl;
    std::cout << "type_consistency.ignore_string_bounds: " << qos.type_consistency().type_consistency.m_ignore_string_bounds << std::endl;
    std::cout << "type_consistency.ignore_member_names: " << qos.type_consistency().type_consistency.m_ignore_member_names << std::endl;
    std::cout << "type_consistency.prevent_type_widening: " << qos.type_consistency().type_consistency.m_prevent_type_widening << std::endl;
    std::cout << "type_consistency.force_type_validation: " << qos.type_consistency().type_consistency.m_force_type_validation << std::endl;

    std::cout << "=================================" << std::endl;
}
