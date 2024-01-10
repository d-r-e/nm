#!/bin/bash

# set -e

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'  

FT_NM=./ft_nm
NM=nm

BAD_FILES=(
    test/
    /dev/null
    /etc/passwd
    Makefile
)

PROBLEMATIC=(
    /usr/lib/x86_64-linux-gnu/Scrt1.o
    # /usr/lib/debug/.build-id/de/9995b90dca9031d340a386d5cc627ef84e6080.debug
)

declare -a FLAGS=()
FLAGS+=("-p")
FLAGS+=("-r")
FLAGS+=("-rp")
FLAGS+=("-u")
FLAGS+=("-pu")
FLAGS+=("-rpu")
FLAGS+=("-g")
FLAGS+=("-gu")
FLAGS+=("-rpgu")
FLAGS+=("-pgu")
FLAGS+=("-rgu")
CHECK_FLAGS=0
make

check_output() {
    local file=$1
    if diff -q <($FT_NM $file 2>&1) <($NM $file 2>&1) >/dev/null; then
        echo -n
        if file $file | grep "ELF" | grep -q "not stripped"; then
            echo -e "${GREEN}[OK]: $file${RESET}"
        else
            return 0
        fi
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -c5 --color <($FT_NM "$file") <($NM "$file")
        echo -e "${RED}Difference found in: $file${RESET}"

        exit 1
    fi

    
    # if [ $CHECK_FLAGS ]; then
    #     for flag in ${FLAGS[@]}; do
    #         if diff -q <( $FT_NM $flag "$file" 2>&1 ) <( $NM $flag "$file" 2>&1 ) >/dev/null; then
    #             echo -n
    #             echo -e "${GREEN}[OK]: $file ${flag}${RESET}"
    #         else
    #             echo -e "${RED}Difference found in: $file ${flag} ${RESET}"
    #             set -x
    #             diff -y --color <( $FT_NM $flag $file ) <( $NM $flag $file )
    #             exit 1
    #         fi
    #     done
    # fi
}

if [ $1 == "--extreme" ];then
    for file in $(find /bin /usr/bin /lib /usr/lib | file -f - | grep "ELF" | grep "not stripped" | cut -d ":" -f 1); do
        check_output "$file"
    done
    exit 0
fi

if [ $1 ]; then
    check_output "$1"
    exit 0
fi



check_output_multi(){
    if diff -q <(./ft_nm $@) <(nm $@) >/dev/null; then
        echo -n
        echo -e "${GREEN}[OK]: $file${RESET}"
    else
        echo -e "${RED}Difference found in: $file${RESET}"
        diff -y --suppress-common-lines --color <(./ft_nm $@) <(nm $@)
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

# for file in $(find ./test/badfiles -type f); do
#     check_output $file
# done



echo "Checking object files inside libft"
for object in $(find ./libft -name "*.o"); do
    check_output $object
done

echo "Checking object files inside /usr/libx32/"
for object in $(find /usr/lib*/*.*o ); do
    check_output $object
done

echo "Checking ./test/bin folder..."
for binary in $(find ./test/bin -type f); do
    check_output $binary
done

# echo "Checking /usr/lib/debug folder..."
# for binary in $(find /usr/lib/debug -type f); do
#     check_output $binary
# done

echo "Checking specified binaries..."
for binary in "/bin/ls"; do
    check_output $binary
done
echo "Checking self..."
check_output $FT_NM

check_output /usr/lib/x86_64-linux-gnu/Scrt1.o

echo "Checking system objects..."
find /usr/lib /lib -name "*.*o" | shuf | head -n 100 | while read -r object; do
    check_output "$object"
done


echo "Checking system binaries..."
find /bin /usr/bin -type f | shuf | head -n 200 | while read -r binary; do
    check_output "$binary"
done


echo "Checking objects in test/lib..."
for object in $(find ./test/lib -name "*.o"); do
    check_output "$object"
done

echo "Checking objects in test/lib..."
for object in $(find /usr/local/lib /usr/local/lib -name "*.o"); do
    check_output "$object"
done


if [ ! -f ./test/bin/kompose ]; then
    curl -L https://github.com/kubernetes/kompose/releases/download/v1.31.2/kompose-linux-amd64 -o ./test/bin/kompose
fi
check_output ./test/bin/kompose


rm -f /tmp/ft_nm_output /tmp/nm_output
rm -f test/bin/nopermission

