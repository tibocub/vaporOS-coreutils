#!/usr/bin/env python3
"""Drive the NuttX sim over a pty: simrun.py 'cmd1' 'cmd2' ...  (or - to read cmds from stdin).
Set NUTTX_DIR to the nuttx checkout if it is not ../../nuttx relative to this file."""
import os, pty, sys, time, select, signal
def run(cmds, boot=4.0, per=0.6, tail=1.5, binary=None):
    binary = binary or os.path.join(os.environ.get("NUTTX_DIR", os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "nuttx")), "nuttx")
    binary = os.path.abspath(binary)
    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(os.path.dirname(binary))
        os.execv(binary, [binary])
    out = b""
    def drain(t):
        nonlocal out
        end = time.time() + t
        while time.time() < end:
            r,_,_ = select.select([fd],[],[],0.1)
            if r:
                try: d = os.read(fd, 65536)
                except OSError: return False
                if not d: return False
                out += d
        return True
    drain(boot)
    for c in cmds:
        os.write(fd, c.encode() + b"\n")
        drain(per)
    drain(tail)
    try:
        os.kill(pid, signal.SIGKILL); os.waitpid(pid, 0)
    except Exception: pass
    return out.decode(errors="replace")
if __name__ == "__main__":
    cmds = sys.argv[1:]
    if cmds == ["-"]: cmds = [l.rstrip("\n") for l in sys.stdin if l.strip()]
    print(run(cmds))
