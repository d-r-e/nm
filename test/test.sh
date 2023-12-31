#!/bin/bash

# set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'  

FT_NM=./ft_nm
NM=nm

# if there is an argument, only test that file
# otherwise, test all files

BAD_FILES=(
    /dev/null
    /etc/passwd
    Makefile
    /dev/random
    ./test/bin/nopermission
)

PROBLEMATIC=(
    /usr/lib/gcc/x86_64-linux-gnu/9/crtoffloadbegin.o
    /usr/lib/x86_64-linux-gnu/Scrt1.o
)

touch ./test/bin/nopermission
chmod 000 ./test/bin/nopermission

make

check_output() {
    local file=$1


    if diff -q <($FT_NM $file) <($NM $file) >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -c --color <($FT_NM $file) <($NM $file)
    fi
}

if [ $1 ]; then
    diff -y --color <(./ft_nm $1) <(nm $1)
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}[OK]: $1${RESET}"
    else
        echo -e "${RED}Difference found in: $1${RESET}"
    fi
    exit 0
fi

check_output_multi(){
    $FT_NM $@ > /tmp/ft_nm_output 2>/dev/null || true  
    $NM $@ > /tmp/nm_output 2>/dev/null || true

    if diff -q /tmp/nm_output /tmp/ft_nm_output >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -y --suppress-common-lines /tmp/ft_nm_output /tmp/nm_output
    fi
}

if [ $1 ]; then
    check_output $1
    exit 0
fi

echo checking problematic...
for file in ${PROBLEMATIC[@]}; do
    check_output $file
done



echo "Checking bad files..."
for file in ${BAD_FILES[@]}; do
    check_output $file
done
chmod 777 ./test/bin/nopermission
rm -f ./test/bin/nopermission

echo "Checking in /usr/lib/debug..."


echo "Checking object files inside libft"
for object in $(find ./libft -name "*.o"); do
    check_output $object
done

echo "Checking ./test/bin folder..."
for binary in $(find ./test/bin -type f); do
    check_output $binary
done

echo "Checking specified binaries..."
for binary in "/bin/ls"; do
    check_output $binary
done
echo "Checking self..."
check_output $FT_NM

check_output /usr/lib/x86_64-linux-gnu/Scrt1.o

# check_output_multi $(ls /bin/*)


# exit 0

# echo "Checking system objects..."
# for object in $(find /usr/lib /lib -name "*.*o" ); do
#     check_output $object
# done

echo "Checking system binaries..."
for binary in $(find /bin /usr/bin -type f ); do
    echo -n $binary " "
    check_output $binary
done

echo "Checking objects in test/lib..."
for object in $(find ./test/lib -name "*.o"); do
    check_output $object
done

echo "checking all files in /fedora/**"
for binary in $(find /fedora -type f); do
    check_output $binary
done;

# echo "checking all files in vagrant/bin/bin/**"
# for binary in $(find ./vagrant/bin/bin -type f); do
#     check_output $binary
# done;

if [ ! -f ./test/bin/kompose ]; then
    curl -L https://github.com/kubernetes/kompose/releases/download/v1.31.2/kompose-linux-amd64 -o ./test/bin/kompose
fi
check_output ./test/bin/kompose


rm -f /tmp/ft_nm_output /tmp/nm_output
rm -f test/bin/nopermission

check_output_multi $(find ./test/bin -type f  )