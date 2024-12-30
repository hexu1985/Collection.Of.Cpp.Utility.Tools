#include "HTTPRequest.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>

int main() {
    std::string server_url = "http://127.0.0.1:8000/";

    try
    {
        // you can pass http::InternetProtocol::V6 to Request to make an IPv6 request
        http::Request request{server_url};

        std::cout << "与服务器的连接正常！\n";

        // send a get request
        const auto testResponse = request.send("GET");
        std::cout << "testResponse: " << std::string{testResponse.body.begin(), testResponse.body.end()} << '\n'; // print the result
    }
    catch (const std::exception& e)
    {
        std::cerr << "无法连接到服务器！\n";
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

    try
    {
        std::string file_path = "./test/file.bin";
        std::ifstream file(file_path);
        if (!file) {
            std::cerr << "open file: " << file_path << " failed!\n";
            exit(1);
        }
        std::string file_data(std::istreambuf_iterator<char>{file}, {});
        http::Request request{server_url};
        const auto response = request.send("POST", file_data, {
                {"Content-Type", "text/plain"}
                });
        std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }

}

