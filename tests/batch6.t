printf "b 2\na 1\nc 3\n" > f6.txt
mkdir d6
mkdir d6/sub
printf "x\n" > d6/a.txt
printf "y\n" > d6/sub/b.txt
printf "z\n" > d6/sub/c.log
find d6 | sort
find d6 -type f | sort
find d6 -type d | sort
find d6 -name "*.txt" | sort
find d6 -name "*.log"
find d6 -maxdepth 1 | sort
find d6 -mindepth 2 | sort
find d6 -depth | sort
find d6 -type f -name "*.txt" | wc -l
find d6 -type f -printf "%f\n" | sort
find d6 -type f -exec echo got {} \; | sort
find d6 -type f -exec cat {} + | sort
find d6 -name "*.txt" -exec grep -l x {} \; | sort
find d6 -name nomatch -exec echo hi \;
find d6 -name a.txt -exec false {} \;
echo rc=$?
find d6 -name a.txt -exec nosuchcmd {} \;
echo rc=$?
find nosuchdir
echo rc=$?
find d6 -type f | sort | xargs cat
printf "a\nb\nc\n" | xargs echo
printf "a b c\n" | xargs -n1 echo
printf "a b c d\n" | xargs -n2 echo
printf "a\0b\0" | xargs -0 echo
echo hello | xargs echo pre
printf "" | xargs -r echo none
printf "" | xargs echo empty
printf "f6.txt\n" | xargs cat
xargs -a f6.txt echo
echo a | xargs nosuchcmd
echo rc=$?
echo a | xargs false
echo rc=$?
env FOO=bar printenv FOO
env -i printenv FOO
echo rc=$?
env A=1 env B=2 printenv B
env -i A=1 B=2
env nosuchcmd
echo rc=$?
printf "a\nb\nc\n" > t1
tac t1
echo abc | rev
printf "one two\nx\n" | rev
printf "1\n2\n3\n" > pa
printf "a\nb\nc\n" > pb
paste pa pb
paste -d, pa pb
paste -s pa
paste -s -d: pa
printf "a\nb\nc\n" > c1
printf "b\nc\nd\n" > c2
comm c1 c2
comm -12 c1 c2
comm -23 c1 c2
comm -13 c1 c2
nl t1
nl -ba t1
nl -w3 -s: t1
printf "a\tb\n" | expand
printf "a\tb\n" | expand -t4
printf "a\tb\tc\n" | expand -t 2,5
echo abcdefghij | fold -w 4
echo "the quick brown fox" | fold -s -w 8
printf "hello\n" | od -c
printf "hello\n" | od -An -tx1
printf "hello\n" | od -t d1
printf "hello\n" | xxd
printf "hello" | xxd -p
printf "hello" | xxd -p | xxd -r -p
printf "abcdefgh" > tr.txt
truncate -s 3 tr.txt
cat tr.txt
truncate -s 6 tr.txt
wc -c tr.txt
touch ul.txt
unlink ul.txt
test -e ul.txt
echo rc=$?
printf "1\n2\n3\n4\n5\n" > sp.txt
split -l 2 sp.txt sp_
cat sp_aa
cat sp_ac
split -b 4 sp.txt spb_
cat spb_aa
mktemp mkt.XXXXXX | cut -c1-4
mktemp -d dt.XXXXXX | cut -c1-3
printf hello | md5sum
printf "" | md5sum
printf hello | sha1sum
printf hello | sha256sum
printf hello | sha512sum
md5sum f6.txt > sums.txt
md5sum -c sums.txt
find d6 -name "*.log" -delete
find d6 -type f | sort
