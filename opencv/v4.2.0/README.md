### Ubuntu20.04上源码编译

克隆代码

```bash
$ git clone https://gitee.com/opencv/opencv.git
$ cd opencv
$ git checkout 4.2.0

```bash
$ git clone https://gitee.com/opencv/opencv_contrib.git
$ cd opencv_contrib
$ git checkout 4.2.0
```

安装依赖
```bash
$ sudo apt update
$ sudo apt install -y build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
$ sudo apt install -y python3-dev python3-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
```

编译opencv
```bash
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DINSTALL_C_EXAMPLES=ON \
      -DINSTALL_PYTHON_EXAMPLES=ON \
      -DOPENCV_GENERATE_PKGCONFIG=ON \
      -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules \
      -DBUILD_EXAMPLES=ON
cmake --build build -j4
```
