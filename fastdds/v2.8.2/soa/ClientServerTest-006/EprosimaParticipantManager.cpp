#include "EprosimaParticipantManager.hpp"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

using namespace eprosima::fastdds::dds;

EprosimaParticipantManager::ParticipantPtr 
EprosimaParticipantManager::create_participant(DomainId_t domain_id,
    const DomainParticipantQos& qos, DomainParticipantListener* listen) {
    auto participant = DomainParticipantFactory::get_instance()->create_participant(domain_id, qos, listen, StatusMask::none());
    if (participant == nullptr) {
        return nullptr;
    }

    return participant;
}

void EprosimaParticipantManager::delete_participant(ParticipantPtr part) {
    DomainParticipantFactory::get_instance()->delete_participant(part);
}

void EprosimaParticipantManager::add_participant(const ParticipantKey& id, ParticipantPtr part) {
    std::lock_guard<std::mutex> lck{m_part_map_mtx};
    m_part_map[id] = part;
}

EprosimaParticipantManager::ParticipantPtr EprosimaParticipantManager::get_participant(const ParticipantKey& id) {
    std::lock_guard<std::mutex> lck{m_part_map_mtx};
    auto iter = m_part_map.find(id);
    if (iter == m_part_map.end()) {
        return nullptr;
    }

    return iter->second;
}

void EprosimaParticipantManager::remove_participant(const ParticipantKey& id) {
    std::lock_guard<std::mutex> lck{m_part_map_mtx};
    m_part_map.erase(id);
}

