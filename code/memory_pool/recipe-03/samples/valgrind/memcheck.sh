#!/usr/bin/env bash
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./sample_byte_memory_pool1
