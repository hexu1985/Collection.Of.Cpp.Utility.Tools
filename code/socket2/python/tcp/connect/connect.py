#!/usr/bin/env python3
# Basic Connection Example - Chapter 2 - connect.py

import socket

print("Creating socket...", end=" ", flush=True)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("done.")

print("Connecting to remote host...", end=" ", flush=True)
s.connect(("www.google.com", 80))
print("done.")