/*******************************************************************************
 * File:        main.c
 * Project:     SP18 - Ein- und Ausgaenge
 * Author:      Nicolas Pannwitz
 * Version:     look at main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>
#include "main.h"

#pragma config FOSC = INTIO67       // Internal oscillator block
#pragma config PWRTEN = ON          // Power up timer enabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config WDTEN = OFF          // Watch dog timer is always disabled
#pragma config MCLRE = EXTMCLR      // MCLR enabled
#pragma config LVP = OFF            // Single-Supply ICSP disabled

/*********************************************************************
 * main settings
 */

void initPIC(void)
{
    TRISA = 0x00;
    TRISB = 0x01;                   // RB0:Input (PB)
    TRISC = 0x20;                   // RC6:Input (AN16)

    ANSELA = 0x00;                  // All digital
    ANSELB = 0x00;                  // ...
    ANSELC = 0x20;                  // AN16 on

    OSCCON2bits.MFIOSEL = 0;        // 111: 16 MHz
    OSCCONbits.IRCF2 = 1;           // 110:  8 MHz
    OSCCONbits.IRCF1 = 0;           // 101:  4 MHz
    OSCCONbits.IRCF0 = 0;           // 100:  2 MHz <-- aktiv
}

/*********************************************************************
 * debouncing
 */

uint8_t keyPressed (uint8_t *plastKeyDown)
{
    uint8_t actKeyDown;
 
    if(!PB)
    {
        actKeyDown = 1;
    }
    else
    {
        actKeyDown = 0;
    }
 
    if(actKeyDown != *plastKeyDown)
    {
        *plastKeyDown = actKeyDown;
        return actKeyDown;
    }
 
    return 0;
} 

/*********************************************************************
 * main
 */

void main (void)
{
    uint8_t lastKeyDown = 0;
    
    initPIC();
    
    /* turn off the LCD backlight */
    LATCbits.LC2 = 1;
    
    /* endless loop */
    while(1)
    {
        __delay_ms(10);
        
        switch( keyPressed(&lastKeyDown) )
        {
            // push button pressed
            case 1: LED1 = ~LED1;

            default: break;
        }
    }
}
