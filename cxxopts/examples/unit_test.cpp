#include <assert.h>
#include <iostream>
#include <string>

#include "cxxopts.hpp"

using namespace std;

class Argv {
public:
    Argv(std::initializer_list<const char *> args)
        : m_argv(new const char *[args.size()]),
        m_argc(static_cast<int>(args.size())) {
            int i = 0;
            auto iter = args.begin();
            while (iter != args.end()) {
                auto len = strlen(*iter) + 1;
                auto ptr = std::unique_ptr<char[]>(new char[len]);

                strcpy(ptr.get(), *iter);
                m_args.push_back(std::move(ptr));
                m_argv.get()[i] = m_args.back().get();

                ++iter;
                ++i;
            }
        }

    const char **argv() const { return m_argv.get(); }

    int argc() const { return m_argc; }

private:
    std::vector<std::unique_ptr<char[]>> m_args{};
    std::unique_ptr<const char *[]> m_argv;
    int m_argc;
};

void test_(cxxopts::Options &options) {
    options.add_options()
        ("long", "a long option")
        ("s,short", "a short option")
        ("quick,brown", "An option with multiple long names and no short name")
        ("f,ox,jumped", "An option with multiple long names and a short name")
        ("over,z,lazy,dog", "An option with multiple long names and a short name, not listed first")
        ("value", "an option with a value", cxxopts::value<std::string>())
        ("a,av", "a short option with a value", cxxopts::value<std::string>())
        ("6,six", "a short number option")
        ("p, space", "an option with space between short and long")
        ("period.delimited", "an option with a period in the long name")
        ("nothing", "won't exist", cxxopts::value<std::string>())
        ;
    Argv argv({
            "tester",
            "--long",
            "-s",
            "--value",
            "value",
            "-a",
            "b",
            "-6",
            "-p",
            "--space",
            "--quick",
            "--ox",
            "-f",
            "--brown",
            "-z",
            "--over",
            "--dog",
            "--lazy",
            "--period.delimited",
            });
    auto **actual_argv = argv.argv();
    auto argc = argv.argc();
    auto result = options.parse(argc, actual_argv);
    assert(result.count("long") == 1);
    assert(result.count("s") == 1);
    assert(result.count("value") == 1);
    assert(result.count("a") == 1);
    assert(result["value"].as<std::string>() == "value");
    assert(result["a"].as<std::string>() == "b");
    assert(result.count("6") == 1);
    assert(result.count("p") == 2);
    assert(result.count("space") == 2);
    assert(result.count("quick") == 2);
    assert(result.count("f") == 2);
    assert(result.count("z") == 4);
    assert(result.count("period.delimited") == 1);

    auto& arguments = result.arguments();
    assert(arguments.size() == 16);
    assert(arguments[0].key() == "long");
    assert(arguments[0].value() == "true");
    assert(arguments[0].as<bool>() == true);

    assert(arguments[1].key() == "short");
    assert(arguments[2].key() == "value");
    assert(arguments[3].key() == "av");
}

int main(int argc, char **argv) {
    cxxopts::Options options("命令解析的标题", "这里写一下介绍");
    test_(options); //具体实现请看该函数
    return 0;

    // 添加一组解析参数
    options.add_options()("b,bar", "Param bar", cxxopts::value<std::string>())(
            "d,debug", "Enable debugging",
            cxxopts::value<bool>()->default_value("false"))(
            "f,foo", "Param foo",
            cxxopts::value<int>()->default_value("10"))("h,help", "Print usage");

    // 参加第二组解析参数
    options.add_options()(
            "c,cds", "cds test",
            cxxopts::value<std::string>()->default_value("cds test parameter"));

    // 是否准许未知参数
    // options.allow_unrecognised_options();

    // 解析参数
    auto result = options.parse(argc, argv);

    // 参数是否出现
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    // 获取参数值
    bool debug = result["debug"].as<bool>();
    std::string bar;
    if (result.count("bar")) {
        bar = result["bar"].as<std::string>();
        cout << "bar: " << bar << endl;
    }
    int foo = result["foo"].as<int>();
    cout << "foo: " << foo << endl;

    cout << result["c"].as<std::string>();

    return 0;
}
