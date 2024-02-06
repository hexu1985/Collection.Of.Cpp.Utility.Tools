import time
from multiprocessing import Process, Queue


def data_provider(q):
    for i in range(5):
        q.put(i)


def plus_one(inq, outq):
    while True:
        x = inq.get()
        time.sleep(0.5)
        outq.put(x + 1)


def mul_two(inq, outq):
    while True:
        x = inq.get()
        time.sleep(0.5)
        outq.put(x * 2)

if __name__ == '__main__':
    queues: list[Queue] = [Queue(1) for _ in range(3)]  # 限制队列长度以防止队列爆炸
    processes = [
        Process(target=data_provider, args=(queues[0],)),
        Process(target=plus_one, args=(queues[0], queues[1])),
        Process(target=mul_two, args=(queues[1], queues[2])),
    ]

    for p in processes:
        p.start()

    while True:
        start_time = time.time()
        output = queues[2].get()
        if output is None:
            break
        print(f"{output}: {time.time() - start_time:.1f}s")
