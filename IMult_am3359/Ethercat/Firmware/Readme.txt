The firmware version in this folder corresponds to the ICSS version in the SOC

1) v1.0 -> ICSS v1.0 -> Present in AM335x, AMIC110 and AM572x PG1.x
1) v2.0 -> ICSS v2.0 -> Present in AM437x
1) v2.1 -> ICSS v2.1 -> Present in AM572x and AM571x
1) v2.2 -> ICSS v2.2 -> Present in K2G

These firmwares are NOT interchangable!
For example, only "v2.1" is compatible with AM57xx and none of the other firmwares will work with AM57xx.

When the project is created using the projectCreate.bat/sh scripts appropriate firmware version will be linked by default depending on the SOC.