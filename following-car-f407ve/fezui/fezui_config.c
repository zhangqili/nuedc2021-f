/*
 * fezui_config.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"
#include "lefl.h"
#include "display.h"
#include "main.h"
#include "gpio.h"

uint8_t tempuint;

void fezui_init()
{
  u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c,
      u8x8_gpio_and_delay);
  u8g2_InitDisplay(&u8g2);
  u8g2_SetPowerSave(&u8g2, 0);
  u8g2_SetBitmapMode(&u8g2, 1);
  u8g2_SetFontMode(&u8g2, 1);
  u8g2_ClearBuffer(&u8g2);
  u8g2_SendBuffer(&u8g2);

  lefl_link_frame_navigate(&mainframe, &configpage);
}

void fezui_timer_handler()
{
  fezui_fps++;
  lefl_key_update(&key_up, !HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin));
  lefl_key_update(&key_down, !HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin));
  lefl_key_update(&key_go, !HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin));
  lefl_key_update(&key_back, !HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin));
  lefl_link_frame_logic(&mainframe);
  lefl_cursor_move(&cursor, &target_cursor);
  u8g2_ClearBuffer(&u8g2);
  if(fezui_invert)
  {
      u8g2_SetDrawColor(&u8g2, 1);
      u8g2_DrawBox(&u8g2, 0, 0, 128, 64);
      u8g2_SetDrawColor(&u8g2, !fezui_invert);
  }
  else
  {
      u8g2_SetDrawColor(&u8g2, !fezui_invert);
  }
  lefl_link_frame_draw(&mainframe);
  u8g2_SendBuffer(&u8g2);
}

void fezui_draw_cursor(lefl_cursor_t *c)
{
  u8g2_SetDrawColor(&u8g2, 2);
  u8g2_DrawBox(&u8g2, c->x, c->y, c->w, c->h);
  u8g2_SetDrawColor(&u8g2, !fezui_invert);
}



