;**************************************************************
; FILE:        IQ_math.asm 
; TESCRIPTION: additional IQ math functions
; VERSION:     1.0
; AUTHOR:      Mitja Nemec
; FUNCTIONS
; extern long iqXtoiqY(long x, int Qx, int Qy)
; format conversion with saturation
; long result = iqXtoiqY(long source, int source_Q, int result_Q)
;
; extern long mdiv(long x, long y, int Qx, int Qy, int Qz)
; division of various formats with saturation
; long result mdiv(long dividend, long divisor, int dividend_Q, int divisor_Q)
;
; extern long mmul(long x, long y, int Qx, int Qy, int Qz)
; multiplication of various formats with saturation
; long result = mmul(long factor_1, long factor_2, int factor_1_Q, int factor_2_Q, result_Q)
;
;**************************************************************

   .sect   "IQmath"
;**************************************************************
; pretvorba (Qx) = (Qy)
; extern long iqXtoiqY(long x, int Qx, int Qy)
; long x == ACC
; int Qx == XAR4
; int Qy == XAR5
    .global _iqXtoiqY
_iqXtoiqY
  ; rezerviramo deset prostorov na skladu
    ADDB    SP,#6
  ; nastavmo registre
    SETC    OVM
    CLRC    TC
    ABSTC   ACC
  ; spravmo akumulator
    MOVL    *-SP[4],ACC
  ; naracunamo shift
    MOV     ACC,AR4
    SUB     ACC,AR5
    MOV     *-SP[0],ACC
    SB      poz_shift,GEQ
neg_shift
    NEG     ACC
    MOV     *-SP[0],ACC
  ; v levo
    MOVL    ACC,*-SP[4]
    CSB     ACC
    MOV     *-SP[1],T
    MOV     ACC,*-SP[1]
    SUB     ACC,*-SP[0]
    SB      sat_shift,LT
    MOV     T,*-SP[0]
    MOVL    ACC,*-SP[4]
    LSLL    ACC,T
    SB      end_shift,UNC
  ; najvisja vrednost
sat_shift
  ; najvecja vrednost
    MOV     ACC,#0xFFFF<<15
    MOV     AL,#0xFFFF
    SB      end_shift,UNC
poz_shift
  ; v desno
    MOV     T,ACC
    MOVL    ACC,*-SP[4]
    ASRL    ACC,T
end_shift
    NEGTC   ACC
  ; ponastavmo registre
    CLRC    OVM
  ; sprostimo sklad
    SUBB    SP,#6
    LRETR

;**************************************************************
; deljenje 32(Qx)/32(Qy)=32(Qz)
; extern long mdiv(long x, long y, int Qx, int Qy, int Qz)
; long x == ACC
; long y == *-SP[14]
; int Qx == AR4
; int Qy == AR5
; int Qz == *-SP[13]
    .global _mdiv
_mdiv
  ; rezerviramo deset prostorov na skladu
    ADDB    SP,#12		
  ; nastavmo registre
    SETC    OVM
    MOVL    *-SP[10],XAR2
  ; normiram x in ga shranim v *-SP[2], in nastimam TC bit
    CLRC    TC
    ABSTC   ACC
    CSB     ACC
    MOV     *-SP[0],T
    LSLL    ACC,T
    MOVL    *-SP[4],ACC
  ; normiram y in ga shranim v *-SP[4] in nastimam TC bit
    MOVL    ACC,*-SP[16]
    ABSTC   ACC
    SB      overflow_div,EQ
    CSB     ACC
    MOV     *-SP[1],T
    LSLL    ACC,T
    MOVL    *-SP[6],ACC
  ; spravim vsoto normiranj
    MOV     ACC,*-SP[0]
    SUB     ACC,*-SP[1]
    MOV     *-SP[0],ACC
  ; delim
    MOVB    ACC,#0
    MOVL    XT,*-SP[6]
    MOVL    P,*-SP[4]
    RPT     #31
  ||SUBCUL  ACC,XT
    MOVL    *-SP[8],P
    MOVB    XAR2,#0
    MOVL    P,XAR2
    RPT     #31
  ||SUBCUL  ACC,XT
    MOVL    ACC,*-SP[8]
  ; nastavim predznak
    SB      poz_div,NTC
    NEG64   ACC:P
  ; zacano spravim rezultat
poz_div
    MOVL    *-SP[6],ACC
    MOVL    *-SP[8],P
  ; naracunam zeljeni shift
    MOV     ACC,@AR4
    SUB     ACC,@AR5
    SUB     ACC,*-SP[17]
    ADD     ACC,*-SP[0]
    SB      v_levo_div,LT
    SB      nikamor_div,EQ
v_desno_div
    MOV     T,ACC
    MOVL    ACC,*-SP[6]
    MOVL    P,*-SP[8]
    ASR64   ACC:P,T
    SB      nikamor_div,UNC
v_levo_div
    NEG     ACC
    MOV     *-SP[0],ACC
    SUB     ACC,#31
    SB      overflow_div,GEQ
    MOV     T,*-SP[0]
    MOVL    ACC,*-SP[6]
    MOVL    P,*-SP[8]
    LSL64   ACC:P,T
nikamor_div
  ; ponastavimo registre
    MOVL    XAR2,*-SP[10]
    CLRC    OVM
  ; sprostimo sklad
    SUBB    SP,#12
    LRETR
overflow_div
; najvecja vrednost
    MOV     ACC,#0xFFFF<<15
    MOV     AL,#0xFFFF
    NEGTC   ACC
; ponastavimo registre
    MOVL    XAR2,*-SP[10]
    CLRC    OVM
; sprostimo sklad
    SUBB    SP,#12
    LRETR

;**************************************************************
; mnozenje 32(Qx) * 32(Qy) = 32(Qz)
; extern long mmul(long x, long y, int Qx, int Qy, int Qz)
; long x == ACC
; long y == *-SP[14]
; int Qx == AR4
; int Qy == AR5
; int Qz == *-SP[13]
    .global _mmul
_mmul
  ; rezerviramo sest prostorov na skladu
    ADDB    SP,#12		
  ; nastavmo registre
    SETC    OVM
    MOVL    *-SP[12-4],XAR2
  ; pomnozimo med seboj
    MOVL    XT,*-SP[12+4]
    IMPYL   P,XT,ACC
    QMPYL   ACC,XT,ACC
    MOVL    *-SP[12-8],ACC
    MOVL    *-SP[12-6],P
  ; prestejem predznacene bite
    CSB     ACC
    MOV     *-SP[12-11],T
    SB      sub32_mul,NEQ
    MOVL    ACC,P
    CSB     ACC
    MOV     ACC,T
    ADD     ACC,*-SP[12-11]
    MOV     *-SP[12-11],ACC
sub32_mul
; naracunam zeljeni shift
    MOV     ACC,@AR4
    ADD     ACC,@AR5
    SUB     ACC,*-SP[12+5]
    SUB     ACC,#32
    SB      v_levo_mul,LT
    SB      nikamor_mul,EQ
v_desno_mul
    MOV     T,ACC
    MOVL    ACC,*-SP[12-8]
    MOVL    P,*-SP[12-6]
    ASR64   ACC:P,T
    SB      nikamor_mul,UNC
v_levo_mul
    NEG     ACC
    MOV     *-SP[12-12],ACC
    SUB     ACC,*-SP[12-11]
    SB      overflow_mul,GT
    MOV     T,*-SP[12-12]
    MOVL    ACC,*-SP[12-8]
    MOVL    P,*-SP[12-6]
    LSL64   ACC:P,T
nikamor_mul
  ; ponastavimo registre
    MOVL    XAR2,*-SP[12-4]
    CLRC    OVM
  ; sprostimo sklad
    SUBB    SP,#12
    LRETR
overflow_mul
  ; najvecja vrednost
    MOV     ACC,#0xFFFF<<15
    MOV     AL,#0xFFFF
    AND     AH,#0x7FFF
    NEGTC   ACC
  ; ponastavimo registre
    MOVL    XAR2,*-SP[10]
    CLRC    OVM
  ; sprostimo sklad
    SUBB    SP,#12
    LRETR
