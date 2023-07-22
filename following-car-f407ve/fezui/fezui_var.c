/*
 * fezui_var.c
 *
 *  Created on: 2023年6月19日
 *      Author: xq123
 */
#include "fezui.h"

char fezui_tempstr[STRING_LENGTH] = {0};
char fpsstr[STRING_LENGTH] = {0};


uint8_t key_buffer[KEY_NUM] = {0};
uint8_t cmd_buffer = 0;

lefl_key_t key_up;
lefl_key_t key_down;
lefl_key_t key_go;
lefl_key_t key_back;

uint8_t fezui_rest_countdown = SCREEN_REST_TIME;
uint8_t fezui_debug;
uint32_t fezui_run_time;
uint32_t fezui_fps;
/*
enum USB_HID_KEYBOARD_SCAN_CODES *ansi_104_keycode[6][17] = {
        {KEY_ESC,KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,KEY_PRINT_SCREEN,KEY_SCROLL_LOCK,KEY_PAUSE},
        {KEY_GRAVE,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,KEY_MINUS,KEY_EQUAL,KEY_BACKSPACE,KEY_INSERT,KEY_HOME,KEY_PAGE_UP},
        {KEY_TAB,KEY_Q,KEY_W,KEY_E,KEY_R,KEY_T,KEY_Y,KEY_U,KEY_I,KEY_O,KEY_P,KEY_LEFT_BRACE,KEY_RIGHT_BRACE,KEY_BACKSLASH,KEY_DELETE,KEY_END,KEY_PAGE_DOWN},
        {KEY_CAPS_LOCK,KEY_A,KEY_S,KEY_D,KEY_F,KEY_G,KEY_H,KEY_J,KEY_K,KEY_L,KEY_SEMICOLON,KEY_APOSTROPHE,KEY_ENTER},
        {KEY_LEFT_SHIFT,KEY_Z,KEY_X,KEY_C,KEY_V,KEY_B,KEY_N,KEY_M,KEY_COMMA,KEY_DOT,KEY_SLASH,KEY_RIGHT_SHIFT,NULL,KEY_UP_ARROW,NULL},
        {KEY_LEFT_CONTROL,KEY_LEFT_GUI,KEY_LEFT_ALT,KEY_SPACEBAR,KEY_RIGHT_ALT,KEY_RIGHT_GUI,KEY_MENU,KEY_RIGHT_CONTROL,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW}
};
*/

const uint8_t led_seg_like_font[195] U8G2_FONT_SECTION("led_seg_like_font") =
        "\32\0\3\3\3\4\4\1\4\5\11\0\0\11\0\0\0\0\0\0\0\0\246 \5\0\354\0!\5\0\354"
        "\0\42\5\0\354\0#\5\0\354\0$\5\0\354\0%\5\0\354\0&\5\0\354\0'\5\0\354\0("
        "\5\0\354\0)\5\0\354\0*\5\0\354\0+\5\0\354\0,\5\0\354\0-\5\0\354\0.\5\11"
        "\354\10/\5\0\354\0\60\10M\354\260\351\233\1\61\6I\356\70\10\62\11M\354(&\36\23\13\63\11"
        "M\354(&\32\23\15\64\11M\354\210i\63\346\0\65\11M\354\60&\26\23\15\66\11M\354\60&\332"
        "\324\14\67\7M\354(\346\17\70\11M\354\260\251\335\324\14\71\11M\354\260\251\31\23\15\0\0\0\4\377"
        "\377\0";


lefl_link_frame_t mainframe = {
        .current_page=&configpage
    };

lefl_cursor_t cursor={0,0,128,64,fezui_draw_cursor};
lefl_cursor_t target_cursor={0,0,0,0,fezui_draw_cursor};

lefl_bit_array_t lines[4]=
{
    {.len=64},
    {.len=64},
    {.len=64},
    {.len=64},
};

lefl_animation_base_t animations[4]=
{
        {
                .easing_func=lefl_animation_cubic_ease,
                .end=170,
                .mode=LEFL_ANIMATION_EASE_OUT,
                .target=&cursor.x,
        },
        {
                .easing_func=lefl_animation_cubic_ease,
                .end=170,
                .mode=LEFL_ANIMATION_EASE_OUT,
                .target=&cursor.y,
        },
        {
                .easing_func=lefl_animation_cubic_ease,
                .end=170,
                .mode=LEFL_ANIMATION_EASE_OUT,
                .target=&cursor.w,
        },
        {
                .easing_func=lefl_animation_cubic_ease,
                .end=170,
                .mode=LEFL_ANIMATION_EASE_OUT,
                .target=&cursor.h,
        },
};




