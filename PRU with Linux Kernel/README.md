# PRU with Linux User Space 

- There are several models of Beaglebones. The most common is the Beaglebone Black which has 2/4G MMC of memory internally. By default Beaglebone Black comes with a last Debian version installed. The Beaglebone version of Debian can be found on the official page, [`Beaglebone.org`](https://beagleboard.org/latest-images). This version of debian already has the PRU compiler installed, several simple examples and with WEB IDE Cloud9 running on port 3000 of beaglebone (Cloud9 does not work with internet explore). 

- I recommend updating the MMC or creating an updated debian image on an SD card, as there have been recent changes to the Linux kernel. The new versions of the Linux kernel, no longer dynamically load the tree of your device using the slot file. So the images from BeagleBoard.org Debian now deal with Capes is that device tree overlays are now managed in u-boot. A configuration file, /boot/uEnv.txt, must therefore be edited to notify the bootloader that a specific overlay needs to be loaded. 

- If you want to boot with an SD card on Beaglebone-black you have to keep clicking the boot button on the board when you turn it on.

- Since almost all versions of Beaglebone have microSD slots, it is recommended to create a Linux SDK. This requires rebuilding Linux Embedded and creating an SD card. There is also a version of the Linux kernel provided by texas is based on the Yocto Project and already has the necessary tools to work with PRU's. The Remoteproc and rpmsg modules are enabled by default and included out of the box in the Linux Processor SDK. One advantage is being able to create a custom Linux version.

### Requirements
- If you want to build a linux version based on texas Yocto project, then it is necessary to install the [`Processor SDK Linux`](https://software-dl.ti.com/processor-sw/esd/PROCESSOR-SDK-LINUX-AM335X/latest/index_FDS.html). The Linux SDK installation folder contains the files and scripts needed for re-building the Linux kernel and create SD Card .

- Texas provides a [Wiki](https://software-dl.ti.com/processor-sdk-linux/esd/docs/06_03_00_106/linux/index.html), where it explains step by step to re-building the Linux kernel and create SD Card.

### ARM <--> PRU interaction via remoteproc and rpmsg Linux drivers 

There are two essential firmware to work with linux SDK: [RemoteProc and RPMsg](http://software-dl.ti.com/processor-sdk-linux/esd/docs/latest/linux/Foundational_Components_PRU-ICSS_PRU_ICSSG.html#remoteproc-and-rpmsg)
 -  Remoteproc is a framework that allows the ARM host processor(s) to load firmware into PRU cores, start the PRU cores, stop the PRU cores, and configure resources that the PRUs might need during their execution (such as configuring the PRUSS INTC module and providing shared buffers in DDR memory for message passing). This is included as a kernel driver, so a sysfs interface is also exposed to the User Space to start / stop PRU cores, as well as specifying the firmware file to load

 - RPMsg is a method for transmitting messages and data between the PRU cores and the ARM core running Linux, making it possible to communicate with the PRUs from the user space. 

## Get started  

- Before starting, we need to configure a way to send the binary files to the beaglebone. If you use Linux on the PC, you can mount the SD card on the PC and copy the binary files to the `/home/debian` folder. Another method is to create an SFTP connection to transfer files from the PC to the beaglebone. 

- The next step is to verify that the .dtbo file that configures the pins and some peripherals is being loaded when Linux starts.
There is a `AM335X-PRU-RPROC-4-19-TI-PRUCAPE-00A0.dtbo` file in the` /lib/firmware` folder that initiates the status of the pins and configures some settings of the PRU's. For the example shown below, these configurations are sufficient, for more complex examples it may be necessary to activate more resources that are not present in this file, needing to be compiled. It is necessary to check the boot configuration file, `uEnv.txt`. Open the beaglebone terminal and copy this command, `sudo nano /boot/uEnv.txt` and check that the line" uboot_overlay_pru =/lib/firmware/AM335X-PRU-RPROC-4-19-TI-00A0.dtbo " it is uncommented. Note that any file called here must be in the `/lib/firmware` folder.

- For a better understanding we will import the examples that are described in the `PRU with CCStudio` folder, so it is necessary to understand the previous examples. To use the PRU`s present in Beaglebone without an operating system, it was necessary to configure the Arm processor with the .gel file previously explained. So to work with PRU's through the linux operating system it is also necessary to have an additional file, but more complex than the .gel file. 

- The important steps to work with PRU`s through the Linux user space are:
    - Include a resource_table.h in the projects, which has the function of configuring the interaction mode between the ARM processor and the PRU`s (remoteproc, RPMsg buffer, sharing registers and interruptions). 
    - At the end of compiling the project with resource_table.h it is necessary to move the generated binaries to the beaglebone, in the `lib/firmware` folder.
    - Make sure there is a file in the bin/firmware folder capable of overwriting a device tree and modifying the active kernel tree
    - Activate PRU`s peripherals through Sysfs, provided by the Linux kernel that exports information about various kernel subsystems, hardware devices, and associated device drivers from the kernel's device model to user space
    - Place the generated binary files in the `/lib /firmware` folder
    - Activate PRU`s through sysfs. 


### Example Led blink 

- The goal is to make the project Led Blink previously developed, and upload the file to the PRU through the Linux userspace. The files needed for this example are in this section of github, in the `Example Led Blink` folder.

- Follow these steps:
    - 1 -> Open CCStudio and import the `resource_table_0.h` file into the project, open the main.c file and add the file to be compiled. `#include" resource_table_0.h "`.
    - 2 -> Move the binary file into the beaglebone. Choose the easiest method, through ssh or put the SD Card on the PC. Move to the `/home/debian` folder for ease of permission. To speed up the `Example Led Blink` process is a binary` initcializacao.out` file generated in the same way that I explained in the previous step.
    - 3 -> Connect the beaglebone using the ssh protocol for example.
    - 4 -> Go to the `/home/debian` folder and run the following commands to move the binary file to the `/lib/firmware` folder.
        - Check if the file exists run the comand:  `ls`. 
        - If the file with the name exists, you can move it:
            -  ```Makefile 
                sudo mv name_file.out /lib/firmware 
               ```
    - 5 -> To facilitate the process, I created a `led-blink.sh` script that is in the Example Led Blink folder. Move the file to the `/ home / debian` folder for example.
    - 6 -> It is necessary to give permission to the script so that it can be executed. Place the following command on the beaglebone terminal:
        -  ```Makefile 
                chmod a+x /home/debian/led-blink.sh
           ```
    - 7 -> The following commands show how to activate and deactivate the blink led configured on pin P9_27 of the beaglebone, go to the folder where the script is located and you can run the following commands to start and stop led blink.
        -  ```Makefile 
                ./led-blink.sh init    # comand to start project 
                ./led-blink.sh stop    # comand to stop project 
           ```

### Example - Comunication between CORE`s PRU 

....

