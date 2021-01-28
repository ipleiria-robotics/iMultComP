
# PRU with Linux User Space 

## 

- [x] PRU-ICSS Getting Starting on TI-SDK-LINUX
    In the Processor SDK Linux, TI provides prebuilt PRU-ICSS firmware to implement 100Mbps Ethernet
ports. In addition to the PRU firmware provided, TI has also created and provided the Linux drivers necessary to use the PRU Ethernet ports from user space applications.
There are two essential firmware to work with linux SDK: 
-- [RemoteProc and RPMsg](http://software-dl.ti.com/processor-sdk-linux/esd/docs/latest/linux/Foundational_Components_PRU-ICSS_PRU_ICSSG.html#remoteproc-and-rpmsg)
    - RemoteProc
       Remoteproc is a framework that allows the ARM host processor(s) to load firmware into PRU cores, start the PRU cores, stop the PRU cores, and configure resources that the PRUs might need during their execution (such as configuring the PRUSS INTC module and providing shared buffers in DDR memory for message passing).
        This is included as a kernel driver, so a sysfs interface is also exposed to the User Space to start / stop PRU cores, as well as specifying the firmware file to load

    - RPMsg 
        RPMsg is a method for transmitting messages and data between the PRU cores and the ARM core running Linux, making it possible to communicate with the PRUs from the user space. 
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