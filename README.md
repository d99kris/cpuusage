Cpuusage
========
Cpuusage is an instrumentation CPU profiler for Linux applications.
It provides logging of standard POSIX function calls for any application, and logging of
internal function calls for programs compiled with -finstrument-functions.
The logging output format is a HTML-file generated using 
[Chromium Catapult](https://github.com/catapult-project/catapult).

Example Usage
=============
Tracing all POSIX function calls from a regular (non-instrumented) application:

    $ cpuusage -v -o cutrace1.html -a ./build/cutest1 
    cpuusage: https://github.com/d99kris/cpuusage
    cpuusage: initializing
    cpuusage: starting program ./build/cutest1
    cpuusage: processing output trace
    cpuusage: completed processing 96 samples
    $ xdg-open cutrace1.html 

Resulting visualization:
![culog1 screenshot](/doc/culog1.png)

Supported Platforms
===================
Cpuusage is primarily developed and tested on Linux.

Installation
============
Pre-requisites (Ubuntu):

    sudo apt install git cmake build-essential python-lxml

Download the source code:

    git clone https://github.com/d99kris/cpuusage && cd cpuusage

Generate Makefile and build:

    mkdir -p build && cd build && cmake .. && make -s

Optionally install in system:

    sudo make install

Usage
=====

General syntax:

    cpuusage -a [OPTIONS] PROG [ARGS..]
    cpuusage -c [OPTIONS] PROG [ARGS..]
    cpuusage -f <FUNCTIONS> [OPTIONS] PROG [ARGS..]
    cpuusage -i <INCLUDES> [OPTIONS] PROG [ARGS..]
    cpuusage --help|-h
    cpuusage --version|-v

Options:

    -a     trace all standard POSIX function calls

    -c     trace internal function calls (requires PROG to be compiled with
           -finstrument-functions)

    -f <FUNCTIONS>
           trace specified POSIX functions

    -i <INCLUDES>
           trace functions in specified POSIX headers

    --help, -h
           display this help and exit

    --version, -v
           output version information and exit

    -j <PATH>
           write iterim JSON trace log to specified path

    -m <SAMPLES>
           only log up to specified number of samples

    -o <PATH>
           write  HTML  report  to  specified  path,  rather  than  default
           ./cutrace-PID.html

    -v     verbose mode

    PROG   program to run and profile

    [ARGS] optional arguments to the program

More Examples
=============
**Tracing specified POSIX function calls from a regular (non-instrumented) application:**

    $ cpuusage -v -o cutrace2.html -f fopen,fwrite,fread,fclose ./build/cutest1 
    cpuusage: https://github.com/d99kris/cpuusage
    cpuusage: initializing
    cpuusage: starting program ./build/cutest1
    cpuusage: processing output trace
    cpuusage: completed processing 36 samples
    $ xdg-open cutrace2.html 

Resulting visualization:
![culog2 screenshot](/doc/culog2.png)

**Tracing internal function calls in an instrumented application:**

    $ cpuusage -v -o cutrace3.html -c ./build/cutest2
    cpuusage: https://github.com/d99kris/cpuusage
    cpuusage: initializing
    cpuusage: starting program ./build/cutest2
    cpuusage: processing output trace
    cpuusage: completed processing 8 samples
    $ xdg-open cutrace3.html 

Resulting visualization:
![culog3 screenshot](/doc/culog3.png)

**Tracing internal function calls and all external POSIX function calls in an instrumented application:**

    $ cpuusage -v -o cutrace4.html -c -a ./build/cutest2
    cpuusage: https://github.com/d99kris/cpuusage
    cpuusage: initializing
    cpuusage: starting program ./build/cutest2
    cpuusage: processing output trace
    cpuusage: completed processing 104 samples
    $ xdg-open cutrace4.html 

Resulting visualization:
![culog4 screenshot](/doc/culog4.png)

Alternatives
============
There are many CPU profilers available for Linux. Most of them are sample-based, and here
is a list of some of them:
- Gperftools
- Gprof
- Valgrind - Callgrind

License
=======
Cpuusage is distributed under the BSD 3-Clause license. See LICENSE file.

Keywords
========
linux, cpu usage, instrumented profiler, alternative to callgrind.

