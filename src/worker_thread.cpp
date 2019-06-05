#include "worker_thread.hpp"

#include <cassert>

using namespace std;

namespace MiniUtils {

namespace {     // details

thread_local shared_ptr<TaskQueue> current_thread_task_queue;
thread_local string current_thread_name;

// WorkerThreadInterrupt 
struct WorkerThreadInterrupt {
};

void this_thread_exit()
{
    throw WorkerThreadInterrupt();
}

void task_process(WorkerThread *worker_thread)
{
    current_thread_task_queue = worker_thread->getTaskQueue();
    current_thread_name = worker_thread->getName();

    TaskQueue &incoming_queue = *current_thread_task_queue;
	while (true) {
        TaskPtrList working_list;
		incoming_queue.popTask(working_list);
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

}   // namespace {

// WorkerThread
WorkerThread::WorkerThread(const string &name): name_(name) 
{
}

WorkerThread::~WorkerThread() 
{
    if (isRun()) {
        stop();
    }
}

void WorkerThread::start() 
{
    assert(!thread_ && !taskQueue_);
    taskQueue_ = make_shared<TaskQueue>();
    thread_ = make_shared<thread>(&task_process, this);
}

void WorkerThread::stop() 
{
    taskQueue_->pushTask(&this_thread_exit);
    thread_->join();
    thread_.reset();
    taskQueue_.reset();
}

bool WorkerThread::isRun() 
{
    return (thread_ ? true : false);
}

shared_ptr<TaskQueue> WorkerThread::getTaskQueue() 
{
    return taskQueue_;
}

const string &WorkerThread::getName() const
{
    return name_;
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

