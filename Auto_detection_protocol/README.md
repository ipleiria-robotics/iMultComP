# iMultComP

## Automatic protocol detection

- In this section is a project (texas TIDEP0032) that automatically detects the protocol when the network cable is connected. The objective of this project is to detect the protocol, to record in a non-volatile memory a certain value depending on the detected protocol. The value inserted in the non-volatile memory can be used to reset the CPU and then start the program in the memory zone of the respective protocol. It is a dynamic way to start the board automatically according to the protocol  

- [Link to the demo video](https://www.youtube.com/watch?v=4IExPr1a1yQ&ab_channel=TexasInstruments)

- The "detection_firmware" project is for the PRU and the "selectorMin" project is for the ARM. The purpose of this "detection_firmware" project is to generate the binary and convert it into a .h file and then insert it into the "selectorMin" project. If you want to use this project just import the project "selectorMin" to CCStudio because the binary of the project "detection_firmware" is already inserted, only if you want to make changes to the PRU project.



