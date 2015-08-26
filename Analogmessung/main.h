/*******************************************************************************
 * File:        main.h
 * Project:     SP18 - Analogmessung
 * Author:      Nicolas Meyertöns
 * Version:     1.0
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>

/*********************************************************************
 * Nuetzliche Markos definieren (erleichtert das Schreiben/Lesen des Codes)
 */

#define LED1        LATBbits.LB1
#define PB          PORTBbits.RB0
#define SUMMER      LATBbits.LB3

#define AN17        0   // R21
#define LDR         1   // AN11

#define _XTAL_FREQ  2000000 /*Clock frequency in Hz*/

#endif	/* MAIN_H */

