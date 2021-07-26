#! /bin/bash
FILES=$(find bin -type f )
BIN=ft_otool
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)" 
COLOR_RED="$(tput setaf 1)"
COLOR_YELLOW="$(tput setaf 3)"
make
RESULT=$(( 0 ))
NFILES=$(($(find bin -type f  | wc -l)))

function echo_green() {
    echo $COLOR_GREEN $1 $COLOR_REST
}

for f in $FILES;do
    otool -t "${f}" >/dev/null
    if [ "$?" == "0" ]; then
        diff -i <(./$BIN "${f}" 2>/dev/null| cat -e) <(otool -t "${f}" 2>/dev/null| cat -e) >/dev/null
        if [ "$?" == "0" ];then 
            echo -e "diff ${COLOR_GREEN}OK" $COLOR_REST "-> ${f}"
            RESULT=$(($RESULT + 1))
        else
            echo -e "diff ${COLOR_RED}K0" $COLOR_REST "-> ${f}"
            if [ "${1}" == "-v" ]; then
                diff -i <(./$BIN "${f}" ) <(otool -t "${f}" ) 
            fi
        fi
    fi
done
echo_green "RESULT: $RESULT/$NFILES"
if [ "${RESULT}" == "${NFILES}" ];then
fi
exit 0
FILES=$(find /usr/bin -type f )
for f in $FILES;do
    otool -t "${f}" &>/dev/null
    if [ "$?" == "0" ]; then
        diff -i <(./$BIN "${f}" 2>/dev/null| cat -e) <(otool -t "${f}" 2>/dev/null| cat -e) >/dev/null
        if [ "$?" == "0" ];then 
            echo -e "diff ${COLOR_GREEN}OK" $COLOR_REST "-> ${f}"
        else
            echo -e "diff ${COLOR_RED}K0" ${COLOR_YELLOW} "-> ${f}" $COLOR_REST
            if [ "${1}" == "-v" ]; then
                diff -i <(./$BIN "${f}" ) <(otool -t "${f}" ) 
            fi
        fi    
    fi
done
#diff <(./ft_nm /bin/*) <(otool -t /bin/*) 