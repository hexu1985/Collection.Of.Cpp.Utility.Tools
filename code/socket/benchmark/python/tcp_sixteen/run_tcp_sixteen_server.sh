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

# usage: tcp_sixteen.py [-h] [-p PORT] {client,server} host
python3 tcp_sixteen.py -p "${server_port}" server "${server_host}"
