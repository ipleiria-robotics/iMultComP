# Program the PRU's without an operating system
- In this short tutorial I demonstrate how to program and work with PRUs. You must have Code Composer Studio (CCS) and PRU Compiler installed. The PRU C Compiler can be found in the App Center, CCS's own plugin. Open CCStudio, open the View tab and click on CCS App Center. Select PRU Compiler by clicking on Install Software.

- The beaglebone processor, Am3358 has two PRUs.

- In this example the beaglebone white (version A6a) is used because it has Debug Probe XDS100v2 and while supporting the traditional JTAG standard. This emulator was removed from the beaglebone black, which is possessed by connectors to weld an external JTAG emulator.


## Register and Pinout 
- There are two special register (R30 and R31)
    - R30 serves on the interface with GPIO output (GPO). 
    - R31 returns status information from GPIO input (GPI) and Interrupts. 
    - The information about the pins can be found in the ref pinput folder

## Initialization GEL Files 

- The General Extension Language (.GEL) is interpreted language similar to C that allows creating functions to extend Code Composer Studio IDE’s usefulness. Commonly used to initialize devices or boards, it can also be extended to perform several automation tasks within a Debug Session. When loading the file in the main processor (ARM), the PRU settings are configured. 

-  The extension .gel and uses either the Built-In GEL Functions or the GEL Macros and follows the guidelines of the GEL Grammar - a C-based rules syntax.

-  You can associate a GEL startup file with each processor in your system configuration. When CCStudio is launched, startup GEL files defined in the CCStudio setup utility are loaded into the host PC's memory and the "StartUp()" function is executed (if one is defined).

## Get started 
- Below are two examples to explain how to program the PRU without an operating system (Bare_metal). 

### Example LED Blink 

- The purpose of this example is useful to demonstrate how to program a led_blink working directly with the PRU and the files can be found in the "Example Led Blink" folder. The beaglebone black has two groups of pins, P9 and P8. P9 is connected to PRU0 and P8 to PRU1. Not all pins are available for PRUs, in the Ref / Pinout folder the pin tables for P9 and P8 are available. As in this example PRU0 is used so just look at table_P9. There are several configuration modes, to configure the pins with the PRU0 just look at MODE5(OUTPUTS) and MODE6(INPUTS). 

- To understand the configuration of the pins it is necessary to look at the `.gel` file located in the" Example Led Blink "folder. As explained, it is necessary to load the .gel file on the main processor 'ARM' to configure and initialize the PRU's. The file contains the pin configurations to be associated with the PRU0 as well as the configurations of the main registers. For more details it is recommended to read the comments in the .gel file.

-- Open CCStudio and click File -> New -> CCS Project
-- Next, in the target field, select am335x - Beaglebone
-- Open the PRU tab, insert some name, and then select Empty Project(With main.c)
-- Click Finish to continue 
-- With the project created, copy the contents of the main.c file found in the `Example Led blink` folder to the new main.c file created in CCStudio.
-- To compile the project it is necessary to include the header files of the PRU. Click with the right mouse button and select properties> Bluid> PRU Compiler> Include options and double click on `${CCS_BASE_ROOT}/pru/include` to edit the directory. Next click on Browse and point to the `Includes` folder found in the `PRU with CCStudio` folder, and click OK.
-- To be able to load the binary into the PRU it is necessary to create a TragetConfiguration, this step is necessary to create a .ccxml file. The .ccxml file is used by CCStudio to configure the connection between the board and the PC using the JTAG protocol. Select the View > TargetConfiguration tab. On the right side of the screen, a new tab will appear, it is necessary to open the "User Defined" folder to view the new .ccxml file created.
-- It is necessary to edit the .ccxml file to have the information contained in the .gel file. In the 'Connection' field select "XDS100v2 USB Debug Probe", consult the datasheet of your development board to see which emulator is present, in 'Board/Device' select the "Beaglebone" board. At the bottom of the tab, switch to the Advanced tab, select the main core "CortexA8" and then in the 'initiazation script' field click on browse and point to the .gel file in the `Example Led Blink` folder. Finally save the settings. 
-- Right-click on the .ccxml file created earlier and select 'Launch selected Configuration'. If everything went well, the connection between the PC and the card was successful.
-- A tree will appear with the devices found by the emulator on the development board. Select the Arm CortexA8, click with the right mouse button and select 'Connection Target'. Now it's time to load the .gel file to the main core, in the Script tab select 'PRU_config'> 'PRU_cape_Init'. This step loads the configurations that the .gel file has to the board.
-- The next step is to load the generated binary into PRU0, select the PRU and connect to it as done in the previous step. At the end of the connection, click on the Run> load> tab and look for the binary in the Debug folder of the created project.
-- Then click F8 or play to start. 

### Example - Comunication between CORE`s PRU 

 In this example, PRU0 has the function of reading a digital input and generating an interruption for PRU1 and then toggle led when the button is pressed. The main objective is to understand how the pins are addressed to the core PRUs and also to understand how the PRUs communicate with each other.

 To carry out this example, it is advisable to first perform the example "Led Blink" to understand how to create a project and how to upload the project to the PRU's. If you did the previous example, now it is not necessary to create a new 'TargetConfiguration', since the .ccxml file is saved in the installation directory of CCStudio. You can create a new 'TargetConfiguration' or edit the one created previously, as you just need to change the path pointed to a new .gel file that contains the information about the pins needed for this project.

 - Open CCS and create two new PRU projects, one project to load into PRU0 and the other to load into PRU1. The following instruction applies to both projects.
 - Go to the project properties and in Build Include Options, edit the path `${CCS_BASE_ROOT}/pru/include` so that this path points to the folder `Includes` found in the `PRU with CCStudio` folder.
 - Copy the code from the file 'code_pru0.c' to the project inherent to PRU0 and the code code_pru1.c to the PRU1 project.
 - Then, it is necessary to modify a 'target_configuration.ccxml' created in the previous project, in the 'Advanced' tab, select the main core "CortexA8" and then in the 'initiazation script' field click on browse and point to the .gel file in the Example `xample Comunication_between_CORE_PRU`  folder. Finally save the settings.
 - In the targetConfiguration file created, select the "Launch Selected Configuration" option, starting the debug mode.
 - Then it is necessary to connect to the main Processor, CortexA8 and load the startup script, in the "Scripts" tab and select "PRU_config".
After completing the previous step, it is necessary to load the corresponding binaries into PRU0 and PRU1.



The purpose of this simple example is to understand how PRUs work and their interaction with the main processor, without an operating system, such as Linux or TI-RTOS.




