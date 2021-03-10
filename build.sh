#/bin/bash!

#choose here wether to boot from an sd card or from the network, by uncommenting the option you want.
# boot_mode=network
boot_mode=sd_card

#choose your version of raspberry pi, by uncommenting the version you want. only the options below
#are supported for the moment.
pi_version=3b
# pi_version=3b+

#set serial number of your pi. is only necessary when booting from network.
pi_serial=8f3e7b47

#add here the paths to given files and directories.
#bin folder of the compiler.
compiler_directory=$(dirname $PWD)/shared_resources/compilers/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-elf-with_enum_bitfields/bin
#folder containing raspberry pi boot files. should normally not be changed.
binary_blob_directory=source_code/pi_3/boot/binary_blob
#path of image file. should normally not be changed.
image_directory=compiled_code/kernel8.img
#path to copy image and boot files to. when booting from sd card only the sd_directory should be given.
#when booting from network with a pi 3b+ no sd card is needed, so only tftp_directory should be given.
#it should point to where your sd card is mounted in the file system. when booting from network you
#should also provide the directory of the root folder of your tftp server (see readme.md).
tftp_directory=/home/tsrapnik/tftp_root
sd_directory=/media/tsrapnik/odessa

#actual script. no need to change anything here.
if [ "$1" = "boot" ]
then
    if [ "$boot_mode" = "sd_card" ]
    then
        #copy boot files to sd card.
        cp $binary_blob_directory/bootcode.bin $sd_directory
        cp $binary_blob_directory/fixup.dat $sd_directory
        cp $binary_blob_directory/start.elf $sd_directory
        if [ $? -eq 0 ]
        then
            echo "copy boot files to sd card."
        fi
    elif [ "$boot_mode" = "network" ]
    then
        if [ "$pi_version" = "3b" ]
        then
            #for 3b we need to have an sd card with only bootcode.bin for network booting. for 3b+ no sd
            #card is needed anymore.
            rm $sd_directory/fixup.dat 2> /dev/null
            rm $sd_directory/start.elf 2> /dev/null
            rm $sd_directory/kernel8.img 2> /dev/null
            cp $binary_blob_directory/bootcode.bin $sd_directory
            if [ $? -eq 0 ]
            then
                echo "copy boot files to sd card."
            fi
        fi

        #all other boot files should go to the tftp server.
        mkdir $tftp_directory/$pi_serial 2> /dev/null
        cp $binary_blob_directory/bootcode.bin $tftp_directory/$pi_serial
        cp $binary_blob_directory/fixup.dat $tftp_directory/$pi_serial
        cp $binary_blob_directory/start.elf $tftp_directory/$pi_serial
        #next files are not strictly necessary, but speed up the boot process.
        cp $binary_blob_directory/autoboot.txt $tftp_directory/$pi_serial
        cp $binary_blob_directory/bootcfg.txt $tftp_directory/$pi_serial
        cp $binary_blob_directory/config.txt $tftp_directory/$pi_serial
        cp $binary_blob_directory/recovery.cmdline $tftp_directory/$pi_serial
        cp $binary_blob_directory/recovery.elf $tftp_directory/$pi_serial
        if [ $? -eq 0 ]
        then
            echo "copy boot files to tftp server."
        fi
    fi
elif [ "$1" = "clean" ]
then
    make clean
else
    export PATH=$PATH:$compiler_directory
    make

    if [ "$boot_mode" = "sd_card" ]
    then
        cp $image_directory $sd_directory
        if [ $? -eq 0 ]
        then
            echo "copy image to sd card."
        fi
    elif [ "$boot_mode" = "network" ]
    then
        cp $image_directory $tftp_directory/$pi_serial
        if [ $? -eq 0 ]
        then
            echo "copy image to tftp server"
        fi
    fi
fi