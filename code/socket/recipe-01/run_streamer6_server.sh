#!/usr/bin/env bash

server_host="::"
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

# usage: ./streamer6 [--help] [--client] [--host HOST] [--port PORT]
./streamer6 --host "${server_host}" --port "${server_port}"
