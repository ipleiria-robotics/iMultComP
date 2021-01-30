# PRU with Linux User Space 

- There are several models of Beaglebones. The most common is the Beaglebone Black which has 2/4G MMC of memory internally. By default Beaglebone Black comes with a Debian version installed. The Beaglebone version of Debian can be found on the official page, Beaglebone.org. This version of debian already has the PRU compiler installed, several simple examples and with IDE Cloud9 running on port 3000 of beaglebone, however I do not recommend using this version of debian, as it does not have all the fundamental components to work with more advanced projects. The debian version is recommended for working with beaglebone using high-level languages, such as Javascript or Python.

- Since almost all Beaglebone versions have microSD slots, it is recommended to create a Linux SDK SDK. This requires re-building the Linux Embedded and and create SD Card. The linux version available from texas is based on the Yocto Project and already has the necessary tools to work with PRU's. 

## Requirements 
- It is necessary to install the [`Processor SDK Linux`](https://software-dl.ti.com/processor-sw/esd/PROCESSOR-SDK-LINUX-AM335X/latest/index_FDS.html). The Linux SDK installation folder contains the files and scripts needed for re-building the Linux kernel and create SD Card .

- Texas provides a [Wiki](https://software-dl.ti.com/processor-sdk-linux/esd/docs/06_03_00_106/linux/index.html), where it explains step by step to re-building the Linux kernel and create SD Card.


## ARM <---> PRU interaction via remoteproc and rpmsg Linux drivers 

- [x] PRU-ICSS Getting Starting on TI-SDK-LINUX

There are two essential firmware to work with linux SDK: [RemoteProc and RPMsg](http://software-dl.ti.com/processor-sdk-linux/esd/docs/latest/linux/Foundational_Components_PRU-ICSS_PRU_ICSSG.html#remoteproc-and-rpmsg)
    -  Remoteproc is a framework that allows the ARM host processor(s) to load firmware into PRU cores, start the PRU cores, stop the PRU cores, and configure resources that the PRUs might need during their execution (such as configuring the PRUSS INTC module and providing shared buffers in DDR memory for message passing). This is included as a kernel driver, so a sysfs interface is also exposed to the User Space to start / stop PRU cores, as well as specifying the firmware file to load

    - RPMsg is a method for transmitting messages and data between the PRU cores and the ARM core running Linux, making it possible to communicate with the PRUs from the user space. 



#### Example TI-SDK-Linux with RemoteProc and RPMsg
```Makefile
echo 'am335x-pru0-fw' > /sys/class/remoteproc/remoteproc1/firmware # specify the name of the firmware to be loaded PRU0
echo 'am335x-pru1-fw' > /sys/class/remoteproc/remoteproc2/firmware # specify the name of the firmware to be loaded PRU1
echo 'start' > /sys/class/remoteproc/remoteproc1/state # Start remoteproc1 
echo 'start' > /sys/class/remoteproc/remoteproc2/state # Start remoteproc1 

#List all of the devices under /dev/ and look for the two rpmsg_pru character devices
ls /dev/ | grep pru

#Write to each of the character devices to test them out
echo "message_1" > /dev/rpmsg_pru30 #
echo "message_2" > /dev/rpmsg_pru31
# Now read
cat /dev/rpmsg_pru30
cat /dev/rpmsg_pru31
```