#!/usr/bin/env python3

import threading
import datetime
import time

def print_message(message):
    print(f"{datetime.datetime.now()}: {message}")

def repeat_hello():
    print_message("Hello, World!");
    timer = threading.Timer(2.0, repeat_hello)
    timer.start()

def main():
    timer = threading.Timer(2.0, repeat_hello)
    timer.start()
    print_message("Repeating timer started...");

if __name__ == "__main__":
    main()
