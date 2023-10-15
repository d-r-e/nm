#!/bin/bash

set -e

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

touch ./test/bin/nopermission
chmod 000 ./test/bin/nopermission

make

check_output() {
    local file=$1

    
    $FT_NM $file > ft_nm_output 2>/dev/null || true  
    $NM $file > nm_output 2>/dev/null || true

    if diff -q nm_output ft_nm_output >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff nm_output ft_nm_output
    fi
}

# echo "Checking bad files..."
# for file in ${BAD_FILES[@]}; do
#     check_output $file
# done

# echo "Checking ./test/bin folder..."
# for binary in $(find ./test/bin -type f); do
#     check_output $binary
# done

# echo "Checking specified binaries..."
# for binary in "/bin/ls"; do
#     check_output $binary
# done

# echo "Checking object files inside libft"
# for object in $(find ./libft -name "*.o"); do
#     check_output $object
# done


echo "Checking self..."
check_output $FT_NM



# echo "Checking /bin/ folder..."
# for binary in $(find /bin /usr/bin -type f); do
#     check_output $binary
# done


# # echo "Checking system objects..."
# # for object in $(find /usr/lib /lib -name "*.o" | head -10); do
# #     check_output $object
# done


rm -f ft_nm_output nm_output
rm test/bin/nopermission