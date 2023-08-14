### 任务队列类和工作线程类

- Task: 任务类为模板类(Task<Fn>)，继承自TaskBase基类
- TaskQueue: 一个线程安全的任务队列类，容器元素类型为std::shared_ptr<TaskBase> 
- WorkerThread: 一个处理任务队列的工作线程
