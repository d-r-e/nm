#! /bin/bash
BIN=ft_otool

if [ ! -f $BIN ];then
    echo "${BIN} not found."
    exit 1
fi

make
LEN=10
if [ "${1}" == "--long" ];then
    LEN=100
fi

FILES=$(find /usr/bin/** -type f 2>/dev/null | head -n $LEN )
FILES_EXEC=$(find /bin/** -type f 2>/dev/null | head -n $LEN )
FILES_O=$(find ~/.local/ -name "*.o" 2>/dev/null | head -n $LEN )
FILES_O+=$(find . -name "*.o" 2>/dev/null )
FILES_SO=$(find /usr/lib/** -name "*.so" 2>/dev/null | head -n $LEN )
FILES_DYLIB=$(find /usr/lib/** -name "*.dylib" 2>/dev/null | head -n $LEN )
OVERKILL="/usr/lib/libnetsnmp.5.2.1.dylib /usr/lib/libnetsnmp.5.dylib"
FILES_A=$(find . -name "*.a" | head -n $LEN)
COLOR_REST="$(tput sgr0)"
COLOR_GREEN="$(tput setaf 2)" 
COLOR_RED="$(tput setaf 1)"
COLOR_YELLOW="$(tput setaf 3)"

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
function test_files()
{
    local FILES=${@}
    if [ "$FILES" == "" ];then
        return
    fi
    for f in $FILES;do
        file "${f}" | head -n1| grep Mach | grep -v x86_64h &> /dev/null
        if [ "$?" == "0" ]; then
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
        if [ $(($LEN < 100)) ];then 
          sleep 0.1
        fi
    done
    if [ "$f" == "./libft/libft.a" ]; then
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
    echo "RESULT: $RESULT/$NFILES"
    [ "${RESULT}" == "${NFILES}" ] && success || failure
}
test_files $FILES_SO
test_files $FILES_DYLIB
test_files $FILES_O
test_files $FILES
test_files $FILES_EXEC

exit 0

#diff <(./ft_nm /bin/*) <(otool -t /bin/*) test_files $FILES_EXEC
