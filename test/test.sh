#!/bin/bash

# set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'  

FT_NM=./ft_nm
NM=nm

BAD_FILES=(
    /dev/null
    /etc/passwd
    Makefile
    /dev/random
    ./test/bin/nopermission
)

PROBLEMATIC=(
    /usr/lib/gcc/x86_64-linux-gnu/9/crtoffloadbegin.o

)

touch ./test/bin/nopermission
chmod 000 ./test/bin/nopermission

make

check_output() {
    local file=$1

    
    $FT_NM -p $file > ft_nm_output 2>/dev/null || true  
    $NM -p $file > nm_output 2>/dev/null || true

    if diff -q ft_nm_output nm_output >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -y  ft_nm_output nm_output
    fi
}

check_output_multi(){
    $FT_NM $@ > ft_nm_output 2>/dev/null || true  
    $NM $@ > nm_output 2>/dev/null || true

    if diff -q nm_output ft_nm_output >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -y  ft_nm_output nm_output
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

echo "Checking ./test/bin folder..."
for binary in $(find ./test/bin -type f); do
    check_output $binary
done

echo "Checking specified binaries..."
for binary in "/bin/ls"; do
    check_output $binary
done

echo "Checking object files inside libft"
for object in $(find ./libft -name "*.o"); do
    check_output $object
done

# check_output_multi $(ls /bin/*)

exit 0

echo "Checking self..."
check_output $FT_NM


echo "Checking system objects..."
for object in $(find /usr/lib /lib -name "*.*o" | head -1000); do
    check_output $object
done


rm -f ft_nm_output nm_output
rm test/bin/nopermission