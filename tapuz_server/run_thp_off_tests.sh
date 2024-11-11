#!/bin/bash

echo never > /sys/kernel/mm/transparent_hugepage/enabled
for i in {1..5};do perf stat --repeat=3 --field-separator=, --output=./thp_off_low_util/$i/thpoff$i.out --event cycles,instructions,dTLB-load-misses,dTLB-loads,L1-dcache-load-misses,L1-dcache-loads,LLC-load-misses,LLC-loads --  time --format=seconds-elapsed,%e\nuser-time-seconds,%U\nkernel-time-seconds,%S\nmax-resident-memory-kb,%M --output=./thp_off_low_util/$i/time.out  ./mosmodel_dev/scripts/setCpuMemoryAffinity.sh 1 ./new_benchmark/lu128.out $i; done