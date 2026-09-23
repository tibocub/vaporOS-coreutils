#!/usr/bin/env python3
"""usage: sim_run.py test.t [seconds-per-command] -> transcript in the same format as host_run.sh"""
import sys, re
sys.path.insert(0, __import__('os').path.dirname(__import__('os').path.abspath(__file__)))
from simrun import run
lines = [l.rstrip("\n") for l in open(sys.argv[1]) if l.strip()]
per = float(sys.argv[2]) if len(sys.argv) > 2 else 0.7
import re as _re
MAPPED = lines
cmds = ["vaporshell", "cd /tmp", "rm -f w.txt big2.txt f.txt n.txt nonl.txt empty.txt c.txt s.txt u1.txt big.txt b1.txt b2.txt o.txt t1.txt t2.txt t3.txt sorted.txt"] + MAPPED
out = run(cmds, boot=4.0, per=per, tail=3.0)
out = re.sub(r'\x1b\[[0-9;]*[A-Za-z]', '', out).replace('\r', '')
out = out.replace("vaporshell$ ", "\x01")
blocks = out.split("\x01")
res = []
started = False
for b in blocks:
    if not b: continue
    b = b.replace("tbx wc ", "wc ", 1) if b.startswith("tbx wc ") else b
    first = b.split("\n", 1)[0]
    if first == lines[0]: started = True
    if started: res.append("\x01" + b)
sys.stdout.write("".join(res))
