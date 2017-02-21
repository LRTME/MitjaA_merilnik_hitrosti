/****************************************************************
* FILE:         PER_int.h
* DESCRIPTION:  periodic interrupt header file
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#ifndef     __PER_INT_H__
#define     __PER_INT_H__

#include    "DSP28x_Project.h"

#include    "define.h"
#include    "globals.h"

#include    "PWM_drv.h"
#include    "PCB_util.h"
#include    "math.h"

#include    "DLOG_gen.h"

/**************************************************************
* Funckija, ki pripravi vse potrebno za izvajanje
* prekinitvene rutine
**************************************************************/
extern void PER_int_setup(void);

#endif // end of __PER_INT_H__ definition
