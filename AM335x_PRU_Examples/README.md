# Information and examples about PRU

PRU are microcontrollers with a built-in RISC architecture in the ARM Am335x processors, so there are several ways to program them. In this section, I demonstrate two ways to program PRU's.

- The first tutorial is in the folder "PRU with CCStudio". In this tutorial I explain how to work directly with the PRU's, without an operating system running on the ARM processor. It is necessary to have Code composer Studio installed and the PRU compiler, as explained in the tutorial.

- The second tutorial, is in the folder "PRU with Linux Kernel". The goal of the tutorial is to learn how to program PRU's running Linux (debian) on the Am335x processor.


## PRU C Compiler 

TI provides de PRU C compiler in the PRU Code Generation Tools package, available in the `linuxdevkit/sysroots/x86_64_arago-linux/usr/share/ti/cgt-pru/`, if the operation system is ubuntu, this package is located in the installation folder of the Linux sdk, example: `~/ti-processor-sdk-linux-am335x-evm-xx.xx.xx.xx/linux-devkit/sysroots/x86_64-arago-linux/usr/share/ti/cgt-pru`. This C compiler is available as an add-on package for CCSudio or it can be transferred to the beaglebone, if you have the `Debian` version, and compile on the beaglebone itself.

