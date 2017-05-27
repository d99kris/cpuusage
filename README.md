Cpuusage
========
Cpuusage is a simple instrumentation CPU profiler for Linux and macOS applications.
By using compiler intrumentation (-finstrument-functions) it enables capturing start and end of function
calls. A JSON logging format compatible with
[Chromium Catapult](https://github.com/catapult-project/catapult) is used, enabling nice visualization
of the captured data.

Supported Platforms
===================
Cpuusage is primarily developed and tested on Linux, but basic
functionality should work in macOS / OS X as well. Current version has been tested on:
- OS X El Capitan 10.11
- Ubuntu 16.04 LTS

Usages
======
Tracing Standard Function Calls
-------------------------------
Without making any changes to an existing application is it possible to trace its calls to a specified
set of function calls. The header file
[examples/exwrapper.h](https://github.com/d99kris/cpuusage/blob/master/examples/exwrapper.h)
provides an example of a listing required to trace specified functions. By using the provided code
generation tool
[src/genwrapper.py](https://github.com/d99kris/cpuusage/blob/master/src/genwrapper.py) one can
generate a C file with the corresponding wrapper function bodies. The final step is to build a library of
these files and preload it at program start. Refer to 
[CMakeLists.txt](https://github.com/d99kris/cpuusage/blob/master/CMakeLists.txt) example
"cutest1" for how it can be done in cmake.

With wrapper header
[examples/exwrapper.h](https://github.com/d99kris/cpuusage/blob/master/examples/exwrapper.h)
and test program
[examples/cutest.c](https://github.com/d99kris/cpuusage/blob/master/examples/cutest.c)
the output may look like this:
![culog1 screenshot](/doc/culog1.png)

The actual
[Catapult generated HTML log](https://raw.githubusercontent.com/d99kris/cpuusage/master/doc/culog1.html)
(right-click and save locally before opening, as GitHub blocks Javascript content)
and corresponding
[JSON log data](https://github.com/d99kris/cpuusage/blob/master/doc/culog1.json)
can be accessed for better understanding of the detailed log data provided.

Tracing an Applications Internal Calls
--------------------------------------
To trace the timing of an applications internal calls one need to recompile the application with 
"-rdynamic -finstrument-functions". One can compile with instrumentation enabled on a file-by-file basis.

The output may look like this:
![culog2 screenshot](/doc/culog2.png)

The actual
[Catapult generated HTML log](https://raw.githubusercontent.com/d99kris/cpuusage/master/doc/culog2.html)
(right-click and save locally before opening, as GitHub blocks Javascript content)
and corresponding
[JSON log data](https://github.com/d99kris/cpuusage/blob/master/doc/culog2.json)
can be accessed for better understanding of the detailed log data provided.

Tracing Standard Function Calls + Applications Internal Calls
-------------------------------------------------------------
This is essentially a combination of the options above, in which both a set of external function calls
are being traced in addition to application internal calls.

The output may look like this:
![culog3 screenshot](/doc/culog3.png)

The actual
[Catapult generated HTML log](https://raw.githubusercontent.com/d99kris/cpuusage/master/doc/culog3.html)
(right-click and save locally before opening, as GitHub blocks Javascript content)
and corresponding
[JSON log data](https://github.com/d99kris/cpuusage/blob/master/doc/culog3.json)
can be accessed for better understanding of the detailed log data provided.

Usage
=====
Pre-requisites (Ubuntu):

    sudo apt install git cmake build-essential

Download the source code:

    git clone https://github.com/d99kris/cpuusage && cd cpuusage

Build and run examples:

    ./run-examples.sh
    
Detailed Usage
==============
The run-time configuration of cpuusage is done using environment variables.

CU_FILE
-------
This environment variable determines the path and filename of the output JSON trace file. If not
specified the default output log name is culog-<PID>.json.

CU_MANUAL
---------
If this environment variable is set to "1", tracing is not automatically started at program startup, but
rather requires manual start/stop by sending below signals to the relevant process:

    kill -SIGUSR1 `pidof myprogram`  # starts tracing
    kill -SIGUSR2 `pidof myprogram`  # stops tracing

CU_MAX_SAMPLES
--------------
By default the maximum number of samples is 1000000. Once this number of samples have been collected, the
tracing is automatically disabled. This number also controls the amount of memory allocated upfront by
cpuusage.

Alternatives
============
There are many profilers available for Linux and macOS, here are some of them:
- callgrind
- gperftools
- gprof

Limitations
===========
Some known limitations:
- The following functions are used by cpuusage logger internally, and cannot be wrapped/instrumented for logging: gettimeofday(), pthread_self().
- In order to get function names when tracing an applications internal calls on Linux, the functions need to be public / non-static.

License
=======
Cpuusage is distributed under the BSD 3-Clause license. See LICENSE file.

Keywords
========
linux, macos, os x, cpu usage, profiler, performance.

