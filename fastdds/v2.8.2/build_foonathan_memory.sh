
#!/usr/bin/bash

cmake -S. -Bbuild
cmake --build build
cmake --install build --prefix ~/fast_dds/install
