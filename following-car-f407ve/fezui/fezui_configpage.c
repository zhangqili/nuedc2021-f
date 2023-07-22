/*
 * fezui_menupage.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#include "fezui.h"
#include "gpio.h"
#include "main.h"
#include "pid_control.h"
#include "stage.h"
#include "motor_control.h"
#include "communication.h"

lefl_page_t configpage =
{ configpage_logic, configpage_draw, configpage_load };

lefl_menu_t configmenu =
{ .items =
{ "Home", "Oscilloscope", "Statistic", "Settings" }, .selected_index = 0, .len =
    4, .menu_cb = config_menu_cb };

float menu_offset;

lefl_animation_base_t menuanimation =
{ .easing_func = lefl_animation_cubic_ease, .from = 100, .to = 0, .end = 50,
    .tick = 0, .parameter1 = 3.0, .parameter2 = 3.0, .mode =
        LEFL_ANIMATION_EASE_OUT, .target = &menu_offset, };

void configpage_logic(lefl_page_t *page)
{
  lefl_animation_tick(&menuanimation);

  lefl_cursor_set(&target_cursor, 3 + (int16_t) menu_offset,
      configmenu.selected_index * ITEM_HEIGHT + 3,
      strlen(configmenu.items[configmenu.selected_index]) * 6 + 6,
      ITEM_HEIGHT);
}
void configpage_draw(lefl_page_t *page)
{
  u8g2_SetFont(&u8g2, u8g2_font_6x12_tf);

  sprintf(fezui_tempstr,"L:%d",motor_l.Encoder);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT, fezui_tempstr);

  sprintf(fezui_tempstr,"R:%d",motor_r.Encoder);
  u8g2_DrawStr(&u8g2, 43, ITEM_HEIGHT, fezui_tempstr);

  sprintf(fezui_tempstr,"C:%d",color_flag);
  u8g2_DrawStr(&u8g2, 88, ITEM_HEIGHT, fezui_tempstr);

  sprintf(fezui_tempstr,"b_err:%.0f",bias_error);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*2, fezui_tempstr);

  sprintf(fezui_tempstr,"out:%f",Turn.pidout);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*3, fezui_tempstr);

  sprintf(fezui_tempstr,"p_l:%4.0f",motor_pid_l.pidout);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*4, fezui_tempstr);

  sprintf(fezui_tempstr,"p_r:%4.0f",motor_pid_r.pidout);
  u8g2_DrawStr(&u8g2, 64, ITEM_HEIGHT*4, fezui_tempstr);

  sprintf(fezui_tempstr,"l_e:%d",speed_l);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*5, fezui_tempstr);

  sprintf(fezui_tempstr,"r_e:%d",speed_r);
  u8g2_DrawStr(&u8g2, 64, ITEM_HEIGHT*5, fezui_tempstr);


  sprintf(fezui_tempstr,"yaw:%.1f",Angle_gz);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*6, fezui_tempstr);

  sprintf(fezui_tempstr,"yaw_adjust:%d",yaw_adjust);
  u8g2_DrawStr(&u8g2, 0, ITEM_HEIGHT*7, fezui_tempstr);

  sprintf(fezui_tempstr,"%ld",fezui_fps);
  u8g2_DrawStr(&u8g2, 90, ITEM_HEIGHT*7, fezui_tempstr);

  sprintf(fezui_tempstr,"%d",Communication_RX_Count);
  u8g2_DrawStr(&u8g2, 64, ITEM_HEIGHT*8, fezui_tempstr);
  /*
  for (uint8_t i = 0; i < configmenu.len; i++)
  {
    u8g2_DrawStr(&u8g2, 5 + (int16_t) menu_offset, ITEM_HEIGHT * (i + 1),
        configmenu.items[i]);
  }
  */
  fezui_draw_cursor(&cursor);

}

void config_menu_cb(lefl_menu_t *menu)
{

  switch (menu->selected_index)
  {
    case 0:
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    default:
      break;
  }
}

void configpage_load(lefl_page_t *page)
{
  lefl_animation_begin(&menuanimation);
  key_back.key_cb =
      lambda(void,
          (lefl_key_t*k){lefl_link_frame_go_back(&mainframe);lefl_cursor_set(&cursor ,0 ,0 ,WIDTH ,HEIGHT);});
  key_go.key_cb = lambda(void, (lefl_key_t*k){lefl_menu_click(&configmenu);});
  key_down.key_cb = lambda(void,
      (lefl_key_t*k){lefl_menu_index_increase(&configmenu, 1);});
  key_up.key_cb = lambda(void,
      (lefl_key_t*k){lefl_menu_index_increase(&configmenu, -1);});
}

