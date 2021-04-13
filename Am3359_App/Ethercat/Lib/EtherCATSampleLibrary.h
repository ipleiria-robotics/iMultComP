/*
* This source file is part of the EtherCAT Slave Stack Code licensed by Beckhoff Automation GmbH & Co KG, 33415 Verl, Germany.
* The corresponding license agreement applies. This hint shall not be removed.
*/

/*-----------------------------------------------------------------------------------------
------
------    Description
------
------    EtherCATSampleLibrary.h
------
------                                                                                                                                                                 ------
-----------------------------------------------------------------------------------------*/

typedef struct
{
    unsigned long * pInput;
    unsigned long * pOutput;
    
    void (* pApplication)(void);
    void (* pStateTrans)(unsigned short transind);
}
ECAT_SLAVE_INTERFACE, *pECAT_SLAVE_INTERFACE;


pECAT_SLAVE_INTERFACE Ecat_Open(unsigned short InputSize, unsigned short OutputSize);

void Ecat_OnTimer(void);

void Ecat_Close(void);
