#!/usr/bin/env bash
# usage: TBX_REF=/path/to/toybox host_run.sh test.t  -> transcript of each line + its output, using reference toybox
set -u
T=$(readlink -f "$1")
REF="${TBX_REF:?set TBX_REF to a host toybox built from the pinned upstream commit (make defconfig && make)}"
W=$(mktemp -d); B=$(mktemp -d)
for c in $($REF); do ln -s $REF $B/$c; done
cd $W
export PATH=$B:/usr/bin:/bin
RC=0
while IFS= read -r line; do
  printf "\001%s\n" "$line"
  (exit $RC)
  eval "$line" 2>&1
  RC=$?
done < "$T"
rm -rf $W $B
