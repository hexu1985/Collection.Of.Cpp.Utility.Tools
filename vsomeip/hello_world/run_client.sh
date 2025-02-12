#!/usr/bin/env bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
echo ${current_file_dir}

sudo route add -host 224.224.224.245 dev enp3s0

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
VSOMEIP_CONFIGURATION=${current_file_dir}/helloworld-client.json \
VSOMEIP_APPLICATION_NAME=hello_world_client \
./hello_world_client
