import sys
def blocks(p): return [b for b in open(p,errors='replace').read().split("\x01") if b]
h=blocks(sys.argv[1]); s=blocks(sys.argv[2])
print("blocks host/sim:",len(h),len(s))
bad=0
for i,b in enumerate(h):
    sb=s[i] if i<len(s) else "<missing>\n"
    if sb!=b and not (i==len(h)-1 and sb.rstrip()==b.rstrip()):
        bad+=1
        hb=b.split("\n",1); sbb=sb.split("\n",1)
        print("=== DIFF:",hb[0]); print(" host:",repr(hb[1][:160] if len(hb)>1 else "")); print(" sim :",repr(sbb[1][:160] if len(sbb)>1 else ""))
print("diffs:",bad)
