#include "DDSUtility.hpp"

namespace fdds {

std::ostream& operator<< (std::ostream& out, const eprosima::fastdds::dds::DataWriterQos& qos)
{
    out << "=== DataWriterQos ===" << std::endl;
    
    // Durability QoS
    out << "durability.kind: ";
    switch (qos.durability().kind) {
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::VOLATILE_DURABILITY_QOS:
            out << "VOLATILE_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_LOCAL_DURABILITY_QOS:
            out << "TRANSIENT_LOCAL_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::TRANSIENT_DURABILITY_QOS:
            out << "TRANSIENT_DURABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DurabilityQosPolicyKind::PERSISTENT_DURABILITY_QOS:
            out << "PERSISTENT_DURABILITY_QOS" << std::endl; break;
    }

    // Durability Service QoS
    out << "durability_service.service_cleanup_delay: " 
              << qos.durability_service().service_cleanup_delay.seconds << "s "
              << qos.durability_service().service_cleanup_delay.nanosec << "ns" << std::endl;
    out << "durability_service.history_kind: ";
    switch (qos.durability_service().history_kind) {
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
            out << "KEEP_LAST_HISTORY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS:
            out << "KEEP_ALL_HISTORY_QOS" << std::endl; break;
    }
    out << "durability_service.history_depth: " << qos.durability_service().history_depth << std::endl;
    out << "durability_service.max_samples: " << qos.durability_service().max_samples << std::endl;
    out << "durability_service.max_instances: " << qos.durability_service().max_instances << std::endl;
    out << "durability_service.max_samples_per_instance: " << qos.durability_service().max_samples_per_instance << std::endl;

    // Deadline QoS
    out << "deadline.period: " << qos.deadline().period.seconds << "s " 
              << qos.deadline().period.nanosec << "ns" << std::endl;

    // Latency Budget QoS
    out << "latency_budget.duration: " << qos.latency_budget().duration.seconds << "s "
              << qos.latency_budget().duration.nanosec << "ns" << std::endl;

    // Liveliness QoS
    out << "liveliness.kind: ";
    switch (qos.liveliness().kind) {
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::AUTOMATIC_LIVELINESS_QOS:
            out << "AUTOMATIC_LIVELINESS_QOS" << std::endl; break;
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::MANUAL_BY_PARTICIPANT_LIVELINESS_QOS:
            out << "MANUAL_BY_PARTICIPANT_LIVELINESS_QOS" << std::endl; break;
        case eprosima::fastdds::dds::LivelinessQosPolicyKind::MANUAL_BY_TOPIC_LIVELINESS_QOS:
            out << "MANUAL_BY_TOPIC_LIVELINESS_QOS" << std::endl; break;
    }
    out << "liveliness.lease_duration: " << qos.liveliness().lease_duration.seconds << "s "
              << qos.liveliness().lease_duration.nanosec << "ns" << std::endl;
    out << "liveliness.announcement_period: " << qos.liveliness().announcement_period.seconds << "s "
              << qos.liveliness().announcement_period.nanosec << "ns" << std::endl;

    // Reliability QoS
    out << "reliability.kind: ";
    switch (qos.reliability().kind) {
        case eprosima::fastdds::dds::ReliabilityQosPolicyKind::BEST_EFFORT_RELIABILITY_QOS:
            out << "BEST_EFFORT_RELIABILITY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::ReliabilityQosPolicyKind::RELIABLE_RELIABILITY_QOS:
            out << "RELIABLE_RELIABILITY_QOS" << std::endl; break;
    }
    out << "reliability.max_blocking_time: " << qos.reliability().max_blocking_time.seconds << "s "
              << qos.reliability().max_blocking_time.nanosec << "ns" << std::endl;

    // Destination Order QoS
    out << "destination_order.kind: ";
    switch (qos.destination_order().kind) {
        case eprosima::fastdds::dds::DestinationOrderQosPolicyKind::BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS:
            out << "BY_RECEPTION_TIMESTAMP_DESTINATIONORDER_QOS" << std::endl; break;
        case eprosima::fastdds::dds::DestinationOrderQosPolicyKind::BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS:
            out << "BY_SOURCE_TIMESTAMP_DESTINATIONORDER_QOS" << std::endl; break;
    }

    // History QoS
    out << "history.kind: ";
    switch (qos.history().kind) {
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_LAST_HISTORY_QOS:
            out << "KEEP_LAST_HISTORY_QOS" << std::endl; break;
        case eprosima::fastdds::dds::HistoryQosPolicyKind::KEEP_ALL_HISTORY_QOS:
            out << "KEEP_ALL_HISTORY_QOS" << std::endl; break;
    }
    out << "history.depth: " << qos.history().depth << std::endl;

    // Resource Limits QoS
    out << "resource_limits.max_samples: " << qos.resource_limits().max_samples << std::endl;
    out << "resource_limits.max_instances: " << qos.resource_limits().max_instances << std::endl;
    out << "resource_limits.max_samples_per_instance: " << qos.resource_limits().max_samples_per_instance << std::endl;
    out << "resource_limits.allocated_samples: " << qos.resource_limits().allocated_samples << std::endl;

    // Transport Priority QoS
    out << "transport_priority.value: " << qos.transport_priority().value << std::endl;

    // Lifespan QoS
    out << "lifespan.duration: " << qos.lifespan().duration.seconds << "s "
              << qos.lifespan().duration.nanosec << "ns" << std::endl;

    // User Data QoS
    out << "user_data.size: " << qos.user_data().size() << std::endl;

    // Ownership QoS
    out << "ownership.kind: ";
    switch (qos.ownership().kind) {
        case eprosima::fastdds::dds::OwnershipQosPolicyKind::SHARED_OWNERSHIP_QOS:
            out << "SHARED_OWNERSHIP_QOS" << std::endl; break;
        case eprosima::fastdds::dds::OwnershipQosPolicyKind::EXCLUSIVE_OWNERSHIP_QOS:
            out << "EXCLUSIVE_OWNERSHIP_QOS" << std::endl; break;
    }

    // Ownership Strength QoS
    out << "ownership_strength.value: " << qos.ownership_strength().value << std::endl;

    // Writer Data Lifecycle QoS
    out << "writer_data_lifecycle.autodispose_unregistered_instances: " 
              << qos.writer_data_lifecycle().autodispose_unregistered_instances << std::endl;

    // Data Representation QoS
    out << "representation.m_value.size: " << qos.representation().m_value.size() << std::endl;
    for (const auto& rep : qos.representation().m_value) {
        out << "  - representation: ";
        switch (rep) {
            case eprosima::fastdds::dds::DataRepresentationId_t::XCDR_DATA_REPRESENTATION:
                out << "XCDR_DATA_REPRESENTATION" << std::endl; break;
            case eprosima::fastdds::dds::DataRepresentationId_t::XML_DATA_REPRESENTATION:
                out << "XML_DATA_REPRESENTATION" << std::endl; break;
            case eprosima::fastdds::dds::DataRepresentationId_t::XCDR2_DATA_REPRESENTATION:
                out << "XCDR2_DATA_REPRESENTATION" << std::endl; break;
        }
    }

    // Publish Mode QoS
    out << "publish_mode.kind: ";
    switch (qos.publish_mode().kind) {
        case eprosima::fastdds::dds::PublishModeQosPolicyKind::SYNCHRONOUS_PUBLISH_MODE:
            out << "SYNCHRONOUS_PUBLISH_MODE" << std::endl; break;
        case eprosima::fastdds::dds::PublishModeQosPolicyKind::ASYNCHRONOUS_PUBLISH_MODE:
            out << "ASYNCHRONOUS_PUBLISH_MODE" << std::endl; break;
    }

    // Property QoS
    out << "properties.properties.size: " << qos.properties().properties().size() << std::endl;
    for (const auto& prop : qos.properties().properties()) {
        out << "  - " << prop.name() << " = " << prop.value() << std::endl;
    }

    // Binary Property QoS
    out << "properties.binary_properties.size: " << qos.properties().binary_properties().size() << std::endl;

    out << "=================================" << std::endl;
    return out;
}

}   // namespace fdds
