# Regression tests: host toybox vs the vaporOS sim

Each `*.t` file is a list of shell commands, one per line. The same file
is run twice and the transcripts compared:

- on the host, with a reference toybox built from the pinned upstream
  commit (`make defconfig && make` in a toybox checkout at the commit named
  in the Makefile), and
- on the NuttX sim, typed into vaporshell over a pty.

```
TBX_REF=/path/to/toybox/toybox bash tests/host_run.sh tests/batch4.t > /tmp/host.out
python3 tests/sim_run.py tests/batch4.t 0.6 > /tmp/sim.out      # NUTTX_DIR=... if needed
python3 tests/cmp.py /tmp/host.out /tmp/sim.out
```

Files: `batch4.t` (head tail wc tee cut uniq sort, no pipelines) `batch6.t` (find xargs env and text utilities; no known differences), `batch5.t` (grep egrep fgrep sed tr ln cmp expr date chmod; `ln`/`chmod` differ, see docs/porting-notes.md), and
`pipes.t` (two tbx tasks running concurrently). Add a `.t` per batch.

Known, expected difference: the reference build prints `(see "head --help")`
after some usage errors because it has help text compiled in; ours doesn't.
`tail -f` isn't covered (it never exits): try it by hand, see below.

```
tail -f f.txt &        # then: echo more >> f.txt   -- should appear within ~250 ms
```

Uses only what upstream toybox itself does as its oracle, so this catches
porting regressions, not deviations from POSIX that toybox has too.
