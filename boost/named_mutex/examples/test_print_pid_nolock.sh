#!/bin/bash

for i in $(seq 1 10)
do
    ./print_pid_nolock $i &
done
