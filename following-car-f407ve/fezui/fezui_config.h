/*
 * fezui_config.h
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */

#ifndef FEZUI_CONFIG_H_
#define FEZUI_CONFIG_H_

#define WIDTH                 128
#define HEIGHT                64
#define STRING_LENGTH         64
#define NUMBER_STRING_LENGTH  STRING_LENGTH
#define REFRESH_RATE          170
#define KPS_HISTORY_LENGTH    65
//#define TILE_LENGTH         56
#define SCREEN_REST_TIME      15
#define KEY_NUM               12
#define MAIN_KEY_NUM          4
#define ITEM_HEIGHT           8
#define HISTORY_LENGTH        65

#define _SCREEN_REST_ON
#define _FPS_ON

#define roll()                rand()%2;

extern const uint8_t led_seg_like_font[195] U8G2_FONT_SECTION("led_seg_like_font");

extern uint8_t key_buffer[KEY_NUM];
extern lefl_key_t key_up;
extern lefl_key_t key_down;
extern lefl_key_t key_go;
extern lefl_key_t key_back;

extern uint32_t fezui_keytotalcounts[MAIN_KEY_NUM];
extern uint32_t fezui_keyinitcounts[MAIN_KEY_NUM];
extern uint32_t fezui_fps;
extern uint8_t fezui_kps;
extern uint8_t KPS_history_max;
extern char fpsstr[STRING_LENGTH];
extern lefl_loop_array_t KPS_history;
extern uint8_t UI_KPSMaximumPerSecond;
extern uint8_t fezui_rest_countdown;
extern char fezui_tempstr[STRING_LENGTH];
extern uint8_t fezui_debug;
extern uint8_t cmd_buffer;

extern uint32_t fezui_run_time;


extern lefl_loop_array_t analog_historys[4];
extern uint8_t analog_history_buffer[4];

extern lefl_bit_array_t lines[4];
extern lefl_loop_array_t KPS_queue;

extern lefl_animation_base_t animations[4];
extern uint8_t wheelcount;


void fezui_draw_cursor(lefl_cursor_t*c);
void Analog_Read();
void fezui_read_counts();
void fezui_save_counts();

extern lefl_link_frame_t mainframe;

extern lefl_page_t configpage;
void configpage_logic(lefl_page_t *page);
void configpage_draw(lefl_page_t *page);
void configpage_load(lefl_page_t *page);
void config_menu_cb(lefl_menu_t *menu);

#endif /* FEZUI_CONFIG_H_ */
