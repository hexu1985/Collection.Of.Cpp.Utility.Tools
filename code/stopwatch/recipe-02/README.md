### timer类, 一个带打印功能的计时器类

timer类用来测量程序中部分代码的执行时间并分析代码。
这个类的工作方式非常像一块机械秒表，记录的是cpu time。
初始化秒表或是调用它的 start() 成员函数后，秒表将开始计时；
调用秒表的 stop() 成员函数，然后通过time获取流逝时间ms。

### 参考
数据结构C++语言描述：应用标准模板库 第3章: <https://github.com/hexu1985/Data.Structures.With.Cpp.Using.STL>
