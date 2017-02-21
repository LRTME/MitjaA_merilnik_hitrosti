/**************************************************************
* FILE:         PCB_util.h 
* DESCRIPTION:  definitions for PCB Initialization & Support Functions
* AUTHOR:       Mitja Nemec
*
**************************************************************/
#ifndef   PCB_UTIL_H
#define   PCB_UTIL_H

#include    "DSP28x_Project.h"
#include    "define.h"

/**************************************************************
* Funkcije ki manipulirajo LED diodo na ControlSTICK-u
**************************************************************/
extern void PCB_LEDstick_on(void);
extern void PCB_LEDstick_off(void);
extern void PCB_LEDstick_toggle(void);

/**************************************************************
* Funckija ki inicializa MCU in tiskanino
**************************************************************/
extern void PCB_init(void);


#endif  // end of PCB_UTIL_H definition

