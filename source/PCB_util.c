/**************************************************************
* FILE:         PCB_util.c 
* DESCRIPTION:  PCB initialization & Support Functions
* AUTHOR:       Mitja Nemec
*
**************************************************************/
#include "PCB_util.h"

/**************************************************************
* Funckija ki prizge LED diodo 1
**************************************************************/
#pragma CODE_SECTION(PCB_LEDstick_on, "ramfuncs");
void PCB_LEDstick_on(void)
{
    GpioDataRegs.GPBSET.bit.GPIO34 = 1;
}

/**************************************************************
* Funckija ki ugasne LED diodo 1
**************************************************************/
#pragma CODE_SECTION(PCB_LEDstick_off, "ramfuncs");
void PCB_LEDstick_off(void)
{
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
}

/**************************************************************
* Funckija ki spremeni stanje LED diode
**************************************************************/
#pragma CODE_SECTION(PCB_LEDstick_toggle, "ramfuncs");
void PCB_LEDstick_toggle(void)
{
    GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
}



/**************************************************************
* Funckija ki inicializira PCB
**************************************************************/
void PCB_init(void)
{
    EALLOW;

    /* IZHODI */
    // LED na controlSTICK-u
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO34 = 0;
    EDIS;

}
