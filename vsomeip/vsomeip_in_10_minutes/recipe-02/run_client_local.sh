#!/usr/bin/env bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
echo ${current_file_dir}

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
VSOMEIP_CONFIGURATION=${current_file_dir}/vsomeip-local.json \
./client-example
