# python program to explain the
# use of cancel() method in Timer class

import threading

def helper_function(i):
    print("Value printed=",i)

if __name__=='__main__':

    thread1 = threading.Timer(interval = 3, function = helper_function,args = (9,))
    print("Starting the timer object")
    print()

    # Starting the function after 3 seconds
    thread1.start()

    print("This gets printed before the helper_function as helper_function starts after 3 seconds")
    print()

    # This cancels the thread when 3 seconds 
    # have not passed
    thread1.cancel()
    print("Thread1 cancelled, helper_function is not executed")
