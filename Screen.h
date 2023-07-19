#pragma once

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#define LCD_BLACK   1
#define LCD_WHITE   0

#define LCD_WIDTH   84
#define LCD_HEIGHT  48
#define LCD_BUFFER_SIZE 504

#define LCD_POWERDOWN            0x04
#define LCD_ENTRYMODE            0x02
#define LCD_EXTENDEDINSTRUCTION  0x01

#define LCD_DISPLAYBLANK         0x00
#define LCD_DISPLAYNORMAL        0x04
#define LCD_DISPLAYALLON         0x01
#define LCD_DISPLAYINVERTED      0x05

#define LCD_FUNCTIONSET          0x20
#define LCD_DISPLAYCONTROL       0x08
#define LCD_SETYADDR             0x40
#define LCD_SETXADDR             0x80

#define LCD_SETTEMP  0x04
#define LCD_SETBIAS  0x13
#define LCD_SETVOP   0x80

#define LCD_CONTRAST 0xB0
#define LCD_BIAS     0x13

class LCDScreen
{
public:
    LCDScreen(uint8_t LCD_RST, uint8_t LCD_DC);
    void initScreen();

    void fill(bool colour);
    void setPixel(uint8_t x, uint8_t y, bool colour);

    inline void writeCommand(uint8_t command) const;

    void drawBuffer() const;
    
protected:
    uint8_t buffer[LCD_BUFFER_SIZE] = {0};

private: //fields
    uint8_t RST, DC;
};