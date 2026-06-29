cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/home/hexu/local \
    -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
    -DWITH_CUDA=ON \
    -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.8 \
    -DCUDA_ARCH_BIN="8.9" \
    -DENABLE_FAST_MATH=1 \
    -DWITH_CUBLAS=1 \
    -DBUILD_opencv_python3=ON
cmake --build build -j4
