/*******************************************************************************
 * File:        main.c
 * Project:     SP18 - LC-Display
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"
#include "lcd.h"
#include "ds1820.h"

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
 * Main Routine
 */

void main (void)
{   
    uint8_t k = 0, tValue[9];
    char buf[9];
    
    initPIC();
    initLCD();
    
    /*Shut OFF the LCD-Backlight*/
    LATCbits.LC2 = 1;
    
    charChainLCD(0, "Temp [C]");
    
    
    /*Endlosschleife*/
    while(1)
    {
        OW_MRI();
        OW_S_Rom();
        OW_W_Byte(0x44);
        __delay_ms(200);
        __delay_ms(200);
        __delay_ms(200);
        __delay_ms(200);
        OW_MRI();
        OW_S_Rom();
        OW_W_Byte(0xBE);        
        
        for(k=0; k<9; k++)
        {
            tValue[k] = OW_R_Byte();
        }
        
        tValue[0] = tValue[0] >> 1;
        
        itoa(buf,tValue[0],10);
        charChainLCD(0x40,"   ");
        charChainLCD(0x40, buf);
    }
}