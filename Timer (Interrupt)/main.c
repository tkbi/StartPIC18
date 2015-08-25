/*******************************************************************************
 * File:        main.c
 * Project:     SP18 - Timer
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
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
 * Konfigurieren des Timer0 zur Zeitbasiserzeugung
 */

void initTMR0(void)
{
    /* Timer0 off, internal CLK, Teiler 4 */
    T0CON = 0b00000001;
    
    /* enable TMR0 Interrupt */
    INTCONbits.TMR0IE = 1;
    
    /* high priority for TMR0 Interrupt */
    INTCON2bits.TMR0IP = 1;
    
    /* turn on TMR0 */
    T0CONbits.TMR0ON = 1;
    
    /* enable priority for Interrupts */
    RCONbits.IPEN = 1;
    
    /* enable global Interrupts */
    INTCONbits.GIEH = 1;
}

/*********************************************************************
 * Main Routine
 */

void main (void)
{   
    initPIC();
    initTMR0();
    
    /*Shut OFF the LCD-Backlight*/
    LATCbits.LC2 = 1;
    
    /*Endlosschleife*/
    while(1)
    {
    }
}