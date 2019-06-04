#include "worker_thread.hpp"

#include <cassert>

using namespace std;

namespace MiniUtils {

thread_local shared_ptr<TaskQueue> current_thread_task_queue;
thread_local string current_thread_name;

// WorkerThreadInterrupt 
struct WorkerThreadInterrupt {
};

void this_thread_exit()
{
    throw WorkerThreadInterrupt();
}

// WorkerThread
WorkerThread::WorkerThread(const string &name): name_(name) 
{
}

WorkerThread::~WorkerThread() 
{
    if (is_run()) {
        stop();
    }
}

void WorkerThread::start() 
{
    assert(!thread_ && !task_queue_);
    task_queue_ = make_shared<TaskQueue>();
    thread_ = make_shared<thread>(&WorkerThread::task_process, this);
}

void WorkerThread::stop() 
{
    task_queue_->push_task(&this_thread_exit);
    thread_->join();
    thread_.reset();
    task_queue_.reset();
}

bool WorkerThread::is_run() 
{
    return (thread_ ? true : false);
}

shared_ptr<TaskQueue> WorkerThread::get_task_queue() 
{
    return task_queue_;
}

const string &WorkerThread::get_name() const
{
    return name_;
}

void WorkerThread::task_process()
{
    current_thread_task_queue = this->task_queue_;
    current_thread_name = this->name_;

    TaskQueue &incoming_queue = *task_queue_;
	while (true) {
        TaskPtrList working_list;
		incoming_queue.pop_task(working_list);
		while (!working_list.empty()) {
			auto task = working_list.front();
			working_list.pop_front();
            try {
                task->run();
            } catch (WorkerThreadInterrupt) {
                return;
            }
		}
	}
}

namespace current_worker_thread {

std::shared_ptr<TaskQueue> get_task_queue()
{
    return current_thread_task_queue;
}

const std::string &get_name()
{
    return current_thread_name;
}

}   // namespace this_thread

}   // namespace MiniUtils

