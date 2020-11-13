External Components
===================
This directory contains external components used by cpuusage.

Catapult
--------
Catapult is the most important third-party component of cpuusage. Not the entire
Catapult software is needed by cpuusage, so only a part of the original package
is distributed with cpuusage. It is compressed and located in catapult.tar.gz

### Trace File Format
Cpuusage generates JSON files following the
[Trace Event Format](/ext/catapult_trace_event_format.pdf) which subsequently
is converted by Catapult into a HTML report.

The Trace Event Format PDF was generated from the Google Document
[Trace Event Format](https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/edit)
linked from [Catapult Tracing](https://chromium.googlesource.com/catapult/+/HEAD/tracing/README.md).

### Updating Catapult
In case Catapult needs to be updated (it happened when updating to Python 3.x
support) the recommended method is:

1.  Build cpuusage

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

    cp ~/c1.tar.gz ../ext/catapult.tar.gz

5.  Finally delete the previous build directory and try build and run tests
    again.

