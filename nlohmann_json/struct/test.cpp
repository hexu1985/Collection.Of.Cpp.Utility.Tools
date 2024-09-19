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
    j.at("width").get_to(v.width);
    j.at("height").get_to(v.height);
    j.at("frameRate").get_to(v.frameRate);
    j.at("crf").get_to(v.crf);
}

struct pieceinfo {
    std::string  pathname;
    int     startTime;
    int     endTime;
};

void from_json(const json& j, pieceinfo &p) {
    j.at("file").get_to(p.pathname);
    j.at("startTime").get_to(p.startTime);
    j.at("endTime").get_to(p.endTime);
}

struct trackinfo {
    std::string      name;
    pieceinfo   pieces[10];
    int         size;
};

void from_json(const json& j, trackinfo &t) {
    j.at("name").get_to(t.name);
    for(int i = 0; i < j["pieces"].size(); i++) {
        t.pieces[i] = j["pieces"][i];
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
