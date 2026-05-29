
### 📝 安装前置准备
确认系统版本：ROS Noetic 官方支持的 Ubuntu 版本是 20.04 (Focal Fossa)。

```bash
$ bash
$ lsb_release -a
```
如果输出的不是 Release: 20.04，请先安装正确的 Ubuntu 版本。

更新软件包列表：

```
$ bash
$ sudo apt update
$ sudo apt upgrade -y
```
这个步骤能确保你的系统是最新状态，避免后续安装出现问题。


### 🛠️ 安装步骤

1. 配置软件源

步骤 1：获取新公钥

```bash
# 下载并添加新的 GPG 密钥
$ curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /tmp/ros.key
$ sudo gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg /tmp/ros.key
```

步骤 2：添加软件源

```bash
# 添加新的软件源列表文件
$ echo "deb [signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://mirrors.ustc.edu.cn/ros/ubuntu focal main" | sudo tee /etc/apt/sources.list.d/ros-latest.list
你也可以将源地址 http://mirrors.ustc.edu.cn/ros/ubuntu 替换为其他国内镜像，如清华源 http://mirrors.tuna.tsinghua.edu.cn/ros/ubuntu 或阿里源 http://mirrors.aliyun.com/ros/ubuntu。
```

步骤 3：更新系统

```bash
# 更新软件包列表
$ sudo apt update
```

3. 安装 ROS Noetic
推荐安装 desktop-full 版本，它包含了 ROS、Rviz、Gazebo 等最常用的工具和功能包，非常适合新手。

```bash
$ sudo apt install ros-noetic-desktop-full
```

4. 设置环境变量
为了方便每次使用，可以将 ROS 的环境变量配置自动添加到终端配置文件 ~/.bashrc 中。

bash
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
source ~/.bashrc


