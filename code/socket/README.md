### BSD socket API封装

项目依赖：
- fmtlib
- abseil
- gflags（示例代码依赖）

函数命名规则：
- 和C API重名的都改成首字母大写
- 其他函数都是小写加下划线方式

版本记录：
- [版本一](recipe-01)
    + 简单的TCP接口支持
- [版本二](recipe-02)
    + 新增简单的UDP接口支持
    + 新增SocketAddress类
