#!/usr/bin/env bash

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
export VSOMEIP_CONFIGURATION=./vsomeip-tcp-service.json
./service-example
