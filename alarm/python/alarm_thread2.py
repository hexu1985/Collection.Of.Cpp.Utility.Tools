#!/usr/bin/env python3

from threading import Timer
from datetime import datetime

class Alarm:
    def __init__(self, seconds:int, message:str):
        self.seconds = seconds
        self.message = message

def callback(alarm:Alarm):
    print("\nstart timer [({}) {}] at {}".format(alarm.seconds, alarm.message, datetime.now().strftime("%Y-%m-%d %H:%M:%S")))

if __name__ == "__main__":
    while True:
        line = input("Alarm> ")
        if len(line) <= 1:
            continue

        try:
            seconds, *messages = line.split(' ')
            alarm = Alarm(int(seconds), ' '.join(messages))
            print("start timer [({}) {}] at {}".format(alarm.seconds, alarm.message, datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
            t = Timer(interval=int(seconds), function=callback, args=(alarm, ))
            t.start()
        except:
            print("Bad command")

