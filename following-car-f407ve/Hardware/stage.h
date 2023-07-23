/*
 * stage.h
 *
 *  Created on: Jul 15, 2023
 *      Author: fby
 */

#ifndef STAGE_H_
#define STAGE_H_
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "lefl.h"
extern uint8_t track_flag;
extern uint8_t turn_flag;
extern int16_t yaw_adjust;
extern uint16_t pathlength;
extern uint16_t expectlength;
extern int a;
extern int b;
void turn_left();
void turn_left_speed();
void model_select();
void turn_right_speed();
enum CAR_STATE
{
    CAR_START,
    CAR_TRACK,
    CAR_TURN,
    CAR_STOP
};

enum TURN_STATE
{
    TURN_STRAIGHT,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_BACK
};

enum STOP_STATE
{
    STOP_NUM,
    STOP_CROSSING,
    STOP_END
};
extern enum CAR_STATE car_state;
extern enum TURN_STATE turn_state;
extern enum STOP_STATE stop_state;

extern bool K210_stop_flag; //0  不是所需  1是所需 2是远端左拐没有所需
extern bool Num_yes;
extern bool MV_stop_flag;
extern bool MV_end_flag;
extern bool MV_main_flag;
extern bool deliver_return_flag; //假为送药真为让你返回
extern bool turn_finish_flag;
extern bool num_flag;   //k210识别到的数字是否所需，1代表停止识别到0代表没有继续前进

extern uint8_t number;   //k210返回第一次识别到的数字
extern uint8_t track_flag;
extern uint8_t turn_flag;
extern uint16_t turn_dir; //turn方向如果为left90
extern uint8_t back_dir[4];
extern uint8_t i; //转弯次数
extern uint8_t no_see_num_first; //第三个十字路口 0 没有找到 1 找到//平时为1

extern uint8_t go_right; //前往右边的远端

extern int angle;

extern int16_t yaw_adjust;
extern uint16_t pathlength;
extern uint16_t expectlength;

#endif /* STAGE_H_ */
