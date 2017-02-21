/****************************************************************
* FILENAME:     main.c
* DESCRIPTION:  initialization code
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#include    "main.h"

/**************************************************************
* Funkcija, ki se izvede inicializacijo
**************************************************************/
void main(void)
{
    // PLL, in ura
    InitSysCtrl();
    EALLOW;
    SysCtrlRegs.LOSPCP.bit.LSPCLK = 0;
    EDIS;
    // GPIO - najprej
    InitGpio();

    // inicializiram vhodno izhodne pine
    PCB_init();

    // generic init of PIE
    InitPieCtrl();

    // basic vector table
    InitPieVectTable();

    // inicializiram ADC in PWM modul
    PWM_init();

    // inicializiram peridoièno prekinitev za regulacijo motorja
    PER_int_setup();

    // omogocim prekinitve
    EINT;
    ERTM;

    // pozenem casovnik, ki bo prozil ADC in prekinitev
    PWM_start();
    PWM_update(0.0);

    // grem v neskoncno zanko, ki se izvaja v ozadju
    BACK_loop();
}   // end of main
