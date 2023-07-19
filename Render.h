#pragma once

#include "TextRender.h"

class LCDRender : public LCDTextRender
{
    using LCDTextRender::LCDTextRender;   

public:
    void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool colour);
    void drawLine2(uint8_t x, uint8_t y, float length, float angle, bool colour);

    void drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool colour);
};