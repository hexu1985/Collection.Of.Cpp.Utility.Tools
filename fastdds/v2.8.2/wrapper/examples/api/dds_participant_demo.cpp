#include "DDSParticipant.hpp"

using namespace fdds;

int main() {
    DDSParticipant& participant = DDSParticipant::getInstance();
    (void) participant;

    return 0;
}

