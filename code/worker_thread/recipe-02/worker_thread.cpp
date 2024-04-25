#include "worker_thread.hpp"

#include <cassert>

using namespace std;

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

void task_process(WorkerThread* worker_thread)
{
    current_thread_task_queue = worker_thread->GetTaskQueue();
    current_thread_name = worker_thread->GetThreadName();

    TaskQueue& incoming_queue = *current_thread_task_queue;
	while (true) {
        std::deque<Task> working_list;
		incoming_queue.PopTask(working_list);
		while (!working_list.empty()) {
			auto task = working_list.front();
			working_list.pop_front();
            try {
                task.Run();
            } catch (WorkerThreadInterrupt) {
                return;
            }
		}
	}
}

}   // namespace {

// WorkerThread
WorkerThread::WorkerThread(const string& name): name_(name) 
{
}

WorkerThread::~WorkerThread() 
{
    if (IsRun()) {
        Stop();
    }
}

void WorkerThread::Start() 
{
    assert(!thread_ && !task_queue_);
    task_queue_ = make_shared<TaskQueue>();
    thread_ = make_shared<thread>(&task_process, this);
}

void WorkerThread::Stop() 
{
    task_queue_->PushTask(&this_thread_exit);
    thread_->join();
    thread_.reset();
    task_queue_.reset();
}

bool WorkerThread::IsRun() 
{
    return (thread_ ? true : false);
}

shared_ptr<TaskQueue> WorkerThread::GetTaskQueue() 
{
    return task_queue_;
}

const string& WorkerThread::GetThreadName() const
{
    return name_;
}

std::shared_ptr<TaskQueue> WorkerThread::GetCurrentTaskQueue()
{
    return current_thread_task_queue;
}

const std::string& WorkerThread::GetCurrentThreadName()
{
    return current_thread_name;
}

