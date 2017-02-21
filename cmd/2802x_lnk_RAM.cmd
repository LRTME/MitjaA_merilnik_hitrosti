/****************************************************************
* FILENAME:     2802x_lnk_RAM.cmd
* DESCRIPTION:  device linker command file 
* AUTHOR:       Mitja Nemec
* NOTES:        based upon DSP28027 Header Files
*
****************************************************************/

MEMORY
{
PAGE 0 :
    RESET_M     : origin = 0x000000, length = 0x000002

    OTP         : origin = 0x3D7800, length = 0x000400     /* on-chip OTP */
    FLASHA      : origin = 0x3F7000, length = 0x000F80     /* on-chip FLASH */
    CSM_RSVD    : origin = 0x3F7F80, length = 0x000076     /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */
    BEGIN       : origin = 0x3F7FF6, length = 0x000002     /* Part of FLASHA.  Used for "boot to Flash" bootloader mode. */
    CSM_PWL_P0  : origin = 0x3F7FF8, length = 0x000008     /* Part of FLASHA.  CSM password locations in FLASHA */
    FLASHB      : origin = 0x3F6000, length = 0x001000     /* on-chip FLASH */

    IQTABLES    : origin = 0x3FE000, length = 0x000B50     /* IQ Math Tables in Boot ROM */
    IQTABLES2   : origin = 0x3FEB50, length = 0x00008C     /* IQ Math Tables in Boot ROM */
    IQTABLES3   : origin = 0x3FEBDC, length = 0x0000AA      /* IQ Math Tables in Boot ROM */

    ROM         : origin = 0x3FF27C, length = 0x000D44     /* Boot ROM */
    RESET       : origin = 0x3FFFC0, length = 0x000002     /* part of boot ROM  */
    VECTORS     : origin = 0x3FFFC2, length = 0x00003E     /* part of boot ROM  */

    RAML0       : origin = 0x008000, length = 0x001000     /* on-chip RAM block L0 */
    
PAGE 1 :
    BOOT_RSVD   : origin = 0x000000, length = 0x000050     /* Part of M0, BOOT rom will use this for stack */
    RAMM0       : origin = 0x000050, length = 0x0003B0     /* on-chip RAM block M0 */
    RAMM1       : origin = 0x000400, length = 0x000400     /* on-chip RAM block M1 */
}

SECTIONS
{
/* VARIABLES */
    .bss: >         RAMM1,           PAGE = 1
    .ebss: >        RAMM1,           PAGE = 1

/* CODE */
    .text: >        RAML0,          PAGE = 0, ALIGN(4) 

    ramfuncs:       {
                        *(ramfuncs)
                        *(.TI.ramfunc)
                        *(IQmath)
                    }
                    LOAD = RAML0,
                    RUN = RAML0,
                    LOAD_START(_RamfuncsLoadStart),
                    LOAD_SIZE(_RamfuncsLoadSize),
                    LOAD_END(_RamfuncsLoadEnd),
                    RUN_START(_RamfuncsRunStart),
                    RUN_SIZE(_RamfuncsRunSize),
                    RUN_END(_RamfuncsRunEnd),
                    PAGE = 0, ALIGN(4)
                    
/* IQ MATH TABLES */
    IQmathTables: > IQTABLES,       PAGE = 0, type = NOLOAD
  
    IQmathTables2: >IQTABLES2,      PAGE = 0, TYPE = NOLOAD
    IQmathTables3: >IQTABLES3,      PAGE = 0, TYPE = NOLOAD

/* STACK */
    .stack: >       RAMM0,
                    RUN_START(_stack_start),
                    RUN_END(_stack_end),       
                    PAGE = 1
                    
/* HEAP */  
    .sysmem: >      RAMM1,          PAGE = 1
    .esysmem: >     RAMM1,          PAGE = 1

/* CONSTANTS */
    .econst: >      RAML0,          PAGE = 0      
    .const: >       RAML0,          PAGE = 0

/* STARTUP CODE */
    codestart >     BEGIN           PAGE = 0
    .reset: >       RAML0,          PAGE = 0
    .cinit: >       RAML0,          PAGE = 0

/* COMPILER GENERATED CODE */
    .pinit: >       RAML0,          PAGE = 0
    .switch: >      RAML0,          PAGE = 0

/* PASSWORDS */
    csmpasswds: >   CSM_PWL_P0,     PAGE = 0, type = NOLOAD
    csm_rsvd: >     CSM_RSVD,       PAGE = 0, type = NOLOAD
}

/*
//===========================================================================
// End of file.
//===========================================================================
*/
