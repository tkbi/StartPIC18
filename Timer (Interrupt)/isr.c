/*******************************************************************************
 * File:        isr.c
 * Project:     SP18 - Timer
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include "main.h"

void interrupt isr_HIGH_Prio(void)
{
    if( INTCONbits.TMR0IF == 1 )
    {
        /* clear the Interrupt flag */
        INTCONbits.TMR0IF = 0;
        
        /* toggle the LED */
        LED1 = ~LED1;
    }
}
