#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include <stdlib.h>

static i2c_inst_t *current_i2c;
static uint8_t current_addr;

static void send_init_commands(void) {
    uint8_t init_cmds[] = {
        0x00, 0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 
        0x40, 0x8D, 0x14, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 
        0xCF, 0xD9, 0xF1, 0xDB, 0x30, 0xA4, 0xA6, 0x20, 
        0x00, 0xAF
    };
    i2c_write_blocking(current_i2c, current_addr, init_cmds, sizeof(init_cmds), false);
}

void oled_init(ssd1306_t *p, i2c_inst_t *i2c, uint8_t addr, uint_fast8_t sda_pin, uint_fast8_t scl_pin, uint8_t width, uint8_t height ) {
    current_i2c = i2c;
    current_addr = addr;

    p->width = width;
    p->height = height;
    uint32_t buffer_size = (p->width * p->height) / 8;
    p->buffer = malloc(buffer_size);

    i2c_init(current_i2c, 400 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    
    sleep_ms(10);
    send_init_commands();
}

void ssd1306_draw_pixel(ssd1306_t *p, uint32_t x, uint32_t y) {
    if (x >= p->width || y >= p->height) return;             
    p->buffer[x + p->width * (y >> 3)] |= 0x1 << (y & 0x07); 
}
void ssd1306_show(ssd1306_t *p) {
    uint32_t buffer_size = (p->width * p->height) / 8;
    uint8_t temp_buf[1025]; // 1 byte for control byte + 1024 bytes for display buffer
    temp_buf[0] = 0x40;
    
    for (int i = 0; i < buffer_size; i++){
        temp_buf[i + 1] = p->buffer[i];
    }

    i2c_write_blocking(current_i2c, current_addr, temp_buf, sizeof(temp_buf), false);
}
void ssd1306_clear(ssd1306_t *p) {
    uint32_t buffer_size = (p->width * p->height) / 8;
    for (int i = 0; i < buffer_size; i++) {
        p->buffer[i] = 0;
    }
}