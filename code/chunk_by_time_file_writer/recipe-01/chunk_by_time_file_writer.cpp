#include "chunk_by_time_file_writer.hpp"

#include <ctime>
#include <sstream>
#include <fstream>

namespace {

std::string TimeToString(time_t rawtime) {
    char buf[32] = {0};
    std::strftime(buf, sizeof(buf), "%Y%m%d-%H%M%S", localtime(&rawtime));
    return buf;
}

}   // namespace

class ChunkByTimeFileWriter::FileWriter {
private:
    std::ofstream outfile_;
    time_t file_open_time_ = 0;

public:
    FileWriter(const ChunkByTimeFileWriter::Config& config) {
        file_open_time_ = time(0);
        auto file_path = GenerateFilePath(config);
        outfile_.open(file_path);
        if (!outfile_) {
//            AERROR << "open file " << file_path << " failed!";
            throw std::runtime_error("open file failed!");
        }
//        AINFO << "new record file: " << file_path;
    }

    void Write(const char* s, size_t n) {
        outfile_.write(s, n);
        if (!outfile_) {
            throw std::runtime_error("write file failed!");
        }
    }

    ~FileWriter() {
        outfile_.flush();
        outfile_.close();
    }

    time_t FileOpenTime() const {
        return file_open_time_;
    }

    std::string GenerateFilePath(const ChunkByTimeFileWriter::Config& config) {
        std::ostringstream os;
        os << config.dir_path << '/' << config.file_prefix << '.' 
            << TimeToString(file_open_time_) << '.' << config.file_suffix;
        return os.str();
    }
};

ChunkByTimeFileWriter::ChunkByTimeFileWriter(const Config& config): config_(config) {
    const int min_file_split_interval_sec = 10;
    if (config_.file_split_interval_sec < min_file_split_interval_sec) {
        config_.file_split_interval_sec = min_file_split_interval_sec;
    }
}

void ChunkByTimeFileWriter::Open() {
    if (chunk_file_ != nullptr) {
        throw std::runtime_error("Open file failed: already opened!");
    }

    chunk_file_ = std::make_shared<FileWriter>(config_);
}

void ChunkByTimeFileWriter::Write(const char* s, size_t n) {
    if (config_.auto_split) {
        SplitFileIfNeed();
    }
    chunk_file_->Write(s, n);
}

bool ChunkByTimeFileWriter::SplitFileIfNeed() {
    time_t now = time(0);
    if (now - chunk_file_->FileOpenTime() >= config_.file_split_interval_sec) {
        chunk_file_ = std::make_shared<FileWriter>(config_);
        return true;
    }
    return false;
}

void ChunkByTimeFileWriter::Close() {
    chunk_file_.reset();
}
