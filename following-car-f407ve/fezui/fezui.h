/*
 * fez_ui.h
 *
 *  Created on: 2023年6月18日
 *      Author: xq123
 */

#ifndef FEZUI_H_
#define FEZUI_H_

#include "display.h"
#include "lefl.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "fezui_config.h"

extern uint8_t fezui_invert;

void fezui_timer_handler();
void fezui_init();

/*
 * fezui_interface.c
 */
/*
uint8_t ui_init();
void ui_clear_buf();
void ui_send_buf();
void ui_draw_rectangle(uint16_t x,uint16_t y,uint16_t w,uint16_t h,bool fill);
void ui_draw_pixel(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void ui_draw_line(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
*/

/*
 * fezui_controls.c
 */
void fezui_draw_wave(u8g2_t *u8g2_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, lefl_loop_array_t *arr, lefl_bit_array_t *l);
void fezui_draw_chart(u8g2_t *u8g2_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, lefl_loop_array_t *arr, uint8_t max);
void fezui_veil(u8g2_t *u8g2_ptr, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w,
        u8g2_uint_t h, uint8_t level, uint8_t color);

typedef struct __fezui_tile
{
    char* header;
    char* icon;
    void (*tile_cb)(struct __fezui_tile* tile);
} fezui_tile;


#endif /* FEZUI_H_ */
