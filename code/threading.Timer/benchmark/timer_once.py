#!/usr/bin/env python3

from threading import  Timer

def hello():
    print("hello, world")

t = Timer(3.0, hello)
t.start()  # after 3 seconds, "hello, world" will be printed
