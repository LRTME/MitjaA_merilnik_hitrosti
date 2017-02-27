/************************************************************** 
* FILE:         IQ_math.h
* DESCRIPTION:  deklaracije dodanih funkcij nad IQ stevili
* VERSION:      1.0
* AUTHOR:       Mitja Nemec
**************************************************************/
#ifndef IQ_MATH_H
#define IQ_MATH_H

// vse funkcije podpirajo saturacijo, in so nasploh odporne na zlorabo


/**************************************************************
* Funckija ki deli dve stevili v poljublnih IQ formatih
* returns:  rezultat
* arg1:     deljenec
* arg2:     deljitelj
* arg3:     IQ format deljenca
* arg4:     IQ format deljitelja
* arg5:     IQ format rezultata
**************************************************************/
extern long mdiv(long x, long y, int Qx, int Qy, int Qz);

/**************************************************************
* Funckija ki mnozi dve stevili v poljublnih IQ formatih
* returns:  rezultat
* arg1:     faktor 1
* arg2:     faktor 2
* arg3:     IQ format faktorja 1
* arg4:     IQ format faktorja 2
* arg5:     IQ format rezultata
**************************************************************/
extern long mmul(long x, long y, int Qx, int Qy, int Qz);


/**************************************************************
* Funckija ki prevede stevilo iz poljubnega IQ formata 
* v poljubni IQ formati z saturacijo
* returns:  rezultat
* arg1:     stevilo
* arg2:     IQ format stevila
* arg3:     IQ format rezultata (zeljenega stevila)
**************************************************************/
extern long iqXtoiqY(long x, int Qx, int Qy);

#endif  // end of IQ_MATH_H
