#!/usr/bin/env bash
# scripts/regen.sh /path/to/toybox-checkout
#
# Regenerates generated/{config,flags,globals,help,newtoys,tags}.h from
# scripts/toybox.config, using a checkout of upstream toybox
# (codeberg.org/landley/toybox) as the generator. Run this whenever
# scripts/toybox.config changes (i.e. whenever an applet is enabled),
# then commit scripts/toybox.config and generated/ together with the
# new toys/*/*.c and Makefile CSRCS lines.
#
# Nothing here touches the NuttX build: generated/ is checked in on
# purpose (see the Makefile's own comment on why). The checkout is only
# read -- everything happens in a temp copy, and the host `make` it runs
# also produces a native toybox binary you can use as a behavioural
# reference while testing a port (kept at $KEEP if you set it).
#
# To enable an applet: flip its line in scripts/toybox.config from
#   # CONFIG_FOO is not set   to   CONFIG_FOO=y
# and add its .c file (copied verbatim from upstream, same toys/<dir>/
# layout) to the Makefile's CSRCS.
set -euo pipefail

# Must match the commit named in the Makefile.
PIN=e76971b0a7919f13ce9c5eb4b194f93d8c8a9645

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TBX="${1:?usage: $0 /path/to/toybox-checkout}"

head="$(git -C "$TBX" rev-parse HEAD 2>/dev/null || true)"
if [ "$head" != "$PIN" ] && [ -z "${FORCE:-}" ]; then
  echo "toybox checkout is at '${head:-unknown}', expected $PIN" >&2
  echo "(git -C $TBX checkout $PIN, or set FORCE=1 to regenerate anyway)" >&2
  exit 1
fi

tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT

git -C "$TBX" archive HEAD | tar -x -C "$tmp"

# vaporOS-native applets (not in upstream toybox) are picked up by the
# same generator as long as they follow toybox's own file header format.
if [ -d "$HERE/toys/vapor" ]; then
  mkdir -p "$tmp/toys/vapor"
  cp "$HERE"/toys/vapor/*.c "$tmp/toys/vapor/"
fi

cp "$HERE/scripts/toybox.config" "$tmp/.config"
(cd "$tmp" && KCONFIG_CONFIG=.config make >"$tmp/make.log" 2>&1) || {
  tail -20 "$tmp/make.log" >&2
  exit 1
}

for f in config flags globals help newtoys tags; do
  cp "$tmp/generated/$f.h" "$HERE/generated/$f.h"
done

[ -n "${KEEP:-}" ] && cp "$tmp/toybox" "$KEEP"

git -C "$HERE" diff --stat -- generated scripts/toybox.config 2>/dev/null || true
