#!/usr/bin/bash

cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build
cmake --install build --prefix ~/fast_dds/install
