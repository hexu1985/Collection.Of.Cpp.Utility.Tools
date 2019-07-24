#!/usr/bin/env bash
find . -name "Makefile" | awk '{print substr($0, 3, length($0)-2-length("Makefile"))}' | grep -v '^$'
