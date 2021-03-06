/**************************************************************
* FILE:         SPI_dajalnik.h 
* DESCRIPTION:  definitions for SPI_dajalnik Initialization & Support Functions
* VERSION:      1.0
* AUTHOR:       Mitja Nemec
**************************************************************/
#ifndef   __SPI_DAJALNIK_H__
#define   __SPI_DAJALNIK_H__


// kateri SPI modul uporabljamo
#define SPI_MODUL       SpiaRegs

// katere pine uporabljamo
#define SPI_PIN_DAT     GpioCtrlRegs.GPAMUX2.bit.GPIO17
#define SPI_PIN_CLK     GpioCtrlRegs.GPAMUX2.bit.GPIO18
#define SPI_MUX_VALUE   1

// hitrost komunikacije ~ 2 MHz
#define SPI_CLK         2000000

// koliko bitov moramo poslati, da dobimo celotno informacijo
#define SPI_BITS        16

// koliko krat moramo poslati po n bitov (podpira samo 1 in 2
#define SPI_WORDS       1

// kje znotraj dwords se nahaja informacija
#define SPI_MSB         15
#define SPI_LSB         3

/**************************************************************
* Funckija ki inicializira komunikacijo
**************************************************************/
extern void SPI_init(void);

/**************************************************************
* Funckija ki komunicira z sPI dajalnikom polo�aja
* returns:  koda, ki jo vrne dajalnik
**************************************************************/
extern long SPI_getkot(void);

#endif  // end of __SPI_DAJALNIK_H__ definition

