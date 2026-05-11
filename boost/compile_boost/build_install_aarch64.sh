#./b2 install --with-filesystem --prefix=${HOME}/local/boost --layout=tagged toolset=gcc
#./b2 install --with-system --with-filesystem --with-chrono --with-iostreams --with-thread --with-log --prefix=${HOME}/local/boost toolset=gcc -j2
cp gcc-aarch64.jam project-config.jam
./b2 install --with-system --with-filesystem --prefix=${HOME}/local/boost-aarch64 toolset=gcc-aarch64 link=shared -j2

