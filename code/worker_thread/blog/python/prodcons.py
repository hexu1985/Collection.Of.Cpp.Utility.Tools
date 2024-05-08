#!/usr/bin/env python

from random import randint
from time import sleep
from functools import partial
from threading import Thread
from worker_thread import WorkerThread

def mul_two(i):
    print('mul_two({}) is {}'.format(i, i*2))

def consume(i):
    print('consume with data: {}'.format(i))
    mul_two(i)
    sleep(randint(2, 5))

def producer(worker_thread, loops):
    for i in range(loops):
        print('pushTask with data: {}'.format(i))
        worker_thread.putTask(partial(consume, i))
        sleep(randint(1, 3))

def main():
    nloops = randint(2, 5)

    myworker = WorkerThread("worker")
    myworker.start()

    myproducer = Thread(target=producer, args=(myworker, nloops))
    myproducer.start()
    myproducer.join()

    myworker.stop()
    myworker.join()

    print('all DONE')

if __name__ == '__main__':
    main()
