/****************************************************************
* FILENAME:     PER_int.c
* DESCRIPTION:  periodic interrupt code
* AUTHOR:       Mitja Nemec
*
****************************************************************/
#include    "PER_int.h"


// spremenljikva s katero štejemo kolikokrat se je prekinitev predolgo izvajala
int interrupt_overflow_counter = 0;





long kot_iz_senzorja;
float frek;




_iq f_abf=_IQ(0.0);
_iq kot_abf=_IQ(0.0);
_iq epsilon=_IQ(0.0);
_iq dt=_IQ(0.00005);
_iq alpha=_IQ(0.0883642);
_iq betha=_IQ(0.0000142122);





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



    kot_iz_senzorja=SPI_getkot();

   PWM_update_poz((int)(kot_iz_senzorja*9));


//Algoritem za izracun frekvence

            kot_abf+=_IQmpy(f_abf,dt);
            if(kot_abf>_IQ(1.0))
            {
            	kot_abf-=_IQ(1.0);
                            }
            if(kot_abf < 0 )
            {
            	kot_abf +=_IQ(1.0);
            }

            epsilon= (kot_iz_senzorja<<12)- kot_abf;
            if(epsilon>_IQ(1.0))
            {
            	epsilon-=_IQ(1.0);
            }
            if(epsilon < -_IQ(1.0) )
            {
            	epsilon +=_IQ(1.0);
            }

             kot_abf+=_IQmpy(alpha,epsilon);

             f_abf+=_IQmpy(epsilon,_IQdiv(betha,dt));

             frek= _IQtoF(f_abf);// frekvenca za prikaz v float formatu

           PWM_update_hit((int)(((f_abf>>12)*220)>>12));  // frekvenco v 8.24 foramtu pomnozis s faktorjem da bo rezultat manjsi od 11196















    // spavim vrednosti v buffer za prikaz
    DLOG_GEN_update();

    /* preverim, èe me sluèajno èaka nova prekinitev.
       èe je temu tako, potem je nekaj hudo narobe
       saj je èas izvajanja prekinitve predolg
       vse skupaj se mora zgoditi najmanj 10krat,
       da reèemo da je to res problem
    */
    if (EPwm1Regs.ETFLG.bit.INT == TRUE)
    {
        // povecam stevec, ki steje take dogodke
        interrupt_overflow_counter = interrupt_overflow_counter + 1;
        
        // in ce se je vse skupaj zgodilo 10 krat se ustavim
        // v kolikor uC krmili kakšen resen HW, potem moèno
        // proporoèam lepše "hendlanje" takega dogodka
        // beri:ugasni moènostno stopnjo, ...
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

    dlog.iptr1 = &kot_iz_senzorja;
    dlog.iptr2 = &kot_abf;
    dlog.iptr3 = &f_abf;

    // Proženje prekinitve
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;    //sproži prekinitev na periodo
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
    // da mi prekinitev teèe  tudi v real time naèinu
    // (za razhoršèevanje main zanke in BACK_loop zanke)
    SetDBGIER(M_INT3);
}
