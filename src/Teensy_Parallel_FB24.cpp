
#include "Teensy_Parallel_GFX.h"


//=============================================================================
// 16 bit version of the Frame buffer functions
//=============================================================================

inline uint32_t color565To888(uint16_t color) {
    uint8_t r, g, b;
    Teensy_Parallel_GFX::color565toRGB(color, r, g, b);
    if (r) r |= 0x7;
    if (g) g |= 0x3;
    if (b) b |= 0x7;
    return 0xff000000 | (r << 16) | (g << 8) | b;
}


void Teensy_Parallel_FB24::drawPixel(int16_t x, int16_t y, uint16_t color) {
    drawPixel24(x, y, color565To888(color));
}

void Teensy_Parallel_FB24::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawFastVLine24(x, y, h, color565To888(color));
}

void Teensy_Parallel_FB24::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawFastHLine24(x, y, w, color565To888(color));
}

// 
void Teensy_Parallel_FB24::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    fillRect24(x, y, w, h, color565To888(color));
}

void Teensy_Parallel_FB24::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors) {
    if (((x + w) < 0) || ((y + h) < 0) || (x >= _width) || (y >= _height) || (w <= 0) || (h <= 0)) return;
    int16_t w_image = w; // remember width of image coming in.
    if (y < 0) {
        h += y; 
        pcolors += (-y) * w_image;
        y = 0;
    }
    if (x < 0) {
        w += x;
        pcolors += -x; 
        x = 0;
    }
    uint16_t *pfb = (uint16_t*)_fb;
    uint16_t *pfbRow = &pfb[y * (int)_width + x];
    uint16_t *pcolors_row = pcolors;
    for (int16_t iy = 0; iy < h; iy++) {
        if ((y+iy) >= _height) break;
        pfb = pfbRow;
        pcolors = pcolors_row;
        for (int16_t ix = 0; ix < w; ix++) {
            if ((x+ix) >= _width) break;
            uint16_t color = *pcolors++;
            *pfb++ = color565To888(color);
        }
        pfbRow += _width; // setup for next row
        pcolors_row += w_image; // setup for next row.
    }
}


//=============================================================================
// 32 bit (RGB8888) version of the Frame buffer functions
//=============================================================================

void Teensy_Parallel_FB24::drawPixel24(int16_t x, int16_t y, uint32_t color) {
    if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
    uint32_t *pfb = (uint32_t*)_fb;
    pfb[y * (int)_width + x] = color;
}

void Teensy_Parallel_FB24::drawFastVLine24(int16_t x, int16_t y, int16_t h, uint32_t color) {
    if ((x < 0) || ((y + h) < 0) || (x >= _width) || (y >= _height) || (h <= 0)) return;
    
    uint32_t *pfb = (uint32_t*)_fb;
    pfb = &pfb[y * (int)_width + x];
    if (y < 0) {
        h += y; 
        y = 0;
    }
    while (h--) {
        if (y >= _height) break;
        *pfb = color;
        pfb += _width;
        y++;
    }

}

void Teensy_Parallel_FB24::drawFastHLine24(int16_t x, int16_t y, int16_t w, uint32_t color) {
    if (((x + w) < 0) || (y < 0) || (x >= _width) || (y >= _height) || (w <= 0)) return;
    uint32_t *pfb = (uint32_t*)_fb;
    if (x < 0) {
        w += x;
        x = 0;
    }
    pfb = &pfb[y * (int)_width + x];
    while (w--) {
        if (x >= _width) break;
        *pfb++ = color;
        x++;
    }

}

void Teensy_Parallel_FB24::fillRect24(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color) {
    if (((x + w) < 0) || ((y + h) < 0) || (x >= _width) || (y >= _height) || (w <= 0) || (h <= 0)) return;
    if (y < 0) {
        h += y; 
        y = 0;
    }
    if (x < 0) {
        w += x;
        x = 0;
    }
    uint32_t *pfb = (uint32_t*)_fb;
    uint32_t *pfbRow = &pfb[y * (int)_width + x];
    for (int16_t iy = 0; iy < h; iy++) {
        if ((y+iy) >= _height) break;
        pfb = pfbRow;
        for (int16_t ix = 0; ix < w; ix++) {
            if ((x+ix) >= _width) break;
            *pfb++ = color;
        }
        pfbRow += _width; // setup for next row
    }
}

void Teensy_Parallel_FB24::writeRect24(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t *pcolors) {
    if (((x + w) < 0) || ((y + h) < 0) || (x >= _width) || (y >= _height) || (w <= 0) || (h <= 0)) return;
    int16_t w_image = w; // remember width of image coming in.
    if (y < 0) {
        h += y; 
        pcolors += (-y) * w_image;
        y = 0;
    }
    if (x < 0) {
        w += x;
        pcolors += -x; 
        x = 0;
    }
    uint32_t *pfb = (uint32_t*)_fb;
    uint32_t *pfbRow = &pfb[y * (int)_width + x];
    uint32_t *pcolors_row = pcolors;
    for (int16_t iy = 0; iy < h; iy++) {
        if ((y+iy) >= _height) break;
        pfb = pfbRow;
        pcolors = pcolors_row;
        for (int16_t ix = 0; ix < w; ix++) {
            if ((x+ix) >= _width) break;
            *pfb++ = *pcolors++;
        }
        pfbRow += _width; // setup for next row
        pcolors_row += w_image; // setup for next row.
    }
}

