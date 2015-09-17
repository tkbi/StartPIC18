/* *****************************************************************************
 * File:        ds1820.c
 * Project:     --
 * Author:      Nicolas Meyertöns
 * Version:     1.4
 * Web:         www.PIC-Projekte.de
 * ****************************************************************************/

#include <xc.h>
#include "ds1820.h"
#include <stdint.h>
#include "main.h"

/*******************************************************************************
DS18S20 Routinen von Nicolas Meyertoens www.PIC-Projekte.de
*******************************************************************************/

uint8_t DS18S20 [9];

/*******************************************************************************
One_Wire_Master-Reset-Impuls
-------------------
Diese Funktion gibt den MR-Impuls aus und wertet aus,
ob sich mind. 1 Slave am 1-Wire-Bus befindet.
Rueckgabewert = 1 bedeutet kein Slave 0 bedeutet mind. 1 Slave
*******************************************************************************/

unsigned char OW_MRI (void)
{
    uint8_t rec;                    // Fuer den Rueckgabewert

    TRIS_DQ = 0;                    // Der DQ-IO ist ein Ausgang
    __delay_us(490);
    TRIS_DQ = 1;                    // Der DQ-IO ist ein Eingang (DQ=High (Pull-up))
    __delay_us(40);
    rec = DQ;                       // Pegel einlesen
    __delay_us(450);                // 490µs warten (Ende)

    return (rec);                   // Erkennungsbit zurueck geben
}

/*******************************************************************************
One_Wire_Write_Bit
-------------------
Diese Funktion uebertraegt 1 Bit (schreibend)
*******************************************************************************/

void OW_W_Bit (uint8_t bitvalue)
{
    DQ = 0;                         // Portbit loeschen (PORTx=0)
    TRIS_DQ = 0;                    // Der DQ-IO ist ein Ausgang (DQ=Low)

    if(bitvalue)                    // Wenn das zu uebertragende Bit eine 1 ist, dann
    {                               // 1µs warten und DQ wieder auf high ziehen
        __delay_us(1);              // Mindestens 1µs warte
        TRIS_DQ = 1;                // DQ-Leitung ist jetzt wieder High
    }

    __delay_us(100);                // 100µs warten (Ende)

    TRIS_DQ = 1;                    // Den DQ(IO) wieder auf Eingang und somit
                                    // die DQ Leitung auf High (Pull-up)
}

/*******************************************************************************
One_Wire_Write_Byte
-------------------
Diese Funktion uebertraegt 1 Byte (schreibend)
*******************************************************************************/

void OW_W_Byte (uint8_t bytevalue)
{
    uint8_t i, mask = 1;

    //Ein Byte uebertragen und mit dem LSB anfangen

    for (i=0; i<8; i++)
    {
            if (bytevalue & mask)
            {
                OW_W_Bit(1);
            }
            else
            {
                OW_W_Bit(0);
            }
            mask = (mask << 1);
    }
}

/*******************************************************************************
One_Wire_Read_Bit
-------------------
Diese Funktion uebertraegt 1 Bit (lesend)
*******************************************************************************/

uint8_t OW_R_Bit (void)
{
    uint8_t rec;

    TRIS_DQ=0;
    TRIS_DQ=1;                      // DQ sofort wieder High
    __delay_us(15);                 // 15µs warten
    rec = DQ;                       // DQ einlesen u. speichern
    __delay_us(105);                // noch 105 us warten
                                    // bis Ende Time Slot
    return(rec);                    // Rueckgabe von DQ
}

/*******************************************************************************
One_Wire_Read_Byte
-------------------
Diese Funktion uebetraegt 1 Byte (lesend)
*******************************************************************************/

uint8_t OW_R_Byte (void)
{
    uint8_t value = 0 , i;

    // 8 Bits hintereinander einlesen, LSB zuerst
    for(i=0; i<8; i++)
    {
        if (OW_R_Bit())
        {
            value |= 0x01 << i;
        }
    }

    return(value);                  // Rueckgabe von DQ
}

/*******************************************************************************
One_Wire_Skip_Rom
-------------------
Diese Funktion "adressiert" alle Sensoren
*******************************************************************************/

void OW_S_Rom(void)
{
    // Skip ROM-Befehl aussenden: alle Slaves reagieren dann
    // auf den danach folgenden Befehl:
    OW_W_Byte(0xCC);
}