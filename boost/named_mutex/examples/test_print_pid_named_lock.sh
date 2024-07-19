#!/bin/bash

#./named_mutex_remove "mtx"
#./named_mutex_create "mtx"

for i in $(seq 1 10)
do
    ./print_pid_named_lock $i &
done

