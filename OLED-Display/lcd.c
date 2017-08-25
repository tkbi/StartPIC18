/*******************************************************************************
 * File:        lcd.c
 * Project:     SP18 - I2C OLED Display
 * Author:      Nicolas Meyertöns
 * Version:     
 * Web:         http://pic-projekte.de
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>
#include "lcd.h"
#include "peri.h"
#include "font.h"

/*******************************************************************************
 * Framebuffer
 */

uint8_t buffer[1024];

/*******************************************************************************
 * Initialisierung des OLED-Displays
 */

void lcd_init(void)
{
    //i2c.init(SSD1306_DEFAULT_ADDRESS);

    // Turn display off
    lcd_sendCommand(SSD1306_DISPLAYOFF);

    lcd_sendCommand(SSD1306_SETDISPLAYCLOCKDIV);
    lcd_sendCommand(0x80);

    lcd_sendCommand(SSD1306_SETMULTIPLEX);
    lcd_sendCommand(0x3F);
    
    lcd_sendCommand(SSD1306_SETDISPLAYOFFSET);
    lcd_sendCommand(0x00);
    
    lcd_sendCommand(SSD1306_SETSTARTLINE | 0x00);
    
    // We use internal charge pump
    lcd_sendCommand(SSD1306_CHARGEPUMP);
    lcd_sendCommand(0x14);
    
    // Horizontal memory mode
    lcd_sendCommand(SSD1306_MEMORYMODE);
    lcd_sendCommand(0x00);
    
    lcd_sendCommand(SSD1306_SEGREMAP | 0x01);

    lcd_sendCommand(SSD1306_COMSCANDEC);

    lcd_sendCommand(SSD1306_SETCOMPINS);
    lcd_sendCommand(0x12);

    // Max contrast
    lcd_sendCommand(SSD1306_SETCONTRAST);
    lcd_sendCommand(0xCF);

    lcd_sendCommand(SSD1306_SETPRECHARGE);
    lcd_sendCommand(0xF1);

    lcd_sendCommand(SSD1306_SETVCOMDETECT);
    lcd_sendCommand(0x40);

    lcd_sendCommand(SSD1306_DISPLAYALLON_RESUME);

    // Non-inverted display
    lcd_sendCommand(SSD1306_NORMALDISPLAY);

    // Turn display back on
    lcd_sendCommand(SSD1306_DISPLAYON);
}

/*******************************************************************************
 * Übertragen eines Befehls an das OLED-Display
 */

void lcd_sendCommand(uint8_t command)
{
    startI2C();
    sendI2C(SSD1306_DEFAULT_ADDRESS);
    
    sendI2C(0x00);
    sendI2C(command);
    
    stopI2C();
}

/*******************************************************************************
 * Invertierung des OLED-Display
 */

void lcd_invert(uint8_t inverted)
{
    if (inverted)
    {
        lcd_sendCommand(SSD1306_INVERTDISPLAY);
    }
    else
    {
        lcd_sendCommand(SSD1306_NORMALDISPLAY);
    }
}

/*******************************************************************************
 * Übertragen des Framebuffers an das OLED-Display
 */

void lcd_sendFramebuffer(uint8_t *buffer)
{
    lcd_sendCommand(SSD1306_COLUMNADDR);
    lcd_sendCommand(0x00);
    lcd_sendCommand(0x7F);

    lcd_sendCommand(SSD1306_PAGEADDR);
    lcd_sendCommand(0x00);
    lcd_sendCommand(0x07);

    // We have to send the buffer as 16 bytes packets
    // Our buffer is 1024 bytes long, 1024/16 = 64
    // We have to send 64 packets
    
    for (uint8_t packet = 0; packet < 64; packet++)
    {
        startI2C();
        sendI2C(SSD1306_DEFAULT_ADDRESS);
        sendI2C(0x40);
        
        for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte)
        {
            sendI2C(buffer[packet*16+packet_byte]);
        }
        
        stopI2C();
    }
}

/*******************************************************************************
 * Zeichnen eines Pixels in den Framebuffer an die Position (pos_x|pos_y).
 * Setzten des Pixels mit pixel_status = 1, loeschen mit pixel_status = 0
 */

void fb_drawPixel(uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status)
{
    if (pos_x >= SSD1306_WIDTH || pos_y >= SSD1306_HEIGHT)
    {
        return;
    }

    if (pixel_status)
    {
        buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] |= (1 << (pos_y&7));
    }
    else
    {
        buffer[pos_x+(pos_y/8)*SSD1306_WIDTH] &= ~(1 << (pos_y&7));
    }
}

/*******************************************************************************
 * Zeichnen einer vertikalen Linie in den Framebuffer von (x,y) der Laenge 
 * length.
 */

void fb_drawVLine(uint8_t x, uint8_t y, uint8_t length)
{
    for (uint8_t i = 0; i < length; ++i)
    {
        fb_drawPixel(x, i+y, 1);
    }
}

/*******************************************************************************
 * Zeichnen einer horizontalen Linie in den Framebuffer von (x|y) der Laenge
 * length.
 */

void fb_drawHLine(uint8_t x, uint8_t y, uint8_t length)
{
    for (uint8_t i = 0; i < length; ++i)
    {
        fb_drawPixel(i+x, y, 1);
    }
}

/*******************************************************************************
 * Zeichnen eines Rechtecks in den Framebuffer von (x1|y1) zu (x2|y2).
 * Das Rechteck kann gefuellt werden (1 = fill) oder leer sein (0 = fill).
 */

void fb_drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t fill)
{
    uint8_t length = x2 - x1 + 1;
    uint8_t height = y2 - y1;
        
    if (!fill)
    {
        fb_drawHLine(x1, y1, length);
        fb_drawHLine(x1, y2, length);
        fb_drawVLine(x1, y1, height);
        fb_drawVLine(x2, y1, height);
    }
    else
    {
        for (int x = 0; x < length; ++x)
        {
            for (int y = 0; y <= height; ++y)
            {
                fb_drawPixel(x1+x, y+y1, 1);
            }
        }
    }
}

/*******************************************************************************
 * Löschen des Framebuffers
 */

void fb_clear()
{
    for (uint16_t buffer_location = 0; buffer_location < SSD1306_BUFFERSIZE; buffer_location++)
    {
        buffer[buffer_location] = 0x00;
    }
}

/*******************************************************************************
 * Invertieren des Framebuffers
 */

void fb_invert(uint8_t status)
{
    lcd_invert(status);
}

/*******************************************************************************
 * Darstellen des Framebuffers auf dem OLED-Display
 */

void fb_show()
{
    lcd_sendFramebuffer(buffer);
}

/*******************************************************************************
 * Zeichnen eines Zeichens an Position (x,y) - Diese Funktion kann nur indirekt
 * mit Hilfe der Funktion lcd_draw_string aufgerufen werden, da diese den 
 * entsprechenden Index fuer den font-Vector berechnet!
 */

void fb_draw_char (uint16_t x, uint16_t y, uint16_t fIndex)
{
    uint16_t bufIndex = (y << 7) + x;
    uint8_t j;
    
    for(j=0; j < FONT_WIDTH; j++)
    {
        buffer[bufIndex + j] = font[fIndex + j + 1];
    }
}

/*******************************************************************************
 * Zeichnen einer Zeichenkette ab Position (x,y)
 */

void fb_draw_string (uint16_t x, uint16_t y, const char *pS)
{    
    uint16_t lIndex, k;
    
    while(*pS)
    {
        /* index the width information of character <c> */
        lIndex = 0;
        for(k=0; k < (*pS - ' '); k++)
        {
            lIndex += (font[lIndex]) + 1;
        }
        
        /* draw character */
        fb_draw_char(x, y, lIndex);
        
        /* move the cursor forward for the next character */
        x += font[lIndex] + 1;
        
        /* next charachter */
        pS++;
    }
}

void fb_draw_string_big (uint16_t x, uint16_t y, const char *pS)
{
    uint8_t k;

    while(*pS)
    {
        for(k=0; k<10; k++)
        {
            buffer[( y    << 7) + x + k] = font2[*pS - ' '][k*2  ];
            buffer[((y+1) << 7) + x + k] = font2[*pS - ' '][k*2+1];
        }
        
        x += 10;
        
        /* next charachter */
        pS++;
    }    
}
