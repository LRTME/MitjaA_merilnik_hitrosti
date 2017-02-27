/************************************************************** 
* FILE:         PWM_drv.c
* DESCRIPTION:  PWM driver
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#include "PWM_drv.h"

// prototipi lokalnih funkcij
void PWM_init(void);
void PWM_update_hit(int duty);
void PWM_update_poz(int duty);
/**************************************************************
* Funkcija, ki popiše registre za PWM1,2 in 3. Znotraj funkcije
* se omogoèi interrupt za proženje ADC, popiše se perioda, compare
* register, tripzone register, omogoèi se izhode za PWM...
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


//EPWM Module 3
    // setup timer base
    EPwm3Regs.TBPRD = PWM_PERIOD/2;
    EPwm3Regs.TBCTL.bit.PHSDIR = 0;       // count up after sync
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;
    EPwm3Regs.TBCTL.bit.SYNCOSEL = 1;     // sync out on zero
    EPwm3Regs.TBCTL.bit.PRDLD = 0;        // shadowed period reload
    EPwm3Regs.TBCTL.bit.PHSEN = 0;        // master timer does not sync
    EPwm3Regs.TBCTR = 1;

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
    EPwm1Regs.CMPA.half.CMPA = PWM_PERIOD/4;        //50% duty cycle
    EPwm3Regs.CMPB = PWM_PERIOD/4;           		//50% duty cycle
    EPwm3Regs.CMPA.half.CMPA = PWM_PERIOD/4;   	    //50% duty cycle

    // Init Action Qualifier Output A Register 
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;  // clear output on CMPA_UP
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;    // set output on CMPA_DOWN


    // Init Action Qualifier Output A Register
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;  // clear output on CMPA_UP
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;    // set output on CMPA_DOWN

    // Init Action Qualifier Output B Register
    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;  // clear output on CMPA_UP
    EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;    // set output on CMPA_DOWN

    // Dead Time
    
    // Trip zone 

    //PWM_update_poz(0.0);
    //PWM_update_hit(0.0);
    // output pin setup
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // GPIO4 pin is under ePWM control
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // GPIO5 pin is under ePWM control
    EDIS;                                 // Disable EALLOW

}   //end of PWM_PWM_init

/**************************************************************
* Funkcija, ki na podlagi vklopnega razmerja in izbranega vektorja
* vklopi doloèene tranzistorje
* return: void
* arg1: vklopno razmerje od 0.0 do 1.0 (format Q15)
**************************************************************/
void PWM_update_hit(int duty)
{
   int compare;

    // zašèita za duty cycle 
    //(zašèita za sektor je narejena v default switch case stavku)
    if (duty < 0) duty = 0;

    //izraèunam vrednost compare registra(iz periode in preklopnega razmerja)
    compare = ((long)(PWM_PERIOD/2) * (long)duty) >> 15;

    // vpisem vrednost v register
    EPwm3Regs.CMPA.half.CMPA = compare;


}  //end of PWM_update
	void PWM_update_poz(int duty)
{
	int compare;

    // zašèita za duty cycle
    //(zašèita za sektor je narejena v default switch case stavku)
    if (duty < 0) duty = 0;

    //izraèunam vrednost compare registra(iz periode in preklopnega razmerja)
    compare = ((long)(PWM_PERIOD/2) * (long)duty) >> 15;

    // vpisem vrednost v register

    EPwm3Regs.CMPB = compare;

}  //end of PWM_update

/**************************************************************
* Funkcija, ki starta PWM1. Znotraj funkcije nastavimo
* naèin štetja èasovnikov (up-down-count mode)
* return: void
**************************************************************/
void PWM_start(void)
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  //up-down-count mode
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
    
}   //end of AP_PWM_start
