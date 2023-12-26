#!/usr/bin/env python3

import time
import threading

def createTimer():
    t = threading.Timer(2, repeat)
    t.start()

def repeat():
    print('Now:', time.strftime('%H:%M:%S',time.localtime()))
    createTimer()
    
createTimer()
