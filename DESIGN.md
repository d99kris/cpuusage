Cpuusage - Design
=================
This document is intended to provide some design documentation and developer
guidelines for maintaining/developing this project further.

Catapult
--------
Catapult is the most important third-party component of Cpuusage. Not the entire
Catapult software is needed by Cpuusage, so only a part of the original package
is distributed with Cpuusage. It is compressed and located in catapult.tar.gz

In case Catapult needs to be updated (it happened when updating to Python 3.x
support) the recommended method is:

1.  Build Cpuusage

    ./genman.sh

2.  Run tests

    cd build && ctest --output-on-failure

3.  Perform any necessary updates to the Catapult copy in
    build/share/cpuusage/catapult to make tests pass.

4.  When all tests are passing, create a new tar-ball and make sure to exclude
    test_data, e.g. `rm -rf share/cpuusage/catapult/tracing/test_data`
    The tar-ball can be created like this:

    tar -czvf ~/c1.tar.gz share/cpuusage/catapult
   
    Then replace the original file:

    cp ~/c1.tar.gz ../catapult.tar.gz

5.  Finally delete the previous build directory and try build and run tests
    again.

