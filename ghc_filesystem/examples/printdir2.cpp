/*  We start with the appropriate headers and then a function, printdir,
    which prints out the current directory.
    It will recurse for subdirectories, using the depth parameter is used for indentation.  */

#include <cstdlib>
#include <iostream>
#include <ghc/filesystem.hpp>

namespace fs = ghc::filesystem;

void printdir(fs::path dir_path, int depth)
{
    try {
        for (fs::directory_iterator it(dir_path); it != fs::directory_iterator(); ++it) {
            const fs::path &entry = it->path();

            if (fs::is_directory(entry)) {
                std::cout << std::string(depth, ' ') << entry.filename().c_str() << "/\n";
                printdir(entry, depth+4);
            } else {
                std::cout << std::string(depth, ' ') << entry.filename().c_str() << '\n';
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cout << "Filesystem error in directory: " << dir_path << e.what() << std::endl;
    }
}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
    fs::path topdir, pwd{"."};
    if (argc != 2)
        topdir=pwd;
    else
        topdir=argv[1];

    std::cout << "Directory scan of " << topdir << std::endl;
    printdir(topdir,0);
    std::cout << "done" << std::endl;

    exit(0);
}


