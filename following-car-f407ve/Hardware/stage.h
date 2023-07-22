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
extern	enum CAR_STATE car_state;
extern	enum TURN_STATE turn_state;
extern	enum STOP_STATE stop_state;

extern bool MV_stop_flag;
extern bool MV_end_flag;
extern bool MV_main_flag;

extern uint8_t number;
#endif /* STAGE_H_ */
