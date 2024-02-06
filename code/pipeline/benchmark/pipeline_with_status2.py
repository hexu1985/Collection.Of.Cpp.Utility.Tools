from enum import Enum
import traceback
import time
from multiprocessing import Process, Queue


class Status(Enum):
    NORMAL = 0
    ERROR = 1
    STOP = 2

class Wrapper:
    def __init__(self, status: Status, x):
        self.status = status
        self.x = x

def input_worker(data_provider, outq: Queue):
    try:
        for x in data_provider():
            outq.put(Wrapper(Status.NORMAL, x))
        outq.put(Wrapper(Status.STOP, None))
    except Exception as e:
        outq.put(Wrapper(Status.ERROR, (e, traceback.format_exc())))


def worker(func, inq: Queue, outq: Queue):
    while True:
        wrapper = inq.get()
        if wrapper.status != Status.NORMAL:
            outq.put(wrapper)
        else:
            try:
                wrapper.x = func(wrapper.x)
                outq.put(wrapper)
            except Exception as e:
                outq.put(Wrapper(Status.ERROR, (e, traceback.format_exc())))

class Pipeline:
    def __init__(self, data_provider, *funcs):
        num_workers = len(funcs) + 1
        self.queues = [Queue(1) for _ in range(num_workers)]

        self.processes = [
                Process(
                    target=input_worker,
                    args=(data_provider, self.queues[0])
                    )
                ]

        for i, func in enumerate(funcs):
            self.processes.append(
                    Process(
                        target=worker,
                        args=(func, self.queues[i], self.queues[i + 1])
                        )
                    )

        for p in self.processes:
            p.start()

    def __next__(self):
        output = self.queues[-1].get()
        if output.status == Status.NORMAL:
            return output.x
        else:
            self.stop(output)
            raise StopIteration

    def __iter__(self):
        return self

    def stop(self, wrapper: Wrapper | None = None):
        for p in self.processes:
            p.terminate()
        if wrapper is not None and wrapper.status == Status.ERROR:
            error, traceback_info = wrapper.x
            raise RuntimeError(
                    f"Error in pipeline:\n{traceback_info}") from error

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.stop()

    def __del__(self):
        self.stop()

def data_provider():
    for i in range(5):
        if i == 2:
            raise ValueError("i == 2")
        yield i


def plus_one(x):
    time.sleep(0.5)
    return x + 1


def mul_two(x):
    time.sleep(0.5)
    return x * 2


if __name__ == '__main__':
    with Pipeline(data_provider, plus_one, mul_two) as pipeline:
        start_time = time.time()
        for output in pipeline:
            print(f"{output}: {time.time() - start_time:.1f}s")
            start_time = time.time()
