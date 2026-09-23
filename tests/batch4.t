printf "b 2\na 1\nc 3\na 1\n" > f.txt
printf "one\ntwo\nthree\nfour\nfive\nsix\nseven\neight\nnine\nten\neleven\ntwelve\n" > n.txt
printf "no newline at end" > nonl.txt
printf "" > empty.txt
printf "x:1:z\ny:22:w\nnofield\n:3:\n" > c.txt
printf "b\nB\na\nA\nb\n10\n9\n2\n" > s.txt
printf "aa 1\nAA 2\nab 3\nab 4\nb 5\n" > u1.txt
cat n.txt n.txt n.txt n.txt > b1.txt
cat b1.txt b1.txt b1.txt b1.txt > b2.txt
cat b2.txt b2.txt b2.txt b2.txt > big.txt
head -n 2 n.txt
head -n 0 n.txt
head -n 20 n.txt
head -c 7 n.txt
head -n 1 f.txt n.txt
head -n 2 nonl.txt
head -n -10 n.txt
head empty.txt
head -n 3 < n.txt
head nosuchfile
echo rc=$?
tail -n 2 n.txt
tail -n +11 n.txt
tail -n 0 n.txt
tail -c 6 n.txt
tail -c +60 n.txt
tail -n 1 nonl.txt
tail -n 2 f.txt n.txt
tail -3 n.txt
tail -n 2 < n.txt
tail -n +12 < n.txt
tail -n 2 big.txt
tail -n 2 < big.txt
tail -n 3071 big.txt > o.txt
wc -l o.txt
tail -c 4 big.txt
tail empty.txt
tail nosuchfile
echo rc=$?
wc f.txt
wc -l f.txt
wc -w f.txt
wc -c f.txt
wc -lw n.txt
wc f.txt n.txt
wc empty.txt
wc nonl.txt
wc -l < n.txt
wc -l big.txt
wc nosuchfile
echo rc=$?
tee t1.txt < f.txt
cat t1.txt
tee -a t1.txt < c.txt
cat t1.txt
tee t2.txt t3.txt < s.txt
cat t2.txt t3.txt
cut -c1-3 n.txt
cut -c2 n.txt
cut -b 1,3 f.txt
cut -c3- f.txt
cut -c-2 f.txt
cut -d: -f1 c.txt
cut -d: -f2 c.txt
cut -d: -f1,3 c.txt
cut -d: -f2 -s c.txt
cut -d: -f2- c.txt
cut -f1 f.txt
cut -d: -f3 < c.txt
cut -d: -f2 -n c.txt
cut -F 1 f.txt
cut -c1 nosuchfile
echo rc=$?
uniq f.txt
uniq -c f.txt
uniq -d f.txt
uniq -u f.txt
uniq -i u1.txt
uniq -f1 u1.txt
uniq -s1 u1.txt
uniq -w1 u1.txt
uniq -c empty.txt
uniq < f.txt
sort f.txt
sort -r f.txt
sort s.txt
sort -f s.txt
sort -n s.txt
sort -nr s.txt
sort -u f.txt
sort -k2 f.txt
sort -k2,2nr f.txt
sort -t: -k2n c.txt
sort -b f.txt
sort -o sorted.txt f.txt
cat sorted.txt
sort -c f.txt
echo rc=$?
sort -c sorted.txt
echo rc=$?
sort -s -k1,1 u1.txt
sort n.txt f.txt
sort < f.txt
sort empty.txt
sort nosuchfile
echo rc=$?
sort big.txt > o.txt
uniq -c o.txt
