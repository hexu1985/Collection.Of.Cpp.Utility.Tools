### 按时间分块的文件写入器

支持Open、Write、SplitFileIfNeed、Close，Flush接口

config定义：

```cpp
struct Config {
    std::string dir_path;
    std::string file_prefix;
    std::string file_suffix;
    int file_split_interval_sec=60;
    bool auto_split=true;
};
```

支持自动或手动检查文件分块。

