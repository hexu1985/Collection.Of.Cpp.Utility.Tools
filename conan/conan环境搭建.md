### 一．安装Conan2.x

Conan基于Python开发，需要Python 3.6及以上版本。Ubuntu20.04自带python3.8，因此需要安装pip依赖。

1.安装系统依赖

```shell
$ sudo apt update
$ sudo apt install python3-pip build-essential git cmake -y
```

2.安装Conan

```shell
$ pip3 install conan
```

3.检验Conan是否安装

```shell
$ conan --version
```

如果没有需加入PATH 

```shell
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrcsource ~/.bashrc
```

### 二．搭建 JFrog Artifactory

1.创建jfrog文件夹存放配置文件

```shell
mkdir -p ~/jfrog && cd jfrog
```

2.下载cpp版本的jfrog配置文件

```shell
$ wget -P ~/jfrog https://releases.jfrog.io/artifactory/bintray-artifactory/org/artifactory/cpp/ce/jfrog-artifactory-cpp-ce/7.63.12/jfrog-artifactory-cpp-ce-7.63.12-linux.tar.gz
$ tar xzvf jfrog-artifactory-cpp-ce-7.63.12-linux.tar.gz
```

或者前往官网(https://conan.io/downloads)下载最新版本的jfrog-artifactory-cpp-ce-x.xx.xx-linux.tar.gz。

3.启动服务

```shell
./artifactory-cpp-ce-7.63.12/app/bin/artifactoryctl start
```

### 三．初始化 Artifactory

1.访问本地页面

http://<你的服务器IP>:8081
账号为admin。密码为password

2.初始化设置

修改密码、设置basedURL `http://<你的服务器IP>:8081`、跳过代理Configure Default Proxy、仓库Create Repositories选择Conan即完成jfrog本地仓库的设置。

1234!Abc

Set Base URL
Set the Base URL that will be used as the custom URL for accessing the JFrog Platform. The Base URL you set, for example, https://yourdomain.com, will be used as the Base URL in redirect responses, and generated emails.
Select Base URL

http://localhost:8082

### 四．配置Conan客户端

1.创建默认profile

```shell
$ conan profile detect
```

它会根据本地的GCC版本自动生成相应的conan配置文件。

2.添加远程仓库

个人：conan remote add <jfrog-name> http://<服务器IP>:8081/artifactory/api/conan/conan-local

```shell
$ conan remote add artifactory http://127.0.0.1:8081/artifactory/api/conan/conan-local
```

公司：conan remote add <> https://devops jfrog.sany.com.cn/artifactory/api/conan/syzk-cpp-source

3.登陆仓库

```shell
$ conan remote login <对应添加仓库命令的名字> <用户名> -p <密码>
$ conan remote login artifactory admin -p 1234!Abc
```

### 五．测试hello

1.模板生成程序（生成一个使用 CMake 构建的 C++ 库项目）

```shell
$ conan new cmake_lib -d name=hello -d version=0.1
```

2.本地构建测试

```shell
$ conan create . --user=myteam --channel=stable --build=missing
```

把包标识为：hello/0.1@myteam/stable

3.上传包

```shell
$ conan upload hello/0.1@myteam/stable --all -r=my-local（名字）
$ conan upload hello/0.1@myteam/stable --all -r=artifactory
```
