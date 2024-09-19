#include <iostream>
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct videoinfo {
    int width;
    int height;
    int frameRate;
    int crf;
};

void from_json(const json& j, videoinfo& v) {
    v.width = j["width"].get<int>();
    v.height = j["height"].get<int>();
    v.frameRate = j["frameRate"].get<int>();
    v.crf = j["crf"].get<int>();
}

struct pieceinfo {
    std::string  pathname;
    int     startTime;
    int     endTime;
};

void from_json(const json& j, pieceinfo &p) {
    p.pathname = j["file"].get<std::string>();
    p.startTime = j["startTime"].get<int>();
    p.endTime = j["endTime"].get<int>();
}

struct trackinfo {
    std::string      name;
    pieceinfo   pieces[10];
    int         size;
};

void from_json(const json& j, trackinfo &t) {
    t.name = j["name"].get<std::string>();
    for(int i = 0; i < j["pieces"].size(); i++) {
        from_json(j["pieces"][i], t.pieces[i]);
    }
    t.size = j["pieces"].size();
}

std::ostream& operator<<(std::ostream& out, const videoinfo& info) {
    out << "width: " << info.width << "\n"
        << "height: " << info.height << "\n"
        << "frameRate: " << info.frameRate << "\n"
        << "crf: " << info.crf << "\n";
    return out;
}

std::ostream& operator<<(std::ostream& out, const pieceinfo& info) {
    out << "pathname: " << info.pathname << "\n"
        << "startTime: " << info.startTime << "\n"
        << "endTime: " << info.endTime << "\n";
    return out;
}

std::ostream& operator<<(std::ostream& out, const trackinfo& info) {
    out << "name: " << info.name << "\n";
    for (int i = 0; i < info.size; i++) {
        out << "pieces[" << i << "]:\n";
        out << info.pieces[i] << "\n";
    }
    return out;
}

int main()
{
    json j;
    std::ifstream jfile("test.json");
    jfile >> j;
    videoinfo vi = j.at("output");
    int tilength = j["tracks"].size();
    trackinfo ti[tilength];
    for (int i = 0; i < tilength; i++) {
        ti[i] = j["tracks"][i];
    }
    std::cout << "videoinfo:\n" << vi << std::endl;
    for (int i = 0; i < tilength; i++) {
        std::cout << "trackinfo[" << i << "]:\n" << ti[i] << std::endl;
    }
    return 0;
}
