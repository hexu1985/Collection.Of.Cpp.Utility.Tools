#!/bin/bash
  
sudo bash -c "echo -1 > /proc/sys/kernel/perf_event_paranoid"
perf record ./example

#perf report
perf report --stdio


