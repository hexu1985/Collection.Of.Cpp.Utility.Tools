#!/usr/bin/bash

mkdir -p gen
fastddsgen soa_on_dds_types.idl -d gen
fastddsgen ClientServerTypes.idl -d gen

