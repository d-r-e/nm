#! /bin/bash
FILES=$(ls /bin/)
BIN=ft_nm
make
for f in $FILES;do
    diff -i <(./$BIN $f 2>/dev/null| cat -e) <(otool -t $f 2>/dev/null| cat -e)
done