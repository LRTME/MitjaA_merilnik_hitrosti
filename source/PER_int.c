/****************************************************************
* FILENAME:     PER_int.c
* DESCRIPTION:  periodic interrupt code
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#include    "PER_int.h"

// spremenljikva s katero �tejemo kolikokrat se je prekinitev predolgo izvajala
int interrupt_overflow_counter = 0;
int zapwm;
/**************************************************************
* Prekinitev, ki v kateri se izvaja regulacija
**************************************************************/
#pragma CODE_SECTION(PER_int, "ramfuncs");
void interrupt PER_int(void)
{
    /* lokalne spremenljivke */

    // najprej povem da sem se odzzval na prekinitev
    // Spustimo INT zastavico casovnika ePWM1
    EPwm1Regs.ETCLR.bit.INT = 1;
    // Spustimo INT zastavico v PIE enoti
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    
    // povecam stevec prekinitev
    interrupt_cnt = interrupt_cnt + 1;

    if (interrupt_cnt >= SAMPLE_FREQ)
    {
        interrupt_cnt = 0;
    }
    SPI_getkot();
    PWM_update_hit(zapwm);

    // spavim vrednosti v buffer za prikaz
    DLOG_GEN_update();

    /* preverim, �e me slu�ajno �aka nova prekinitev.
       �e je temu tako, potem je nekaj hudo narobe
       saj je �as izvajanja prekinitve predolg
       vse skupaj se mora zgoditi najmanj 10krat,
       da re�emo da je to res problem
    */
    if (EPwm1Regs.ETFLG.bit.INT == TRUE)
    {
        // povecam stevec, ki steje take dogodke
        interrupt_overflow_counter = interrupt_overflow_counter + 1;
        
        // in ce se je vse skupaj zgodilo 10 krat se ustavim
        // v kolikor uC krmili kak�en resen HW, potem mo�no
        // proporo�am lep�e "hendlanje" takega dogodka
        // beri:ugasni mo�nostno stopnjo, ...
        if (interrupt_overflow_counter >= 10)
        {
            asm(" ESTOP0");
        }
    }
    
}   // end of PWM_int

/**************************************************************
* Funckija, ki pripravi vse potrebno za izvajanje
* prekinitvene rutine
**************************************************************/
void PER_int_setup(void)
{
    // inicializiram data logger
    dlog.mode = Normal;
    dlog.auto_time = 1;
    dlog.holdoff_time = 1;

    dlog.prescalar = 1;                		// store every  sample

    dlog.slope = Negative;
    dlog.trig = &interrupt_cnt;
    dlog.trig_value = 1;

    dlog.iptr1 = &zapwm;
    dlog.iptr2 = &interrupt_cnt;

    // Pro�enje prekinitve
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    //spro�i prekinitev na periodo
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;         //ob vsakem prvem dogodku
    EPwm1Regs.ETCLR.bit.INT = 1;                //clear possible flag
    EPwm1Regs.ETSEL.bit.INTEN = 1;              //enable interrupt

    // registriram prekinitveno rutino
    EALLOW;
    PieVectTable.EPWM1_INT = &PER_int;
    EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    IER |= M_INT3;
    // da mi prekinitev te�e  tudi v real time na�inu
    // (za razhor��evanje main zanke in BACK_loop zanke)
    SetDBGIER(M_INT3);
}
