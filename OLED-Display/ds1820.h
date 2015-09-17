/* *****************************************************************************
 * File:        ds1820.h
 * Project:     --
 * Author:      Nicolas Meyertöns
 * Version:     1.4
 * Web:         www.PIC-Projekte.de
 * ****************************************************************************/

#ifndef DS1820
#define	DS1820

#include <stdint.h>

/*Makros*/
#define LAT_DQ          LATAbits.LA7
#define DQ              PORTAbits.RA7
#define TRIS_DQ         TRISAbits.TRISA7

/*Bekannmachen globaler Variablen*/
extern uint8_t DS18S20 [9];

/*Prototypen*/
uint8_t OW_MRI (void);
void OW_W_Bit (uint8_t bitvalue);
void OW_W_Byte (uint8_t bytevalue);
uint8_t OW_R_Bit (void);
uint8_t OW_R_Byte (void);
void OW_S_Rom(void);

#endif	/* DS1820.H */