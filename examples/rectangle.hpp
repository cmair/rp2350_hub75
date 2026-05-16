
#pragma once

#include "libraries/pico_graphics/pico_graphics.hpp"
#include <cstdint>
#include <math.h>

using namespace pimoroni;

class Rectangle : public PicoGraphics_PenRGB888
{

private:
    int w;
    int h;

    int x = 0;
    int y = 0;

    unsigned int color = 0xFF0000;

    void drawPixel(int x, int y, uint32_t color)
    {
        set_pen(color);
        set_pixel(Point(x, y));
    }

public:
    explicit Rectangle(uint width = 64, uint height = 64) : PicoGraphics_PenRGB888(width, height, nullptr), w(width), h(height)
    {
        set_pen(0);
        clear();
        setIntensity(1.0);
    }


    // http://members.chello.at/easyfilter/bresenham.html
    void plotLine(int x0, int y0, int x1, int y1, uint32_t color)
    {
        int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
        int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
        int err = dx+dy, e2; /* error value e_xy */

        for(;;){  /* loop */
            drawPixel(x0,y0, color);
            if (x0==x1 && y0==y1) break;
            e2 = 2*err;
            if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
                if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
        }
    }


    void draw()
    {
        plotLine(0, 0,                DISPLAY_WIDTH-1,  DISPLAY_HEIGHT-1, 0xFFFF00);
        plotLine(0, DISPLAY_HEIGHT-1, DISPLAY_WIDTH-1,  0               , 0xFFFFFF);

        plotLine(0,                 0,                  DISPLAY_WIDTH-1,    0,                  0xFF0000);
        plotLine(DISPLAY_WIDTH-1,   0,                  DISPLAY_WIDTH-1,    DISPLAY_HEIGHT-1,   0x00FF00);
        plotLine(DISPLAY_WIDTH,     DISPLAY_HEIGHT-1,   0,                  DISPLAY_HEIGHT-1,   0x0000FF);
        plotLine(0,                  DISPLAY_HEIGHT-1,  0,                  1,                  0x00FFFF);

    }
};
