#! /bin/bash
FILES=$(ls test/*.out)
BIN=ft_nm
make
for f in $FILES;do
    
    diff -i <(./$BIN $f | cat -e) <(otool -t $f | cat -e)
done