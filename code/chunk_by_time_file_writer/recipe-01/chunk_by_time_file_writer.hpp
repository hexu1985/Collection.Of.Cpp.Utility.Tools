#pragma once

#include <memory>
#include <string>

class ChunkByTimeFileWriter {
private:
    class FileWriter;

public:
    struct Config {
        std::string dir_path;
        std::string file_prefix;
        std::string file_suffix;
        int file_split_interval_sec=60;
        bool auto_split=true;
    };

    ChunkByTimeFileWriter(const Config& config);

    void Open();

    void Write(const char* s, size_t n);

    bool SplitFileIfNeed();

    void Close();

private:
    Config config_;
    std::shared_ptr<FileWriter> chunk_file_;
};

