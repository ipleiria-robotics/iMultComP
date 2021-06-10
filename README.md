# iMultComP
Industrial Multi Communication Protocol

- In industrial communications, time is an essential element in a packet network, used to synchronize the clocks of the connected components through time synchronization protocols. In industrial networks, to ensure accurate communication between industrial computers, PLC, HMI, sensors, etc., timing requires nanosecond values. 
- To communicate with modern industrial protocols, specific hardware is required. A hardware that has become common in the industry is the FPGA (Field Programmable Gate Array), a programmable logic device that supports the implementation of digital circuits. It has performance, parallelism and real time. For example, performing calculations and receiving packets from an industrial network, delivering the result at the same clock rate, something impossible for a microprocessor running an optional system (linux or RTOS).

## 

- In order to have an embedded system running an operating system (LINUX or RTOS) but able to communicate with industrial networks, Texas has developed an AM335x processor that has two PRU-ICSS modules (Programmable Real-time Unit and Industrial Communication SubSystem).
-So, for example, the PRU module is responsible for receiving packets from an industrial network, storing the data in buffers to supply the data to the ARM processor. 

## Board TMDSICE3359 AM3359 Industrial Communications Engine 

- The AM3359 Industrial Communications Engine (ICE) is a development platform targeted for systems that specifically focus on the industrial communications capabilities of the Sitara AM335x Arm® Cortex ™ -A8 processors.
The AM335x Arm Cortex-A8 processors integrate the Programmable Real-time Unit (PRU) that has been architected to implement the real-time communication technologies used in a broad range of industrial automation equipment. It enables low foot print designs with minimal external components and with best in class low power performance.

<img src="/Image/board.png" width="300" >

- [Sitara ICEAM3359](https://www.ti.com/tool/TMDSICE3359)


## PRU-ICSS Industrial Software for Sitara Processors SITARA AM3359 

- The Programmable Real-time Unit and Industrial Communication SubSystem (PRU-ICSS) consists of dual 32-bit RISC architecture cores, shared data, instruction memories, internal peripheral modules, and an interrupt controller (INTC). 
- The programmable of the PRU, along with its access to pins, events and all system-on-chip (SoC) resources, provides flexibility in implementing fast real-time responses. 
- Am335x processors have two PRU`s cores internally. They are programmed with a small, deterministic instruction set. Each PRU can operate independently or in coordination with each other and can also work in coordination with the device-level host CPU (ARM). The interaction between processors is determined by the nature of the firmware loaded into the PRU’s instruction memories. 
- In the folder AM335x PRU Examples, you can find more information about PRU`s and practical examples.
 
![PRU_ICSS_Block](https://user-images.githubusercontent.com/65302944/109708007-bf4f0280-7b92-11eb-83ce-fad872dfa510.PNG)

- [Link to Dowload SDK for AM335x](https://www.ti.com/tool/PROCESSOR-SDK-AM335X)
PRU-ICSS was designed with real-time industrial communications in mind. Ethernet, Fieldbus,interface, and redundancy protocols are available for the PRU-ICSS, including certified solutions for EtherCAT, PROFIBUS, PROFINET. In the context of the SoC, there are three environments in which the PRU-ICSS is used: CCStudio without operation system, with SDK TI RTOS running on the Arm core, and Linux running on the Arm core.

- In the folder AM335x_PRU_Examples, there are several tutorials to learn how to program the PRU´s but also to better understand how the PRU´s work inside the ARM AM335x processor


## Github Organization 

-  In order to have competence on Texas tools I recommend starting with the tutorials that are in the folder `AM335X_PRU_Examples`

- After performing the previous step, you are ready to understand the construction of industrial protocols with TI-RTOS. The projects are in the `Individual_Projects_Protocols` folder. 

- In the folder `Auto_detection_protocol` there is a project that detects the protocol automatically. This project is useful for creating a bootloader capable of automatically starting any project present in the `Individual_Projects_Protocols` folder. 


