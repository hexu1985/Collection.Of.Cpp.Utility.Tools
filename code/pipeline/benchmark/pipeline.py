import time
from multiprocessing import Process, Queue


def input_worker(data_provider, inq):
    for x in data_provider():
        inq.put(x)


def worker(func, inq, outq):
    while True:
        x = inq.get()
        outq.put(func(x))


class Pipeline:
    def __init__(self, data_provider, *funcs):
        self.queues = [Queue(1) for _ in range(len(funcs) + 1)]

        self.processes = [
                Process(
                    target=input_worker, 
                    args=(data_provider, self.queues[0])),
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
        return self.queues[-1].get()

    def __iter__(self):
        return self

    def stop(self):
        for p in self.processes:
            p.terminate()

def data_provider():
    for i in range(5):
        yield i


def plus_one(x):
    time.sleep(0.5)
    return x + 1


def mul_two(x):
    time.sleep(0.5)
    return x * 2


if __name__ == '__main__':

    pipeline = Pipeline(data_provider, plus_one, mul_two)

    start_time = time.time()
    for output in pipeline:
        print(f"{output}: {time.time() - start_time:.1f}s")
        start_time = time.time()
