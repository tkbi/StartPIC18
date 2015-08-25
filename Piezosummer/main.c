/*******************************************************************************
 * File:        main.c
 * Project:     SP18 - Piezosummer
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>
#include "main.h"
#include "lcd.h"

#pragma config FOSC = INTIO67       // Internal oscillator block
#pragma config PWRTEN = ON          // Power up timer enabled
#pragma config BOREN = OFF          // Brown-out Reset disabled
#pragma config WDTEN = OFF          // Watch dog timer is always disabled
#pragma config MCLRE = EXTMCLR      // MCLR enabled
#pragma config LVP = OFF            // Single-Supply ICSP disabled

/*********************************************************************
 * Diverse Einstellungen zum PIC (IO, Takt, ...)
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
 * Tastenentprellung
 */

uint8_t keyPressed (uint8_t *plastKeyDown)
{
    uint8_t actKeyDown;
 
    if(PB)
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
 * Main Routine
 */

void main (void)
{   
    uint8_t lastKeyDown = 0;
    
    initPIC();
    
    /*Shut OFF the LCD-Backlight*/
    LATCbits.LC2 = 1;
    
    /*Endlosschleife*/
    while(1)
    {
        __delay_ms(10);
        
        switch( keyPressed(&lastKeyDown) )
        {
            case 0:
                break;
            case 1:
                SUMMER = ~SUMMER;
        }        
    }
}