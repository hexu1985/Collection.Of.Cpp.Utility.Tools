#!/usr/bin/env bash

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
echo ${current_file_dir}

export LD_LIBRARY_PATH=/home/hexu/local/vsomeip/lib/
./hello_world_service
