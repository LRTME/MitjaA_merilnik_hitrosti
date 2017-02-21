/************************************************************** 
* FILE:         PWM_drv.h
* DESCRIPTION:  PWM driver
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#ifndef     __PWM_DRV_H__
#define     __PWM_DRV_H__

#include    "DSP28x_Project.h"

#include    "define.h"
#include    "globals.h"

/* Definicije */
// delovanje modula ob debug-dogodkih
// 0 stop immediately, 1 stop when finished, 2 run free
#define     PWM_DEBUG       0

// perioda casovnika (v procesorskih cikilh) 
#define     PWM_PERIOD      (CPU_FREQ/(SAMPLE_FREQ * SAMP_PRESCALE))

// prescaler za prekinitev
#define     PWM_INT_PSCL    SAMP_PRESCALE

/**************************************************************
* Funkcija, pokli�e funkciji PWM_PWM_init in PWM_ADC_init; kli�emo
* jo iz main-a
* return: void
**************************************************************/
extern void PWM_init(void);

/**************************************************************
* Funkcija, ki popi�e registre za PWM1,. Znotraj funkcije
* se omogo�i interrupt za pro�enje ADC, popi�e se perioda, compare
* register, omogo�i se izhode za PWM...
* return:void
**************************************************************/
extern void PWM_update(int duty);

/**************************************************************
* Funkcija, ki starta PWM1. Znotraj funkcije nastavimo
* na�in �tetja �asovnikov (up-down-count mode)
* return: void
**************************************************************/
extern void PWM_start(void);


#endif  // end of __PWM_DRV_H__ definition

