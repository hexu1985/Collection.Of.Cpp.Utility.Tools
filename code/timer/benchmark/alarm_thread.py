#!/usr/bin/env python3

from threading import Timer
from datetime import datetime

class Alarm:
    def __init__(self, interval:float, message:str):
        self.interval = interval
        self.message = message

def callback(alarm:Alarm):
    print("alarm timer at {}".format(datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")))
    print("({}) {}\n".format(alarm.interval, alarm.message))

if __name__ == "__main__":
    while True:
        line = input("Alarm> ")
        if len(line) <= 1:
            continue

        try:
            interval, *message = line.split(' ')
            alarm = Alarm(float(interval), ' '.join(message))
            print("start timer at {}".format(datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")))
            t = Timer(interval=float(interval), function=callback, args=(alarm, ))
            t.start()
        except:
            print("Bad command")

