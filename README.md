# iMultComP
industrial Multi Communication Protocol


## BeagleBone Black Development Board


## PRU 


## Install softwares and Tutoriais 


## PRU-ICSS Industrial Software for Sitaraª Processors AM3358 (Beaglebone)

- [Link to Dowload SDK for AM335x](https://www.ti.com/tool/PROCESSOR-SDK-AM335X)
PRU-ICSS was designed with real-time industrial communications in mind. Ethernet, Fieldbus,interface, and redundancy protocols are available for the PRU-ICSS, including certified solutions for EtherCAT, PROFIBUS, PROFINET. 

 In the context of the SoC, there are three environments in which the PRU-ICSS is used: (CCStudio), TI RTOS running on the Arm core, and Linux running on the Arm core.
### TI-SDK-LINUX


### TI-RTOS 



-------------
## PRU C Compiler 

TI provides de PRU C compiler in the PRU Code Generation Tools package, available in the `linuxdevkit/sysroots/x86_64_arago-linux/usr/share/ti/cgt-pru/`, if the operation system is ubuntu, this package is located in the installation folder of the Linux sdk, example: `~/ti-processor-sdk-linux-am335x-evm-xx.xx.xx.xx/linux-devkit/sysroots/x86_64-arago-linux/usr/share/ti/cgt-pru`. This C compiler is available as an add-on package for CCSudio or it can be transferred to the beaglebone, if you have the `Debian` version, and compile on the beaglebone itself.
