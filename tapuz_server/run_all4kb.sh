#!/bin/bash

for i in {1..5};do perf stat --repeat=3 --field-separator=, --output=./all4kb128/$i/all4kb$i.out --event cycles,instructions,dTLB-load-misses,dTLB-loads,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads --  time --format=seconds-elapsed,%e\nuser-time-seconds,%U\nkernel-time-seconds,%S\nmax-resident-memory-kb,%M --output=./cold_low_util/$i/time.out  ./mosmodel_dev/scripts/setCpuMemoryAffinity.sh 1  ./mosalloc_dev/runMosalloc.py --library  ./mosalloc_dev/build/src/libmosalloc.so -cpf ./all4kb128/layout1.csv ./new_benchmark/lu128.out $i; done
