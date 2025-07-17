
#!/usr/bin/bash

cmake -S. -Bbuild -DFOONATHAN_MEMORY_BUILD_TESTS=OFF
cmake --build build
cmake --install build --prefix ~/fast_dds/install
