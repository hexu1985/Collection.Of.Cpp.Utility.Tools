#!/usr/bin/bash

function gen_filter_str {
    local array
	array=(`echo "$@"`)

    separator=":"

    # 临时修改IFS，然后使用"$*"展开数组
    oldIFS=$IFS
    IFS=$separator
    joined="${array[*]}"
    IFS=$oldIFS
    
    echo "$joined"
}

case_set_array=(
Discovery.*
)

exclude_cases=(
Discovery.ServerClientEnvironmentSetUp
Discovery.ServerClientEnvironmentSetUpDNS
TransportUDP/TransportUDP.UDPMaxInitialPeer_P0_4_P3/TransportUDPv6
TransportUDP/TransportUDP.UDPMaxInitialPeer_P0_4_P3/TransportUDPv4
TransportUDP/TransportUDP.UDPMaxInitialPeer_P0_4_P4/TransportUDPv6
TransportUDP/TransportUDP.UDPMaxInitialPeer_P0_4_P4/TransportUDPv4
TransportUDP/TransportUDP.UDPMaxInitialPeer_P5_4_P4/TransportUDPv6
TransportUDP/TransportUDP.UDPMaxInitialPeer_P5_4_P4/TransportUDPv4
TransportUDP/TransportUDP.UDPMaxInitialPeer_P5_6_P4/TransportUDPv6
TransportUDP/TransportUDP.UDPMaxInitialPeer_P5_6_P4/TransportUDPv4
)

exclude_filter_str=$(gen_filter_str "${exclude_cases[@]}")
echo "-$exclude_filter_str"

current_file_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
cd ${current_file_dir}

start_time=$(date +%s)

for case_set in ${case_set_array[@]}
do
    ./BlackboxTests_FastRTPS --gtest_filter="${case_set}:-${exclude_filter_str}"
    test_result=$?
    if [ ${test_result} -ne 0 ]
    then
        echo -e "\e[1;31m[ERROR]: ${case_set} test fail!\e[0m"
        exit 1
    fi
done

end_time=$(date +%s)
elapsed_time=$((end_time - start_time))

echo -e "\e[1;32m[SUCCESS]: all test pass!\e[0m"
echo "执行时间: ${elapsed_time}秒"
