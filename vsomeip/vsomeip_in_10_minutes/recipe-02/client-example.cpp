#include <iomanip>
#include <iostream>

#include <vsomeip/vsomeip.hpp>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678

std::shared_ptr< vsomeip::application > app;

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "Service ["
        << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
        << "] is " << (_is_available ? "available." : "NOT available.")  << std::endl;
}

void on_state_cbk(vsomeip::state_type_e _state)
{
    if(_state == vsomeip::state_type_e::ST_REGISTERED)
    {
        std::cout << "_state is vsomeip::state_type_e::ST_REGISTERED" << std::endl;
        // we are registered at the runtime now we can request the service
        // and wait for the on_availability callback to be called
        app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    }
}

int main() {

    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();
    app->register_state_handler(on_state_cbk);
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, on_availability);
    app->start();
}
