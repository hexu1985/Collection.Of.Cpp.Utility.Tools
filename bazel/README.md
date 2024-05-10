### Ubuntu上安装bazel

使用二进制安装程序（apt方式由于墙的问题，坑很大）

通常，您应该使用 apt 代码库，但如果您对机器没有管理员权限或无法添加自定义代码库，则二进制安装程序会很有用。

您可以从 [Bazel 的 GitHub 版本页面](https://github.com/bazelbuild/bazel/releases)下载二进制安装程序。

安装程序包含 Bazel 二进制文件，并将其解压缩到 $HOME/bin 文件夹中。您需要手动安装一些其他库，Bazel 才能正常运行。

**第 1 步：安装所需的软件包**

Bazel 需要一个 C++ 编译器和 unzip / zip 文件才能正常运行：

```
$ sudo apt install g++ unzip zip
```

如果要使用 Bazel 构建 Java 代码，请安装 JDK：

```
$ sudo apt-get install default-jdk
```

**第 2 步：运行安装程序**

接下来，从 [GitHub 上的 Bazel 版本页面](https://github.com/bazelbuild/bazel/releases)下载名为 
bazel-version-installer-linux-x86_64.sh 的 Bazel 二进制安装程序。

运行方式如下：

```
$ chmod +x bazel-version-installer-linux-x86_64.sh
$ ./bazel-version-installer-linux-x86_64.sh --user
```

--user 标志将 Bazel 安装到系统上的 $HOME/bin 目录中，并将 .bazelrc 路径设置为 $HOME/.bazelrc。使用 --help 命令可查看其他安装选项。

第 3 步：设置您的环境
如果您按上述方式使用 --user 标志运行 Bazel 安装程序，则 Bazel 可执行文件会安装到 $HOME/bin 目录中。
建议您将此目录添加到默认路径，如下所示：

```
$ export PATH="$PATH:$HOME/bin"
```

您也可以将此命令添加到 ~/.bashrc 或 ~/.zshrc 文件使其永久保留。


https://github.com/bazelbuild/examples
