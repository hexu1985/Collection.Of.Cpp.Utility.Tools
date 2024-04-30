#!/usr/bin/env python

from random import randint
from time import sleep
from queue import Queue
from threading import Thread

def writeQ(queue, val):
    print('producing object for Q...:', val)
    queue.put(val)

def readQ(queue):
    val = queue.get(1)
    print('consumed object from Q...:', val)
    return val

def mul_two(i):
    print('mul_two({}) is {}'.format(i, i*2))

def producer(queue, loops):
    for i in range(loops):
        writeQ(queue, i)
        sleep(randint(1, 3))

    writeQ(queue, -1)

def consumer(queue, loops):
    while True:
        i = readQ(queue)
        if i < 0: break
        mul_two(i)
        sleep(randint(2, 5))

funcs = [producer, consumer]
nfuncs = range(len(funcs))

def main():
    nloops = randint(2, 5)
    q = Queue(32)

    threads = []
    for i in nfuncs:
        t = Thread(target=funcs[i], args=(q, nloops))
        threads.append(t)

    for i in nfuncs:
        threads[i].start()

    for i in nfuncs:
        threads[i].join()

    print('all DONE')

if __name__ == '__main__':
    main()
