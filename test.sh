#! /bin/bash
FILES=$(find bin -type f )
BIN=ft_nm
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)" 
COLOR_RED="$(tput setaf 3)"
make
for f in $FILES;do
    otool -t "${f}" >/dev/null
    if [ "$?" == "0" ]; then
        diff -i <(./$BIN "${f}" 2>/dev/null| cat -e) <(otool -t "${f}" 2>/dev/null| cat -e) >/dev/null
        if [ "$?" == "0" ];then 
            echo -e "diff ${COLOR_GREEN}OK" $COLOR_REST "-> ${f}"
        else
            echo -e "diff ${COLOR_RED}K0" $COLOR_REST "-> ${f}"
            if [ "${1}" == "-v" ]; then
                diff -i <(./$BIN "${f}" ) <(otool -t "${f}" ) 
            fi
        fi    
    fi
done

FILES=$(find /usr/bin -type f )
for f in $FILES;do
    otool -t "${f}" &>/dev/null
    if [ "$?" == "0" ]; then
        diff -i <(./$BIN "${f}" 2>/dev/null| cat -e) <(otool -t "${f}" 2>/dev/null| cat -e) >/dev/null
        if [ "$?" == "0" ];then 
            echo -e "diff ${COLOR_GREEN}OK" $COLOR_REST "-> ${f}"
        else
            echo -e "diff ${COLOR_RED}K0" $COLOR_REST "-> ${f}"
            if [ "${1}" == "-v" ]; then
                diff -i <(./$BIN "${f}" ) <(otool -t "${f}" ) 
            fi
        fi    
    fi
done
#diff <(./ft_nm /bin/*) <(otool -t /bin/*) 