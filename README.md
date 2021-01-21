# iMultComP
industrial Multi Communication Protocol


## BeagleBone Black Development Board


## PRU-ICSS Industrial Software for Sitaraª Processors AM3358 (Beaglebone)
- [Link to Dowload SDK for AM335x](https://www.ti.com/tool/PROCESSOR-SDK-AM335X)
PRU-ICSS was designed with real-time industrial communications in mind. Ethernet, Fieldbus,interface, and redundancy protocols are available for the PRU-ICSS, including certified solutions for EtherCATš, PROFIBUSš, PROFINET. 

    In the context of the SoC, there are three environments in which the PRU-ICSS is used: Code Composer Studio(CCS), TI RTOS running on the Arm core, and Linux running on the Arm core.

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
### Example TI-SDK-Linux with RemoteProc and RPMsg
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
- [x] PRU Getting Starting on TI-RTOS 


-------------
## PRU C Compiler 

TI provides de PRU C compiler in the PRU Code Generation Tools package, available in the `linuxdevkit/sysroots/x86_64_arago-linux/usr/share/ti/cgt-pru/`, if if the operation system is ubuntu, this package is located in the installation folder of the Linux sdk, example: `~/ti-processor-sdk-linux-am335x-evm-xx.xx.xx.xx/linux-devkit/sysroots/x86_64-arago-linux/usr/share/ti/cgt-pru`. This C compiler is available as an add-on package for Code Componser Studio or it can be transferred to the beaglebone, if you have the `Debian` version, and compile on the beaglebone itself.
