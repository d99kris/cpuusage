#!/usr/bin/python

# Copyright (C) 2017 Kristofer Berggren
# All rights reserved.
#
# cpuusage is distributed under the BSD 3-Clause license, see LICENSE for details.
#
# Todo:
# - Make generated initialization code thread-safe (mutex or spinlock protected)
# - Handle function pointer arguments
# - Handle variable arguments (...)
#

import re
import sys

def genwrapper(infilepath, outfilepath):
    infile = open(infilepath, 'r')
    outfile = open(outfilepath, 'w')
    # Target buffers
    headers = "#define _GNU_SOURCE\n#include <dlfcn.h>\n"
    globs = "static int inited = 0;"
    lin_wrapinit = ""
    lin_wrapdecl = ""
    lin_wrapbody = ""
    osx_wrapdecl = ""
    osx_wrapbody = ""
    for line in infile:
        if line.startswith("//"):
            # skip comments
            continue
        elif line.startswith("#include"):
            # copy include statements
            headers += line
        elif line.endswith(";\n"):
            paropen = line.find("(")
            parclose = line.rfind(")")
            semi = line.find(";")
            if (paropen > -1) and (parclose > -1) and (semi > -1):
                namestart = max(line[0:paropen].rfind(" "), line[0:paropen].rfind("*"))
                rtype = line[0:namestart+1]
                rcall = "return "
                if rtype.strip() == "void":
                    rcall = ""
                name = line[namestart+1:paropen]
                args = line[paropen+1:parclose]
                callargs = ""
                for arg in args.split(","):
                    argnamestart = max(arg.rfind(" "), arg.rfind("*"))
                    if argnamestart > 0:
                        argname = arg[max(arg.rfind(" "), arg.rfind("*")) + 1:]
                        if len(callargs) > 0:
                            callargs += ", "
                        callargs += argname
                    
                if (name == "malloc") or (name == "free") or (name == "calloc") or (name == "realloc"):
                    lin_wrapdecl += "" + rtype + "__libc_" + name + "(" + args + ");\n"
                    lin_wrapinit += ""
                    lin_wrapbody += line[0:semi] + "\n{\n  " + rcall + "__libc_" + name + "(" + callargs + ");\n}\n\n";
                else:
                    lin_wrapdecl += "static " + rtype + "(*real_" + name + ") " + "(" + args + ")" + " = NULL;\n"
                    lin_wrapinit += "  real_" + name + " = dlsym(RTLD_NEXT, \"" + name + "\");\n"
                    lin_wrapbody += line[0:semi] + "\n{\n  if (!inited) { wrapper_init(); }\n  " + rcall + "real_" + name + "(" + callargs + ");\n}\n\n";
                    
                osx_wrapdecl += rtype + "wrap_" + name + "(" + args + ");\n"
                osx_wrapbody += rtype + "wrap_" + name + "(" + args + ")\n{\n  " + rcall + name + "(" + callargs + ");\n}\n" + "DYLD_INTERPOSE(wrap_" + name + ", " + name + ");\n\n"

    outfile.write(headers + "\n")

    outfile.write(globs + "\n")
    
    outfile.write("#ifdef __APPLE__\n")
    outfile.write("#define DYLD_INTERPOSE(_newfun, _orgfun) \\\n");
    outfile.write("__attribute__((used)) static struct{ const void *newfun; const void *orgfun; } _interpose_##_orgfun \\\n");
    outfile.write("__attribute__ ((section (\"__DATA,__interpose\"))) = { (const void *)(unsigned long)&_newfun, \\\n");
    outfile.write("                                                     (const void *)(unsigned long)&_orgfun }\n\n");
    outfile.write(osx_wrapdecl + "\n")
    outfile.write("void __attribute__ ((constructor)) wrapper_init(void)\n");
    outfile.write("{\n");
    outfile.write("}\n\n");
    outfile.write("void __attribute__ ((destructor)) wrapper_fini(void)\n");
    outfile.write("{\n");
    outfile.write("}\n\n");
    outfile.write(osx_wrapbody)
    outfile.write("#endif\n\n\n")

    outfile.write("#ifdef __linux__\n")
    outfile.write(lin_wrapdecl + "\n")
    outfile.write("void __attribute__ ((constructor)) wrapper_init(void)\n");
    outfile.write("{\n");
    outfile.write(lin_wrapinit)
    outfile.write("  inited = 1;\n")
    outfile.write("}\n\n");
    outfile.write("void __attribute__ ((destructor)) wrapper_fini(void)\n");
    outfile.write("{\n");
    outfile.write("  inited = 0;\n")
    outfile.write("}\n\n");
    outfile.write(lin_wrapbody)
    outfile.write("#endif\n\n")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        genwrapper(sys.argv[1], sys.argv[2])
    else:
        print("usage: gendlwrap.py <input h-file> <output c-file>")
        exit

