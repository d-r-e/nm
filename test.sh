#! /bin/bash
BIN=ft_otool

if [ ! -f $BIN ];then
    echo "${BIN} not found."
    exit 1
fi

FILES=$(find /bin -type f )
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)" 
COLOR_RED="$(tput setaf 1)"
COLOR_YELLOW="$(tput setaf 3)"
make
RESULT=$(( 0 ))
NFILES=$(( 0 ))

function echo_green() {
    echo -e ${COLOR_GREEN}$1${COLOR_REST}
}

function echo_red() {
    echo -e ${COLOR_RED}$1${COLOR_REST}
}

function success ()
{
    echo_green "-------\nSUCCESS\n-------"
}

function failure ()
{
    echo_red "-------"
    echo_red "FAILURE"
    echo_red "-------"
}

for f in $FILES;do
    otool -t "${f}" | grep "not an object" >/dev/null
    if [ "$?" != "0" ]; then
        NFILES=$(($NFILES + 1))
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
echo "RESULT: $RESULT/$NFILES"
[ "${RESULT}" == "${NFILES}" ] && success || failure

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