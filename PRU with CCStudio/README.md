# Example with PRUs
- In this short tutorial I demonstrate how to program and work with PRUs. You must have Code Composer Studio (CCS) and PRU Compiler installed. The PRU C Compiler can be found in the App Center, CCS's own plugin.

- The beaglebone processor, Am3358 has two PRUs. In this example, PRU0 has the function of reading a digital input and generating an interruption for PRU1 and then toggle led when the button is pressed. The main objective is to understand how the pins are addressed to the core PRUs and also to understand how the PRUs communicate with each other.

- In this example the beaglebone white (version A6a) is used because it has Debug Probe XDS100v2 and while supporting the traditional JTAG standard. This emulator was removed from the beaglebone black, which is possessed by connectors to weld an external JTAG emulator.


## Register and Pinout 
- There are two special register (R30 and R31)
    - R30 serves on the interface with GPIO output (GPO). 
    - R31 returns status information from GPIO input (GPI) and Interrupts. 

## Initialization GEL Files 
- The General Extension Language (GEL) can be used to configure the Code Composer Studio development environment and to initialize the target CPU. When loading the file in the main processor (ARM), the PRU settings are configured. 

## Get started 
- Open CCS and create two new PRU projects, one project to load into PRU0 and the other to load into PRU1. The following instruction applies to both projects.
  - Go to the project properties and in Build Include Options, edit the path `${CCS_BASE_ROOT}/pru/include` so that this path points to the folder `/ti-processor-sdk-linux-am335x-xx-xx/example-applications/pru-icss-xxx/include/am335x`.
 - Copy the code from the file 'code_pru0.c' to the project inherent to PRU0 and the code code_pru1.c to the PRU1 project.
 - Then, it is necessary to create a 'target_configuration.ccxml'. On the View tab, select target configuration and select the connection type "Texas Instrument XDS100v2 USB Debug probe" and the Board device "Beaglebone". The next step is to open the advanced options of the target configuration, choose the CortexA8 option and select the file `Config_PRU.gel` to be the initialization script.
 - In the targetConfiguration file created, select the "Launch Selected Configuration" option, starting the debug mode.
 - Then it is necessary to connect to the main Processor, CortexA8 and load the startup script, in the "Scripts" tab and select "PRU_Cape_Init".
After completing the previous step, it is necessary to load the corresponding binaries into PRU0 and PRU1.

The purpose of this simple example is to understand how PRUs work and their interaction with the main processor, without an operating system, such as Linux or TI-RTOS.


