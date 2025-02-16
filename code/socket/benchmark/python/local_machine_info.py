#!/usr/bin/env python3
# Python Network Programming Cookbook -- Chapter - 1
# This program is optimized for Python 2.7.
# It may run on any other version with/without modifications.

import socket


def print_machine_info():
    host_name = socket.gethostname()
    ip_address = socket.gethostbyname(host_name)
    print("Host name: {}".format(host_name))
    print("IP address: {}".format(ip_address))

if __name__ == '__main__':
    print_machine_info()
