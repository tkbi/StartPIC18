/*******************************************************************************
 * File:        lcd.c
 * Project:     SP18 - Analogmessung
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include "lcd.h"
#include "main.h"

/*********************************************************************
 * Initialisierung
 */
void initLCD(void)
{
    LCD_RS = LCD_RW = LCD_E = 0;

    __delay_ms(15);
    // #1
    LCD_DATA = 0b00000011;		// Auf 8 Bit setzten
    enableLCD();
    __delay_ms(5);
    // #2
    LCD_DATA = 0b00000011;		// ..
    enableLCD();
    __delay_ms(1);
    // #3
    LCD_DATA = 0b00000011;		// ..
    enableLCD();
    __delay_ms(1);
    LCD_DATA = 0b00000010;		// Auf 4 Bit setzen
    enableLCD();
    __delay_ms(5);

    writeLCD(0x28,0);			// Functionset (2 Zeilien)
    writeLCD(0x0C,0);			// Display on
    writeLCD(0x01,0);			// Display clear
    __delay_ms(2);              // 2ms Pause
    writeLCD(0x06,0);			// Entry Mode
    __delay_ms(1);
}

/*********************************************************************
 * Datenuebertragung zum LCD-Controller
 *
 * Parameter:   byte - Das zu übertragende Datenpaket
 *              k - Für Befehle 0, für Daten 1
 */
void writeLCD(unsigned char byte, unsigned char k)
{
    LCD_RW = 0;
    LCD_RS = 0;
    __delay_ms(1);

    // Senden des oberen Nibbles
    LCD_DATA = (byte>>4)&0x0F;
    if (k) LCD_RS = 1;
    enableLCD();

    // Senden des unteren Nibbles
    LCD_DATA= byte&0x0F;
    if (k) LCD_RS = 1;
    enableLCD();
}

/*********************************************************************
 * Schreiben einer Zeichenkette auf das Display an eine gewünschte Adresse
 *
 * Parameter:   adr - Die Adresse an die der Text geschrieben werden soll
 *              Text - Ein Zeiger auf die zu schreibende Zeichenfolge
 */
void charChainLCD (unsigned char adr, const char *pText)
{
    writeLCD(0x80|adr,0);

    while (*pText)
    {
        writeLCD(*pText,1);
        pText++;
    }
}

/*********************************************************************
 * Erzeugen eines Enable Strobbes, der Dauer 20µs
 */
void enableLCD(void)
{
    LCD_E = 1;
    __delay_us(20);
    LCD_E = 0;
}
