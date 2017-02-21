/************************************************************** 
* FILE:         PWM_drv.c
* DESCRIPTION:  PWM driver
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#include "PWM_drv.h"

// prototipi lokalnih funkcij


/**************************************************************
* Funkcija, ki popi�e registre za PWM1,2 in 3. Znotraj funkcije
* se omogo�i interrupt za pro�enje ADC, popi�e se perioda, compare
* register, tripzone register, omogo�i se izhode za PWM...
* return:void
**************************************************************/
void PWM_init(void)
{
//EPWM Module 1
    // setup timer base 
    EPwm1Regs.TBPRD = PWM_PERIOD/2;       
    EPwm1Regs.TBCTL.bit.PHSDIR = 0;       // count up after sync
    EPwm1Regs.TBCTL.bit.CLKDIV = 0;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1;     // sync out on zero
    EPwm1Regs.TBCTL.bit.PRDLD = 0;        // shadowed period reload
    EPwm1Regs.TBCTL.bit.PHSEN = 0;        // master timer does not sync  
    EPwm1Regs.TBCTR = 1;

    // debug mode behafiour
    #if PWM_DEBUG == 0
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;  // stop immediately
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;  // stop immediately
    #endif
    #if PWM_DEBUG == 1
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 1;  // stop when finished
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 1;  // stop when finished
    #endif
    #if PWM_DEBUG == 2
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;  // run free
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;  // run free
    #endif

    // Compare registers
    EPwm1Regs.CMPA.half.CMPA = PWM_PERIOD/4;                 //50% duty cycle

    // Init Action Qualifier Output A Register 
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;  // clear output on CMPA_UP
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;    // set output on CMPA_DOWN

    // Dead Time
    
    // Trip zone 

    PWM_update(0.0);

    // output pin setup
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // GPIO0 pin is under ePWM control
    EDIS;                                 // Disable EALLOW

}   //end of PWM_PWM_init

/**************************************************************
* Funkcija, ki na podlagi vklopnega razmerja in izbranega vektorja
* vklopi dolo�ene tranzistorje
* return: void
* arg1: vklopno razmerje od 0.0 do 1.0 (format Q15)
**************************************************************/
void PWM_update(int duty)
{
   int compare;

    // za��ita za duty cycle 
    //(za��ita za sektor je narejena v default switch case stavku)
    if (duty < 0) duty = 0;

    //izra�unam vrednost compare registra(iz periode in preklopnega razmerja)
    compare = ((long)(PWM_PERIOD/2) * (long)duty) >> 15;

    // vpisem vrednost v register
    EPwm1Regs.CMPA.half.CMPA = compare;
    

}  //end of PWM_update

/**************************************************************
* Funkcija, ki starta PWM1. Znotraj funkcije nastavimo
* na�in �tetja �asovnikov (up-down-count mode)
* return: void
**************************************************************/
void PWM_start(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
    
}   //end of AP_PWM_start



