#include "EprosimaPubWrapper.hpp"

EprosimaPubWrapper::~EprosimaPubWrapper() {
    reset();
}

bool EprosimaPubWrapper::init(const Config& config) {
    reset();
    return true;
}

void EprosimaPubWrapper::reset() {
    if (mp_data_writer != nullptr)
    {
        mp_publisher->delete_datawriter(mp_data_writer);
    }
    if (mp_publisher != nullptr)
    {
        mp_participant->delete_publisher(mp_publisher);
    }
    if (mp_topic != nullptr)
    {
        mp_participant->delete_topic(mp_topic);
    }

    mp_data_writer = nullptr;
    mp_publisher = nullptr;
    mp_topic = nullptr;
    mp_type_support.reset();
    mp_participant = nullptr;
}
