#include <boost/thread/thread.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/ref.hpp>
 
#include <string>
#include <mutex>
 
//boost::shared_mutex global_mutex;
int global_num = 10;//全局变量，写者改变全局变量，读者读全局变量
namespace bip = boost::interprocess;
bip::named_mutex global_mutex(bip::open_or_create,"mtx");
 
//读线程
void read_thread(std::string &name){
    boost::lock_guard<bip::named_mutex> lock(global_mutex);//读锁定
//    bip::named_mutex global_mutex(bip::open_or_create,"mtx");
//    global_mutex.lock();
    printf("线程%s抢占了资源，global_num = %d\n",name.c_str(),global_num);
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    printf("线程%s释放了资源...\n",name.c_str());
//    global_mutex.unlock();
}
 
//写线程
void write_thread(std::string &name){
    std::lock_guard<bip::named_mutex> lock(global_mutex);//写锁定
//    bip::named_mutex global_mutex(bip::open_or_create,"mtx");
//    global_mutex.lock();
    global_num++;//写线程改变数据的数值
    printf("线程%s抢占了资源，global_num = %d\n",name.c_str(),global_num);
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    printf("线程%s释放了资源...\n",name.c_str());
//    global_mutex.unlock();
}
 
int main(){
    std::string read_thread_r1 = "read_thread_r1";
    std::string read_thread_r2 = "read_thread_r2";
    std::string read_thread_r3 = "read_thread_r3";
    std::string read_thread_r4 = "read_thread_r4";
    std::string read_thread_r5 = "read_thread_r5";
    std::string write_thread_w1 = "write_thread_w1";
    std::string write_thread_w2 = "write_thread_w2";
 
    boost::thread_group tg;
    tg.create_thread(boost::bind(read_thread,boost::ref(read_thread_r1)));
    tg.create_thread(boost::bind(read_thread,boost::ref(read_thread_r2)));
    tg.create_thread(boost::bind(read_thread,boost::ref(read_thread_r3)));
    tg.create_thread(boost::bind(read_thread,boost::ref(read_thread_r4)));
    tg.create_thread(boost::bind(read_thread,boost::ref(read_thread_r5)));
    tg.create_thread(boost::bind(write_thread,boost::ref(write_thread_w1)));
    tg.create_thread(boost::bind(write_thread,boost::ref(write_thread_w2)));
    tg.join_all();
 
    return 0;
}
