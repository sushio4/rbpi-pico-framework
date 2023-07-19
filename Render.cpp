#include "Render.h"
#include <math.h>

void LCDRender::drawLine2(uint8_t x, uint8_t y, float length, float angle, bool colour)
{
    drawLine(x, y, x + length * cos(angle), y + length * sin(angle), colour);
}

void LCDRender::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool colour)
{
    bool switched = false;
    //primary and secondary coordinates
    uint8_t p0 = x0, p1 = x1, 
            s0 = y0, s1 = y1;
    //set which is primary to avoid dividing by 0
    if(abs(x0 - x1) < abs(y0 - y1))
    {
        p0 = y0, s0 = x0;
        p1 = y1, s1 = x1;
        switched = true;
    }
    //swap so that p0 < p1
    if(p0 > p1)
    {
        swapuint8_t(p0, p1);
        swapuint8_t(s0, s1);
    }

    int8_t deltas = s1 > s0 ? 1 : -1;

    float dsdp = (float)(s1 - s0)/(float)(p1 - p0);
    for(uint8_t p = p0, s = s0; p < p1; p++)
    {
        float value = dsdp * (p - p0) + s0;
        if(fabs(value - s) > fabs(s + deltas - value)) s += deltas;
        //draw point
        if(switched) setPixel(s, p, colour);
        else         setPixel(p, s, colour);
    }
}

void LCDRender::drawRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool colour)
{
    if(x0 > x1) swapuint8_t(x0, x1);
    if(y0 > y1) swapuint8_t(y0, y1);

    for(uint8_t x = x0; x <= x1; x++)
        for(uint8_t y = y0; y <= y1; y++)
            setPixel(x,y, colour);
}