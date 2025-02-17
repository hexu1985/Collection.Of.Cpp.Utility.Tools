#!/usr/bin/env bash

server_host="0.0.0.0"
server_port=9999

while getopts p: opt
do
    case "$opt" in
        p) server_port=$opt ;;
    esac
done

shift $[ $OPTIND - 1 ]

if [ $# -eq 1 ]
then
   server_host=$1
fi 

# usage: blocks.py [-h] [-c] [-p port] [hostname]
python3 blocks.py -p "${server_port}" "${server_host}"
