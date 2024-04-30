import threading
import logging
import queue
import time
import sys
import traceback

LOGGER = logging.getLogger()

class StopWorkerThreadException(Exception):
    def __init__(self):
        pass

    def __repr__(self):
        return "StopWorkerThreadException"

    def __str__(self):
        return "StopWorkerThreadException"

class WorkerThread(threading.Thread):
    def __init__(self, name=""):
        super().__init__()
        self.task_queue = queue.Queue()
        self.name = name

    def fetch_and_exectue_task_once(self):
        task = self.task_queue.get()
        if callable(task):
            task()

    def run(self):
        LOGGER.info("worker {} is running".format(self.name))
        while True:
            try:
                self.fetch_and_exectue_task_once()
            except StopWorkerThreadException as e:
                LOGGER.info("get stoptask and exit workthread")
                break
            except:
                exc_type, exc_value, exc_traceback = sys.exc_info()
                error_list = traceback.format_exception(exc_type, exc_value, exc_traceback)
                LOGGER.error("task execute error: \n{}".format(''.join(error_list)))

    def putTask(self, task):
        self.task_queue.put(task)

    def stop(self):
        def StopTask():
            raise StopWorkerThreadException
    
        self.putTask(StopTask)

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    class DemoTask:
        def __init__(self, name):
            self.name = name

        def __call__(self):
            print("execute {}".format(self.name))
            time.sleep(1.0)

    print("test of worker_thread.py")
    worker = WorkerThread()
    worker.start()

    print("start task")
    for i in range(1, 5):
        worker.putTask(DemoTask(i))

    print("stop worker thread")
    worker.stop()

    print("join worker thread")
    worker.join()
    print("exit of worker_thread.py")

