#!/usr/bin/env bash

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
VSOMEIP_CONFIGURATION=helloworld-local.json \
VSOMEIP_APPLICATION_NAME=hello_world_service \
./hello_world_service
