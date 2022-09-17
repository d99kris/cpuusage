#!/usr/bin/env bash

# Copyright (C) 2022 Kristofer Berggren
# All rights reserved.
#
# cpuusage is distributed under the BSD 3-Clause license, see LICENSE for
# details.

TMPCATAPULT="/tmp/cpuusage-catapult"

prereq()
{
  if [[ ! -d "${TMPCATAPULT}" ]]; then
    git clone https://chromium.googlesource.com/catapult ${TMPCATAPULT} || exit 1
  else
    echo "using existing clone in ${TMPCATAPULT}"
  fi

  if [[ ! -d "build" ]]; then
    ./make.sh build || exit 1
  else
    echo "using existing build in ./build"
  fi
}

update_catapult()
{
  pushd ./build/share/cpuusage/catapult/ > /dev/null && \
  rm -rf ./* && \
  cp ${TMPCATAPULT}/LICENSE LICENSE && \
  cp -a ${TMPCATAPULT}/tracing tracing && \
  rm -rf tracing/test_data && \
  mkdir common && \
  cp -a ${TMPCATAPULT}/common/py_vulcanize common/ && \
  mkdir third_party && \
  cp -a ${TMPCATAPULT}/third_party/beautifulsoup4-4.9.3 third_party/ && \
  cp -a ${TMPCATAPULT}/third_party/polymer third_party/ && \
  popd > /dev/null && \
  echo "updated catapult module" || exit 1
}

package_catapult()
{
  pushd ./build > /dev/null && \
  rm -f catapult.tar.gz && \
  tar -czvf catapult.tar.gz share/cpuusage/catapult && \
  popd > /dev/null && \
  cp ./build/catapult.tar.gz ./ext/catapult.tar.gz && \
  rm -f ./build/catapult.tar.gz && \
  echo "packaged catapult module" || exit 1
}

test_catapult()
{
  SCRIPT="./build/share/cpuusage/catapult/tracing/bin/trace2html"
  TESTJSON="./tests/ex.json"
  TMPHTML="./build/sanity_test.html"
  python3 ${SCRIPT} ${TESTJSON} --output=${TMPHTML} > /dev/null && \
  rm "${TMPHTML}" && \
  echo "sanity test passed" || exit 1
}

test_all()
{
  ./make.sh tests && \
  echo "all tests passed" || exit 1
}

cleanup()
{
  rm -rf build && \
  rm -rf "${TMPCATAPULT}" && \
  echo "cleanup complete" || exit 1
}

prereq
update_catapult
test_catapult
test_all
update_catapult
package_catapult
#cleanup

echo "success"
exit 0
