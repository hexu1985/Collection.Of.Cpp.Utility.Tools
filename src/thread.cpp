#include "thread.hpp"

#include <cassert>

using namespace std;

namespace MiniUtils {

// ThreadInterrupt 
struct ThreadInterrupt {
};

void this_thread_exit()
{
    throw ThreadInterrupt();
}

// Thread
Thread::Thread(const string &name): name_(name) 
{
}

Thread::~Thread() 
{
    if (is_run()) {
        stop();
    }
}

void Thread::start() 
{
    assert(!thread_ && !task_queue_);
    task_queue_ = make_shared<TaskQueue>();
    thread_ = make_shared<thread>(&Thread::task_process, this);
}

void Thread::stop() 
{
    task_queue_->push_task(&this_thread_exit);
    thread_->join();
    thread_.reset();
    task_queue_.reset();
}

bool Thread::is_run() 
{
    return (thread_ ? true : false);
}

shared_ptr<TaskQueue> Thread::get_task_queue() 
{
    return task_queue_;
}

const string &Thread::get_name() const
{
    return name_;
}

void Thread::task_process()
{
    TaskQueue &incoming_queue = *task_queue_;
	while (true) {
        TaskPtrList working_list;
		incoming_queue.pop_task(working_list);
		while (!working_list.empty()) {
			auto task = working_list.front();
			working_list.pop_front();
            try {
                task->run();
            } catch (ThreadInterrupt) {
                return;
            }
		}
	}
}

}   // namespace MiniUtils

