//###########################################################################
//
// FILE:    F2802x_DefaultIsr.c
//
// TITLE:    F2802x Device Default Interrupt Service Routines.
//
// This file contains shell ISR routines for the 2802x PIE vector table.
// Typically these shell ISR routines can be used to populate the entire PIE
// vector table during device debug.  In this manner if an interrupt is taken
// during firmware development, there will always be an ISR to catch it.
//
// As development progresses, these ISR routines can be eliminated and replaced
// with the user's own ISR routines for each interrupt.  Since these shell ISRs
// include infinite loops they will typically not be included as-is in the final
// production firmware.
//
//###########################################################################
// $TI Release: F2802x Support Library v230 $
// $Release Date: Fri May  8 07:43:05 CDT 2015 $
// $Copyright: Copyright (C) 2008-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2802x_Device.h"     // Headerfile Include File
#include "f2802x_examples.h"   // Examples Include File

// Connected to multiple peripherals
interrupt void PERIPHERAL_ISR(void)
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

// connected to user traps
interrupt void USER_ISR(void)     // INT14 or CPU-Timer2
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("      ESTOP0");
  for(;;);
}

interrupt void NMI_ISR(void)       // Non-maskable interrupt
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
  asm ("    ESTOP0");
  asm ("    NOP");
  asm ("    B $-1,UNC");
  asm ("    NOP");
}

interrupt void ILLEGAL_ISR(void)   // Illegal operation TRAP
{
  // Insert ISR Code here

  // Next two lines for debug only to halt the processor here
  // Remove after inserting ISR Code
    asm ("    ESTOP0");
    asm ("    NOP");
    asm ("    B $-1,UNC");
    asm ("    NOP");
}

//---------------------------------------------------------------------------
// Catch All Default ISRs:
//

interrupt void EMPTY_ISR(void)  // Empty ISR - only does a return.
{
    asm ("    ESTOP0");
    asm ("    NOP");
    asm ("    B $-1,UNC");
    asm ("    NOP");
}

interrupt void PIE_RESERVED(void)  // Reserved space.  For test.
{
    asm ("    ESTOP0");
    asm ("    NOP");
    asm ("    B $-1,UNC");
    asm ("    NOP");
}

interrupt void rsvd_ISR(void)      // For test
{
    asm ("    ESTOP0");
    asm ("    NOP");
    asm ("    B $-1,UNC");
    asm ("    NOP");
}

//===========================================================================
// End of file.
//===========================================================================

