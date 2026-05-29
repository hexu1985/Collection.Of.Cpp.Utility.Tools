
sudo apt install curl -y
curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /tmp/ros.key
sudo gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg /tmp/ros.key
echo "deb [signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://mirrors.ustc.edu.cn/ros/ubuntu focal main" | sudo tee /etc/apt/sources.list.d/ros-latest.list
sudo apt update -y

sudo apt install ros-noetic-desktop-full -y
sudo apt install python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential -y
