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

exclude_cases=(
Discovery.ServerClientEnvironmentSetUp
Discovery.ServerClientEnvironmentSetUpDNS
PubSubBasic.EnvFileWarningWrongFile
PubSubBasic.EnvFileWarningEmpty
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

