#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

#include <fastdds/dds/domain/DomainParticipant.hpp>

struct ParticipantKey {
    eprosima::fastdds::dds::DomainId_t domain_id;
    std::string participant_name;

    friend bool operator ==(const ParticipantKey& lhs, const ParticipantKey& rhs) {
        return lhs.domain_id == rhs.domain_id && 
            lhs.participant_name == rhs.participant_name;
    }
};

namespace details {

template <typename T>
inline void hash_combine(std::size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

}   // namespace details

// 自定义哈希函数（必须为 std::unordered_map 提供）
struct ParticipantKeyHash {
    std::size_t operator()(const ParticipantKey& id) const {
        size_t seed = 0;
        details::hash_combine(seed, id.domain_id);
        details::hash_combine(seed, id.participant_name);
        return seed;
    }
};

class EprosimaParticipantManager {
public:
    using ParticipantPtr = eprosima::fastdds::dds::DomainParticipant*;
    static ParticipantPtr create_participant(
        eprosima::fastdds::dds::DomainId_t domain_id,
        const eprosima::fastdds::dds::DomainParticipantQos& qos,
        eprosima::fastdds::dds::DomainParticipantListener* listen=nullptr); 

    static void delete_participant(ParticipantPtr part);

    void add_participant(const ParticipantKey& id, ParticipantPtr part);
    ParticipantPtr get_participant(const ParticipantKey& id);
    void remove_participant(const ParticipantKey& id);

private:
    std::unordered_map<ParticipantKey, ParticipantPtr, ParticipantKeyHash> m_part_map;
    std::mutex m_part_map_mtx;
};
