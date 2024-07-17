#include <boost/interprocess/sync/named_mutex.hpp>
 
namespace bip = boost::interprocess;

int main(int argc, char* argv[]) {
    const char* name = "mtx";
    if (argc > 1) {
        name = argv[1];
    }
    
    bip::named_mutex::remove(name);
    return 0;
}
