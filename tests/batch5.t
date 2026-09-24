printf "apple pie\nbanana split\ncherry tart\napple crumble\nBanana bread\n" > g.txt
printf "one two\nthree four\nfive six\n" > g2.txt
grep apple g.txt
grep -n apple g.txt
grep -c apple g.txt
grep -i banana g.txt
grep -v apple g.txt
grep -w tart g.txt
grep -o "a[a-z]*e" g.txt
grep -E "^(apple|cherry)" g.txt
grep -F "pie" g.txt
grep -e apple -e cherry g.txt
grep -l apple g.txt g2.txt
grep -L apple g.txt g2.txt
grep -H tart g.txt
grep -h apple g.txt g.txt
grep -x "cherry tart" g.txt
grep -m1 apple g.txt
grep -b tart g.txt
grep -A1 cherry g.txt
grep -B1 cherry g.txt
grep -C1 cherry g.txt
grep -q apple g.txt
echo rc=$?
grep zzz g.txt
echo rc=$?
grep apple nosuchfile
echo rc=$?
grep -s apple nosuchfile
echo rc=$?
grep apple < g.txt
egrep "pie|tart" g.txt
fgrep "a.b" g.txt
echo rc=$?
mkdir gd
cp g.txt gd/a.txt
grep -r apple gd
grep -rl tart gd
cat g.txt | grep -c a
grep -c a g.txt | cat
sed s/apple/APPLE/ g.txt
sed -n 2p g.txt
sed -n 2,3p g.txt
sed 2d g.txt
sed '$d' g.txt
sed -n '/cherry/p' g.txt
sed 's/a/A/g' g.txt
sed -e 's/apple/X/' -e 's/pie/Y/' g.txt
sed -E 's/(a)(p+)/\2\1/' g.txt
sed 's/[aeiou]//2' g.txt
sed = g2.txt
sed 'y/abc/xyz/' g.txt
sed '1!G;h;$!d' g2.txt
sed -n '$=' g.txt
sed '1a added' g2.txt
sed '2i inserted' g2.txt
sed '2c changed' g2.txt
sed -n '/one/,/three/p' g2.txt
sed 's/x/y/' nosuchfile
echo rc=$?
sed -i s/one/ONE/ g2.txt
cat g2.txt
cat g.txt | sed s/pie/PIE/
echo hello | tr a-z A-Z
echo hello world | tr -d l
echo aabbcc | tr -s ab
printf hello | tr -c e X
echo
echo a1b2 | tr -d '[:digit:]'
echo AbC | tr '[:upper:]' '[:lower:]'
printf 'a\tb\n' | tr '\t' ' '
echo hello | tr -s l
echo abc | tr abc xy
ln g.txt hard.txt
cat hard.txt
echo rc=$?
ln -s g.txt sym.txt
echo rc=$?
ln g.txt hard.txt
echo rc=$?
ln -f g.txt hard.txt
echo rc=$?
cmp g.txt g.txt
echo rc=$?
cmp g.txt g2.txt
echo rc=$?
cmp -s g.txt g2.txt
echo rc=$?
cmp -l g.txt g2.txt
cmp -n 3 g.txt g2.txt
echo rc=$?
cmp g.txt nosuchfile
echo rc=$?
expr 2 + 3
expr 10 / 3
expr 7 % 4
expr 2 \* 3
expr 5 - 8
expr abc : 'a.*'
expr abc : 'a\(b\)c'
expr 3 \> 2
expr 1 = 2
expr abc \| def
expr 0 \& 1
echo rc=$?
expr foo
echo rc=$?
expr 1 / 0
echo rc=$?
date -u -d @0 +%Y-%m-%d
date -u -d @86400 '+%H:%M:%S %j %a %b'
date -u -d @1700000000
date -u -d @1700000000 +%s
date -u -d @1700000000 -I
date -u -d '2001-02-03 04:05:06' +%s
date -u -d @1700000000 +%A%%
touch cm.txt
chmod 600 cm.txt
ls -l cm.txt | cut -c1-10
chmod u+x cm.txt
ls -l cm.txt | cut -c1-10
chmod a+rwx cm.txt
ls -l cm.txt | cut -c1-10
chmod go-w,u=r cm.txt
ls -l cm.txt | cut -c1-10
chmod 644 nosuchfile
echo rc=$?
chmod -R 755 gd
ls -l gd | cut -c1-10
chmod
echo rc=$?
date -u -d @1700000000 +%Z
echo abc | tr -c a X
printf 'caf\303\251\n' | tr -d '\303' | wc -c
cp g.txt g.txt
echo rc=$?
cp g.txt cp-copy.txt
cmp g.txt cp-copy.txt
echo rc=$?
