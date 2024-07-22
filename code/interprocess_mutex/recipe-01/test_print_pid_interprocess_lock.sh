#!/bin/bash

./interprocess_mutex_remove "mtx"
./interprocess_mutex_create "mtx"

for i in $(seq 1 10)
do
    ./print_pid_interprocess_lock $i &
done

