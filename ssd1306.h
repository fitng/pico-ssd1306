#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include "hardware/i2c.h"

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t *buffer;
} ssd1306_t;

void oled_init(ssd1306_t *p, i2c_inst_t *i2c, uint8_t addr, uint_fast8_t sda_pin, uint_fast8_t scl_pin);

void ssd1306_draw_pixel(ssd1306_t *p, uint32_t x, uint32_t y);

void ssd1306_show(ssd1306_t *p);

#endif