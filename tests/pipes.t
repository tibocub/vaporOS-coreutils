printf "b 2\na 1\nc 3\na 1\n" > f.txt
printf "one\ntwo\nthree\nfour\nfive\nsix\n" > n.txt
yes | head -n 3
yes foo bar | head -n 2
cat n.txt | head -n 3
cat n.txt | tail -n 2
cat n.txt | tail -n +5
cat n.txt | wc -l
cat f.txt | sort
sort f.txt | uniq -c
cat f.txt | cut -d" " -f2
echo abc | tee t1.txt
cat t1.txt
