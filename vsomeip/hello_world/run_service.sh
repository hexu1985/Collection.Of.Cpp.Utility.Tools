#!/usr/bin/env bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
echo ${current_file_dir}

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
VSOMEIP_CONFIGURATION=${current_file_dir}/helloworld-local.json \
VSOMEIP_APPLICATION_NAME=hello_world_service \
./hello_world_service
