#include <iostream>
#include <variant>

enum class ErrorCode
{
    Ok,
    SystemError,
    IoError,
    NetworkError
};

std::variant<std::string, ErrorCode> FetchNameFromNetwork(int i)
{
    if (i == 0)
        return ErrorCode::SystemError;
        
    if (i == 1)
        return ErrorCode::NetworkError;
        
    return std::string("Hello World!");
}

int main() 
{
    auto response = FetchNameFromNetwork(0);
    if (std::holds_alternative<std::string>(response))
        std::cout << std::get<std::string>(response) << "n";
    else
        std::cout << "Error!\n";
        
    response = FetchNameFromNetwork(10);
    if (std::holds_alternative<std::string>(response))
        std::cout << std::get<std::string>(response) << "n";
    else
        std::cout << "Error!\n";
    
    return 0;
}
