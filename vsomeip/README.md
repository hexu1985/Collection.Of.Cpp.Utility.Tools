### vsomeip库

克隆项目：
```
$ git clone https://github.com/GENIVI/vsomeip.git
# 或者
$ git clone https://gitee.com/mirrors/vsomeip.git
```

安装依赖：

```
$ sudo apt-get install asciidoc source-highlight doxygen graphviz
$ sudo apt-get install libboost-all-dev
```

编译vsomeip：

```
$ cd vsomeip
$ cmake -H. -Bbuild
$ cmake --build build --parallel 4
$ cmake --install build --prefix ~/local/vsomeip
```


参考资料：
- [vsomeip —— 10分钟快速了解 vsomeip](https://blog.csdn.net/Aliven888/article/details/123333466)
- [SOME/IP开源库Vsomeip分析](https://zhuanlan.zhihu.com/p/405534988)
- [学习vsomeip的git库](https://gitee.com/jayhou/vsomeip)
