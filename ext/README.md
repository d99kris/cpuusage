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

1.  Run script to update external modules

    ./ext/update.sh

2.  Perform any necessary updates to the update.sh script to make tests pass.

3.  Once all tests are passing, ext/catapult.tar.gz will be updated
    automatically.

Six
---
The `six` Python module is required by Catapult but upstream no longer ships
it inside the Catapult tree, and the Homebrew `six` formula was disabled in
2025. A vendored copy of [six](https://github.com/benjaminp/six) (MIT) is
kept in `ext/six/` and folded into `catapult/third_party/six/` by
`update.sh` so that `catapult.tar.gz` is self-contained.

