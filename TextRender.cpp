#include "TextRender.h"

void LCDTextRender::putchar(char c)
{
    if(c == '\n')
    {
        cursory++;
        cursorx = 0;
        return;
    }
    //wrapping
    if(cursorx + fontWidth > LCD_WIDTH && textWrapping) cursorx = 0, cursory++;
    
    for(uint8_t i = 0; i < fontWidth; i++)
    {
        uint8_t byte = getFontPtr(font)[(c - fontOffset) * fontWidth + i];
        if(!fontColour) byte = ~byte;
        if(cursory > 5 || cursorx > LCD_WIDTH) return;
        buffer[(cursorx + i) * 6 + cursory] = byte;
    }
    if(!fontColour && cursorx + fontWidth < LCD_WIDTH) buffer[(cursorx + fontWidth) * 6 + cursory] = 0xFF;
    cursorx += fontWidth + 1;
}

void LCDTextRender::puts(const char* str)
{
    //vertical stripe of black pixels for readability
    if(!fontColour)
        buffer[(cursorx++) * 6 + cursory] = 0xFF;

    while(*str)
        putchar(*str++);
    
}

void LCDTextRender::setFont(LCDFontType_e f)
{
    font = f;
    fontWidth = getFontWidth(f);
    fontHeight = getFontHeight(f);
    fontOffset = getFontOffset(f);
}

void LCDTextRender::setCursor(uint8_t xPixels, uint8_t yBytes)
{
    cursorx = xPixels, cursory = yBytes;
}

void LCDTextRender::setTextColour(bool colour)
{
    fontColour = colour;
}

void LCDTextRender::setTextWrap(bool wrap)
{
    textWrapping = wrap;
}

size_t LCDTextRender::toStringBase10(int num, char* dest, size_t bytes) const
{
    if(bytes < 2) return 0;
    size_t written = 0;
    if(num < 0)
        *dest++ = '-', num *= -1, bytes--, written++;

    char buf[bytes] = {0};
    int i = 1;
    for(; i < bytes; i++)
    {
        uint8_t lastPlace = num % 10;
        buf[i] = lastPlace + '0';
        num /= 10;
        written++;
        if(!num) break;
    }
    for(; i >= 0; i--)
        *dest++ = buf[i];
    return written;
}

size_t LCDTextRender::toStringBase16(int num, char* dest, size_t bytes) const
{
    if(bytes < 2) return 0;
    size_t written = 0;
    if(num < 0)
        *dest++ = '-', num *= -1, bytes--, written++;

    char buf[bytes] = {0};
    int i = 1;
    for(; i < bytes; i++)
    {
        uint8_t lastPlace = num & 0xF;
        buf[i] = lastPlace > 9 ? lastPlace + 'A' - 10 : lastPlace + '0';
        num >>= 4;
        written++;
        if(!num) break;
    }
    for(; i >= 0; i--)
        *dest++ = buf[i];
    return written;
}

void LCDTextRender::putint10(int num)
{
    char buffer[13];
    toStringBase10(num, buffer, 11);
    puts(buffer);
}

void LCDTextRender::putint16(int num)
{
    char buffer[11];
    toStringBase16(num, buffer, 11);
    puts(buffer);
}