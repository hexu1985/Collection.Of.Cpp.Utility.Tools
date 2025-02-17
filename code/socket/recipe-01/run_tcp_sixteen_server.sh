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

# usage: ./tcp_sixteen [--help] [--role client|server] [--host HOST] [--port PORT]
./tcp_sixteen --host "${server_host}" --port "${server_port}"