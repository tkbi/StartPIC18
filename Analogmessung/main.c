/*******************************************************************************
 * File:        main.c
 * Project:     SP18 - Analogmessung
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include <stdlib.h>
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
    TRISB = 0x11;                   // RB0:Input (PB), RB4: Input (LDR)
    TRISC = 0x20;                   // RC6:Input (AN16)

    ANSELA = 0x00;                  // All digital
    ANSELB = 0x10;                  // AN11 on
    ANSELC = 0x20;                  // AN17 on

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

/*******************************************************************************
 * Initialisierung des Analog-Digital-Umsetzers
 */

void initADU(void)
{
    /*Nur einschalten, wenn benötigt!*/
    ADCON0bits.ADON = 0;
    ADCON2=0xB9;
}

/*******************************************************************************
 * Analog-Digital-Umsetzung
 */

uint16_t readADC(uint8_t channel)
{
    uint16_t aValue = 0;

    ADCON0bits.ADON = 1;
    switch(channel)
    {
        case AN17:
            ADCON0bits.CHS0=1;
            ADCON0bits.CHS1=0;
            ADCON0bits.CHS2=0;
            ADCON0bits.CHS3=0;
            ADCON0bits.CHS4=1;
            break;
            
        case LDR:
            ADCON0bits.CHS0=1;
            ADCON0bits.CHS1=1;
            ADCON0bits.CHS2=0;
            ADCON0bits.CHS3=1;
            ADCON0bits.CHS4=0;
            break;

        default:
            break;
    }

    ADCON0bits.GO = 1;
    while(ADCON0bits.NOT_DONE);
    aValue=ADRESH;
    aValue=aValue<<8;
    aValue=aValue+ADRESL;
    ADCON0bits.ADON = 0;

    return aValue;
}

/*********************************************************************
 * Main Routine
 */

void main (void)
{   
    uint8_t lastKeyDown = 0;
    uint16_t anValue = 0;
    char buf[9];
    
    initPIC();
    initADU();
    initLCD();
    
    /*Shut OFF the LCD-Backlight*/
    LATCbits.LC2 = 1;
    
    charChainLCD(0,"R21");
    charChainLCD(0x40,"LDR");
    
    /*Endlosschleife*/
    while(1)
    {
        __delay_ms(250);
        __delay_ms(250);
        __delay_ms(250);
        __delay_ms(250);
        
        anValue = readADC(AN17);
        itoa(buf,anValue,10);
        charChainLCD(4,"    ");
        charChainLCD(4,buf);
        
        anValue = readADC(LDR);
        itoa(buf,anValue,10);
        charChainLCD(0x44,"    ");
        charChainLCD(0x44,buf);        
    }
}