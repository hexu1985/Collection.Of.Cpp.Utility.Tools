#include <thread>
#include <chrono>
#include <string>

#include "chunk_by_time_file_writer.hpp"

std::string create_data(int i) {
    return "data " + std::to_string(i) + "\n";
}

int main() {
    ChunkByTimeFileWriter::Config config;
    config.dir_path = ".";
    config.file_prefix = "the-chunk-file";
    config.file_suffix = "txt";
    config.file_split_interval_sec = 15;
    config.auto_split = true;

    ChunkByTimeFileWriter file_writer(config);
    file_writer.Open();
    for (int i = 0; i < 100; i++) {
        auto data = create_data(i);
        file_writer.Write(data.data(), data.size());
        if (i % 10 == 0) {
            file_writer.Flush();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    file_writer.Close();

    return 0;
}
