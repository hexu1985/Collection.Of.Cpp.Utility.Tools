#!/usr/bin/env python3

import threading
import datetime
import time

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def say_hello():
    print_message("Hello, World!")

def main():
    timer = threading.Timer(5.0, say_hello)
    timer.start()
    print_message("Timer started, waiting for it to trigger...")
    
    time.sleep(1)
    print_message("Now we will cancel it.")
    timer.cancel()
    
    while timer.is_alive():
        print_message("Timer is active...")
        time.sleep(1)
    
    print_message("Timer cancelled.")

if __name__ == "__main__":
    main()
