#!/bin/bash

cd src && help2man -n "instrumentation CPU profiler" -N -o cpuusage.1 ./cpuusage
exit ${?}

