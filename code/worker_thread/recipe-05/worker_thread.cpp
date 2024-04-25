#include "worker_thread.hpp"

#include <cassert>
#include <atomic>

namespace {     // details

std::atomic<int> thread_count{};
thread_local std::shared_ptr<TaskQueue> current_thread_task_queue{};
thread_local std::string current_thread_name{};

// WorkerThreadInterrupt 
struct WorkerThreadInterrupt {
};

void this_thread_exit() {
    throw WorkerThreadInterrupt();
}

void process_task_loop(WorkerThread* worker_thread) {
    current_thread_task_queue = worker_thread->GetTaskQueue();
    current_thread_name = worker_thread->GetThreadName();

    TaskQueue& incoming_queue = *current_thread_task_queue;
	while (true) {
        std::deque<Task> working_list;
		incoming_queue.PopTask(working_list);
		while (!working_list.empty()) {
			auto task = std::move(working_list.front());
			working_list.pop_front();
            try {
                task();
            } catch (WorkerThreadInterrupt) {
                current_thread_name = "";
                current_thread_task_queue = nullptr;
                return;
            }
		}
	}
}

}   // namespace {

// WorkerThread
WorkerThread::WorkerThread(const std::string& name): thread_name(name) {
    int id = ++thread_count;
    thread_name += ":"+std::to_string(id);
}

WorkerThread::~WorkerThread() {
    if (IsRunning()) {
        Stop();
    }
}

void WorkerThread::Start() {
    if (IsRunning()) {
        Stop();
    }

    task_queue = std::make_shared<TaskQueue>();
    looper_thread = std::thread(&process_task_loop, this);
}

void WorkerThread::Stop() {
    task_queue->PushTask(&this_thread_exit);
    looper_thread.join();

    looper_thread = std::thread();
    task_queue = nullptr;
}

bool WorkerThread::IsRunning() {
    return looper_thread.joinable(); 
}

std::shared_ptr<TaskQueue> WorkerThread::GetTaskQueue() {
    return task_queue;
}

const std::string& WorkerThread::GetThreadName() const {
    return thread_name;
}

std::shared_ptr<TaskQueue> WorkerThread::GetCurrentTaskQueue() {
    return current_thread_task_queue;
}

const std::string& WorkerThread::GetCurrentThreadName() {
    return current_thread_name;
}

