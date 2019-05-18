#ifndef _SETTINGS_
#define _SETTINGS_

//type of MPU
#include <33FJ128GP802.h>

//fuses to set
#FUSES WPRES32                  //Watch Dog Timer PreScalar 1:32
#FUSES WPOSTS1                  //Watch Dog Timer PostScalar 1:1
#FUSES NOPROTECT                //General Segment Code Protect No Security
#FUSES CKSFSM                   //Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOIOL1WAY                //can reconfigure pins more than once. should we allow this?
#FUSES ICSP3                    //talk through com 3

//osc. for xtal use xtal switching after booting
#FUSES HS                       //HS Xtal
#FUSES FRC                      //FRC without PLL
#FUSES OSCIO                    //OSC2 is not osc out

//#define DEBUG 1
//#define DEBUG_DISPLAY_READ_PROGRAM_MEMORY 1 //will print program memory to stdio every time program memory is read
//#define SIMULATOR 1

// Configure Oscillator to operate the device at 40MIPS using internal RC osilator
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= 12*80/(2*6)=80Mhz for 12MHZ input clock
#define SETTINGS_PLLFBD 78   // M=80
#define SETTINGS_PLLPOST 0   // N1=2
#define SETTINGS_PLLPRE  4   // N2=6
#define SETTINGS_OSCTUN  0   // Adjust to correct frequency
#use delay(clock=80000000)   // this needs to match Fosc

#define TIMEOUT_MSW 0x0008 //this is in Fcy/256. For 40MIPS this is about 265ms
#define TIMEOUT_LSW 0xFFFF
#define UART_BITRATE 921600 //not sure what baudrate we should use. It really depends what the PC can do as we can go pretty fast with a 80MHz clk.
//#define UART_BITRATE 115200
//#define UART_BITRATE 460800 

//lets say 0,0 means dspic33fj128GP802 with hardware 1.0 and version 1.0 of the bootloader
#define BL_ID_LOW 0
#define BL_ID_HIGH 0
#define BL_ID_CHK 71 //some sort of crc (it's the last 8 bits of Iso3309)

//some codes that we may use for comms
#define SOH        'S'//start of comms
#define EOT        'E'//end of comms
#define ACKLOD     'A'//packet received aok
#define NAKLOD     'N'//comms error
#define READY      0x52 //'R'// after a slow operation such as writeing a page to flash the big PC needs to wait for this b4 sending more data. the big pc needs to work out itself when these operations will take place.
#define WRITE_FAIL 0x46 //'F'//as in writing to the flash
#define ERROR_UNKNOWN_TYPE 'U'
#define ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA 'B'
#define ESC    0x1B
#define SYNC   0x02

//these are for asm. this is the only way i have figured out how to use defines in asm. the other things have to be defined as a number such as 0x41 not 'A'
#define HASH_SOH                                                                #SOH
#define HASH_EOT                                                                #EOT
#define HASH_ACKLOD                                                             #ACKLOD
#define HASH_NAKLOD                                                             #NAKLOD
#define HASH_READY                                                              #READY
#define HASH_WRITE_FAIL                                                         #WRITE_FAIL
#define HASH_ERROR_UNKNOWN_TYPE                                                 #ERROR_UNKNOWN_TYPE
#define HASH_ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA  #ERROR_BOOTLOADER_ADDRESS_OR_OUT_OF_BOUNDS_ADDRESS_OR_LACK_OF_DATA
#define HASH_ESC                                                                #ESC
#define HASH_SYNC                                                               #SYNC


//if we want to use LEDS. Of course we do.
#define USE_LEDS

//select what board you have in hal.h
#define HW_1

//define hardware connections
#include "hal.h"



#endif

