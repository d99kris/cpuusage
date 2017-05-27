#!/bin/bash

# Decompress dependency catapult - originally from https://github.com/catapult-project/catapult
if [ ! -d "./catapult" ]; then
  tar -xzvf catapult.tar.gz
  # Reminder: This tar-ball is created at this directory level:
  # tar -czvf catapult.tar.gz catapult
fi

# Create build directory
if [ ! -d "./build" ]; then
  mkdir build
fi

# Build
cd build && \
cmake .. && \
make || \
exit ${?}

# Run examples
echo "-- Running examples"
rm ./culog*.json 2> /dev/null
CU_FILE="./culog1.json" LD_PRELOAD="./libexwrapper.so" DYLD_INSERT_LIBRARIES="./libexwrapper.dylib" DYLD_FORCE_FLAT_NAMESPACE=1 ./cutest1
CU_FILE="./culog2.json" ./cutest2
CU_FILE="./culog3.json" LD_PRELOAD="./libexwrapper.so" DYLD_INSERT_LIBRARIES="./libexwrapper.dylib" DYLD_FORCE_FLAT_NAMESPACE=1 ./cutest3

# Generate reports
cd - > /dev/null
rm ./build/culog*.html 2> /dev/null
echo "-- Generating report for example: cutest1"
./catapult/tracing/bin/trace2html ./build/culog1.json --output=./build/culog1.html
echo "-- Generating report for example: cutest2"
./catapult/tracing/bin/trace2html ./build/culog2.json --output=./build/culog2.html
echo "-- Generating report for example: cutest3"
./catapult/tracing/bin/trace2html ./build/culog3.json --output=./build/culog3.html

# Done
exit

