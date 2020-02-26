#!/bin/bash

mkdir -p build && cd build && cmake .. && make -s && \
cd ../src && help2man -n "instrumentation CPU profiler" -N -o cpuusage.1 ./cpuusage
exit ${?}

