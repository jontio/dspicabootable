#ifndef _HAL_
#define _HAL_

#include "picregs.h"

#ifdef HW_1

//dspic needs this to define what pin does what
#pin_select U1TX=PIN_B5
#pin_select U1RX=PIN_B6

#bit RED_LED = LATA.0
#bit RED_LED_DIRECTION = TRISA.0
#bit GREEN_LED = LATA.1
#bit GREEN_LED_DIRECTION = TRISA.1
#define RED_LED_ON() RED_LED=0
#define GREEN_LED_ON() GREEN_LED=0
#define RED_LED_OFF() RED_LED=1
#define GREEN_LED_OFF() GREEN_LED=1

#bit BTN_A = LATB.3
#bit BTN_A_DIRECTION = TRISB.3
#bit BTN_B = LATB.4
#bit BTN_B_DIRECTION = TRISB.4

//setup other things just for PCWHD
#USE FAST_IO (ALL)
#use rs232(UART1,baud=UART_BITRATE,parity=N,bits=8)

#endif

#endif
