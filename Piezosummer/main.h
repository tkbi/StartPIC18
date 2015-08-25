/*******************************************************************************
 * File:        main.h
 * Project:     SP18 - Piezosummer
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

#define _XTAL_FREQ  2000000 /*Clock frequency in Hz*/

#endif	/* MAIN_H */

