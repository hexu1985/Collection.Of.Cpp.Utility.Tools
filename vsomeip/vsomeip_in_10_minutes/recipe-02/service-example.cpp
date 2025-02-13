#include <vsomeip/vsomeip.hpp>
#include <iostream>

#define SAMPLE_SERVICE_ID 0x1234
#define SAMPLE_INSTANCE_ID 0x5678

std::shared_ptr< vsomeip::application > app;

void on_state_cbk(vsomeip::state_type_e _state)
{
    if(_state == vsomeip::state_type_e::ST_REGISTERED)
    {
        std::cout << "_state is vsomeip::state_type_e::ST_REGISTERED" << std::endl;
        // we are registered at the runtime and can offer our service
        app->offer_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    }
}

int main() {

    app = vsomeip::runtime::get()->create_application("World");
    app->init();
    app->register_state_handler(on_state_cbk);
    app->start();
}
