#!/bin/bash

#source /opt/Xilinx/Vivado/xxx/settings64.sh
source setenv.sh

top=$PWD

cd ./sdk_workspace/vitis_proj/_ide/bitstream
updatemem -meminfo system_wrapper.mmi -data ../../Debug/vitis_proj.elf -proc system_i/microblaze_0 -bit system_wrapper.bit -out download.bit -force
mkdir -p ../flash
echo -e "the_ROM_image:\n{\n$PWD/download.bit\n}\n" > ../flash/bootimage.bif
bootgen -arch fpga -image ../flash/bootimage.bif -w -o ../flash/BOOT.bin -interface spi 

cd $top

#cd -
mkdir -p output
rm -rf output/*
echo -e "\n"
echo -e "\033[42;31m clear output done!!! \033[0m"

cp ./sdk_workspace/vitis_proj/_ide/flash/BOOT.bin ./output/app.bin
if [ $? != 0 ]
then
    echo -e "\n"
    echo -e "\033[41;36m cp app.bin fail!!! Press any key to exit \033[0m"
    exit 1
fi

du -b ./output/app.bin | awk '{print substr($1,$2)}' | xargs -I {} printf "%x\n" {} > ./output/app.txt
if [ $? != 0 ]
then
    echo -e "\n"
    echo -e "\033[41;36m calc size fail!!! Press any key to exit \033[0m"
    exit 1
fi

# function pause(){
# #	read -p "$*"
#     printf "$*\n"
# #    read -n 1
#     read
# #    printf "\n"
# }
# pause 'Press any key to continue'
function pause(){
    read -n 1
}
pause
