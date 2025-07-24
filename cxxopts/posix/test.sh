#!/usr/bin/bash

cd $(dirname ${BASH_SOURCE[0]})

# test getopt
echo '-------------------------------------'
echo '----------------getopt---------------'
echo '-------------------------------------'
set -x
./argopt -i -lr 'hi here' -f fred.c -q
set +x
echo

# test pshm_write 
echo '-------------------------------------'
echo '------------getopt_long--------------'
echo '-------------------------------------'
set -x
./longopt --initialize --list 'hi there' --file fred.c -q
./longopt --init -l --file=fred.c 'hi there'
set +x
echo

