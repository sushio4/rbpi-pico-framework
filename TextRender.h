#pragma once

#include "Screen.h"
#include "Fonts.h"

inline void swapuint8_t(uint8_t& x, uint8_t& y) 
{
    uint8_t t = x;
    x = y;
    y = t;
}

class LCDTextRender : public LCDScreen
{
    using LCDScreen::LCDScreen;
public:
    void putchar(char c);
    void puts(const char* str);
    void putint10(int num);
    void putint16(int num);

    size_t toStringBase10(int num, char* dest, size_t bytes) const;
    size_t toStringBase16(int num, char* dest, size_t bytes) const;

    void setTextColour(bool colour);
    void setTextWrap(bool wrap);

    void setFont(LCDFontType_e f);
    void setCursor(uint8_t xPixels, uint8_t yBytes);

private:
    uint8_t cursorx = 0, cursory = 0;
    LCDFontType_e font = LCDFontType_Default;
    uint8_t fontWidth = 5;
    uint8_t fontHeight = 8;
    uint8_t fontOffset = 0x00;
    bool fontColour = true;
    bool textWrapping = true;
};