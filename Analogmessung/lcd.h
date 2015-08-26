/*******************************************************************************
 * File:        lcd.h
 * Project:     SP18 - Analogmessung
 * Author:      Nicolas Meyertöns
 * Version:     siehe main.h
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#ifndef LCD_H
#define	LCD_H

/*
 * Hier müsst ihr die Anschlüsse anpassen, wie sie bei euch vorliegen
 * 
 * ACHTUNG!	
 * In der aktuellen Version (1.5.xc8) müssen(!) die vier Datenleitung 
 * auf den unteren 4 Bit des Ports angeschlossen sein. Das ist wichtig,
 * da in der Funktion writeLCD erst das obere und danach das untere
 * Nibble des Worts übertragen werden. Und die Übertragung erfolgt über
 * die unteren 4 Bit eines Ports!
 * In der nachfolgenden Version dieser Bibliothek wird ein Anschluss 
 * an beiden Nibblen des Ports möglich sein.
 */
 
#define LCD_RS      LATAbits.LATA6
#define LCD_RW      LATAbits.LATA5
#define LCD_E       LATAbits.LATA4
// Bitte den obigen Hinweis zum Anschluss beachten!
#define LCD_DATA    LATA

void initLCD(void);
void writeLCD(unsigned char Info, unsigned char k);
void enableLCD(void);
void charChainLCD (unsigned char adr, const char *pText);

#endif	/* LCD_H */