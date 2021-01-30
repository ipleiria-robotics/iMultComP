# iMultComP
Industrial Multi Communication Protocol


## BeagleBone Black Development Board

### Sharing Internet with Beaglebone   
- It is possible to share internet sharing from the linux or windows laptop to the beaglebone, via USB. The main steps are to enable internet sharing on the windows or linux laptop and then add the default route for BeagleBone to try to connect to the internet through the laptop, as explained in both tutorials.  

    - [x] In windows it is easier, just go to the original page of [beaglebone.org](https://beagleboard.org/getting-started), follow the steps there to install the beaglebone software. Then just open the internet sharing center(Network Configuration settings in the Windows Control Panel), go to the interface properties for example the wifi, and activate the internet sharing with the beaglebone.  
        . This ["Digi-Key"] tutorial (https://www.digikey.com/en/maker/blogs/how-to-connect-a-beaglebone-black-to-the-internet-using-usb) demonstrates how to enable internet sharing on windows.  

    - [x] This tutorial available on [github]("https://gist.github.com/pdp7/d2711b5ff1fbb000240bd8337b859412"), demonstrates very well how to enable internet sharing of a linux laptop with the beaglebone.  
 
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
