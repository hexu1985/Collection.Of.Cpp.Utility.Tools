#!/usr/bin/env python3

# python program to explain the
# use of cancel() method in Timer class

import threading
import time

def helper_function(i):
    print("Value printed=",i)
    print()
  
if __name__=='__main__':
    
    timer1 = threading.Timer(interval = 3, function = helper_function,args = (19,))
    print("Starting the timer object")
    print()
    
    # Starting the function after 3 seconds
    timer1.start()
    # Sleeping this thread for 5 seconds
    time.sleep(5)
    
    # This will not cancel the thread as 3 seconds have passed
    timer1.cancel()
    print("This time thread is not cancelled as 3 seconds have passed when cancel() method is called")

