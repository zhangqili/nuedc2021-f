/*
 * stage.c
 *
 *  Created on: Jul 15, 2023
 *      Author: fby
 */
#include "stage.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "motor_control.h"
#include "pid_control.h"
enum CAR_STATE car_state;
enum TURN_STATE turn_state;
enum STOP_STATE stop_state;

bool K210_stop_flag; //0  不是所需  1是所需 2是远端左拐没有所需
bool Num_yes;
bool MV_stop_flag;
bool MV_end_flag;
bool MV_main_flag;
bool deliver_return_flag; //假为送药真为让你返回
bool turn_finish_flag;
bool num_flag;   //k210识别到的数字是否所需，1代表停止识别到0代表没有继续前进

uint8_t NUM = 0;   //k210返回第一次识别到的数字
uint8_t track_flag;
uint8_t turn_flag;
uint8_t turn_dir; //turn方向如果为left90
uint8_t back_dir[4] = { 0 };
uint8_t i = 0; //转弯次数
uint8_t no_see_num_first; //第三个十字路口 0 没有找到 1 找到//平时为1
//uint8_t no_see_num_last;//远端先去左边  1左边有所需数字  0没有所需数字掉头
uint8_t go_right = 0; //前往右边的远端
//uint8_t =0;//记录远端右边所经过的路口
int angle;
//int16_t yaw;
int16_t yaw_adjust;
int a = 0, b = 0;
uint16_t pathlength = 0;
uint16_t expectlength = 0;
void turn_left()
{
    yaw_adjust = 90;
    turn_state = TURN_LEFT;
    if ((Angle_gz <= yaw_adjust + 5) && (Angle_gz >= yaw_adjust - 5)) //转到为
    {
        track_flag = 0;
        turn_flag = 0;
    } else //没有转到位
    {
        //car_state= CAR_TURN;
        track_flag = 0;
        turn_flag = 1;
    }
    follow_adjust();
}
void turn_left_speed()
{
    yaw_adjust = 90;
    turn_state = TURN_LEFT;
    if ((Angle_gz <= yaw_adjust + 5) && (Angle_gz >= yaw_adjust - 5)) //转到为
    {
        track_flag = 0;
        turn_flag = 0;
    } else //没有转到位
    {
        //car_state= CAR_TURN;
        track_flag = 0;
        turn_flag = 1;
    }
    if (pathlength > 650)
    {
        track_flag = 0;
        turn_flag = 0;
    }
    //Get_Speed();
    Get_Speed();
    //Motor_PID(20, 20);
    a = motor_l.Encoder > 0 ? motor_l.Encoder : -motor_l.Encoder;
    b = motor_r.Encoder > 0 ? motor_r.Encoder : -motor_r.Encoder;
    pathlength += (a + b) * 0.5;
    follow_speed_adjust();
}
void turn_right_speed()
{
    yaw_adjust = -90;
    turn_state = TURN_RIGHT;
    /*if ((Angle_gz <= yaw_adjust + 5) && (Angle_gz >= yaw_adjust - 5)) //转到为
    {
        track_flag = 0;
        turn_flag = 0;
    } else //没有转到位*/
   /* if(pathlength<650)
    {
        //car_state= CAR_TURN;
        track_flag = 0;
        turn_flag = 1;
    }
    else
    {
        track_flag = 0;
        turn_flag = 0;
    }*/
    //Get_Speed();
    Get_Speed();
    //Motor_PID(20, 20);
    a = motor_l.Encoder > 0 ? motor_l.Encoder : -motor_l.Encoder;
    b = motor_r.Encoder > 0 ? motor_r.Encoder : -motor_r.Encoder;
    pathlength += (a + b) * 0.5;

    follow_speed_adjust();
}
void model_select()
{
    switch (car_state)
    {
        case CAR_START:
            if (deliver_return_flag) // 返回
            {
            } else if (!deliver_return_flag) // 送药
            {
                if (NUM != 0) // 读取到数字
                {
                    //加上光电对管判断
                    car_state = CAR_TRACK;
                    track_flag = 1;
                    turn_flag = 0;
                }
            }
            break;
        case CAR_TRACK:
            if (deliver_return_flag) // 返回
            {
                if (MV_stop_flag) //这里就只有openmv识别以后只有左右转不会停下来
                {
                    if (back_dir[i] == 1) //倒退回溯右变左
                    {
                        car_state = CAR_TURN;
                        turn_state = TURN_RIGHT;
                        i--;
                        track_flag = 0;
                        turn_flag = 1;
                    } else if (back_dir[i] == 2) //左变右
                    {
                        car_state = CAR_TURN;
                        turn_state = TURN_LEFT;
                        i--;
                        track_flag = 0;
                        turn_flag = 1;
                    } else
                    {
                        car_state = CAR_TRACK;
                        turn_state = TURN_STRAIGHT;
                        i--;
                        track_flag = 1;
                        turn_flag = 0;
                    }

                } else if (MV_end_flag)
                {
                    car_state = CAR_STOP;
                    stop_state = STOP_END;
                    track_flag = 1;
                    turn_flag = 0;
                } else
                {
                    car_state = CAR_TRACK;
                    turn_state = TURN_STRAIGHT;
                    track_flag = 1;
                    turn_flag = 0;
                }
            } else if (!deliver_return_flag) // 送药
            {
                if (K210_stop_flag) //只要不为0就会先原地停
                {
                    car_state = CAR_STOP;
                    stop_state = STOP_NUM;
                    track_flag = 0;
                    turn_flag = 0;
                } else if (MV_stop_flag) //需要判断是否会停下来是否到指定路口
                {
//	            	if(no_see_num_first)//远端看得到第三个十字存在
//	            	{
                    turn_state = turn_dir;
                    car_state = CAR_TURN;
                    stop_state = STOP_CROSSING;
                    track_flag = 0;
                    turn_flag = 0;
                    //yaw_adjust=Angle_gz;
                    angle = Angle_gz;
                    if (turn_dir == 1)
                        yaw_adjust = Angle_gz + 90; //左转度数增大
                    else if (turn_dir == 2)
                    {
                        yaw_adjust = Angle_gz - 90; //右转度数减小
                    } else if (turn_dir == 3)
                    {
                        yaw_adjust = Angle_gz - 180;
                    }
                    if (yaw_adjust > 360)
                        yaw_adjust %= 360;
                    else if (yaw_adjust < 0)
                        yaw_adjust = (yaw_adjust + 360) % 360;
                    //stop_state = STOP_CROSSING;
//	            	}
//	            	else//不存在先左转
//	            	{
//	            		car_state= CAR_TURN;
//	            		turn_state = TURN_LEFT;
//						track_flag=0;
//						turn_flag=0;
//	            	}

                } else if (NUM == 1)
                {
                    car_state = CAR_TURN;
                    turn_state = TURN_LEFT;
                    track_flag = 0;
                    turn_flag = 0;
#if USE_GYRO == 1
                    yaw_adjust = Angle_gz + 90;
                    if (yaw_adjust > 360)
                        yaw_adjust %= 360;
                    else if (yaw_adjust < 0)
                        yaw_adjust = (yaw_adjust + 360) % 360;
#endif
                } else if (NUM == 2)
                {
                    car_state = CAR_TURN;
                    turn_state = TURN_RIGHT;
                    track_flag = 0;
                    turn_flag = 0;
#if USE_GYRO == 1
                    yaw_adjust = Angle_gz - 90;
                    if (yaw_adjust > 360)
                        yaw_adjust %= 360;
                    else if (yaw_adjust < 0)
                        yaw_adjust = (yaw_adjust + 360) % 360;
#endif
                } else if (MV_end_flag)
                {
                    car_state = CAR_STOP;
                    stop_state = STOP_END;
                    turn_state = TURN_STRAIGHT;
                } else
                {
                    car_state = CAR_TRACK;
                    track_flag = 1;
                    turn_flag = 0;
                }
            }
            break;
        case CAR_TURN: // 转向函数
            switch (turn_state)
            {
                case TURN_STRAIGHT:
                    turn_finish_flag = 1;
                    break;
                case TURN_LEFT:
                    //yaw_ djust-=90;//假设左转度数减小
                    //yaw_adjust=(yaw_adjust+360)%360;
#if USE_GYRO ==1
                    if ((Angle_gz <= yaw_adjust + 5)
                            && (Angle_gz >= yaw_adjust - 5))			//转到为
#else
			if (pathlength > 650)

#endif
                    {
                        turn_finish_flag = 1;
                    } else	        	//没有转到位
                    {
                        car_state = CAR_TURN;
                        turn_state = TURN_LEFT;
                        track_flag = 0;
                        turn_flag = 1;
                    }
                    break;
                case TURN_RIGHT:
#if USE_GYRO ==1
                    if ((Angle_gz >= yaw_adjust - 5)
                            && (Angle_gz <= yaw_adjust + 5))
#else
			if (pathlength > 650)

#endif
                    {
                        turn_finish_flag = 1;
                    } else
                    {
                        car_state = CAR_TURN;
                        turn_state = TURN_RIGHT;
                        track_flag = 0;
                        turn_flag = 1;
                    }
                    //yaw_adjust+=90;//假设右转度数增加
//	        	if((Angle_gz>=yaw_adjust-5)&&(Angle_gz<=yaw_adjust+5))//转到位
//	        	{
//	        		turn_finish_flag=1;
//	        	}
//	        	else  //没有转到位
//	        	{
//	        		car_state= CAR_TURN;
//					track_flag=0;
//					turn_flag=1;
//	        	}
                    break;
                case TURN_BACK:
                    //yaw_adjust+=180;
#if USE_GYRO ==1
                    if ((Angle_gz >= yaw_adjust - 5)
                            && (Angle_gz <= yaw_adjust + 5))
#else
            if (pathlength > 1300)

#endif
	        //转到位
                    {
                        turn_finish_flag = 1;
                    } else  //没有转到位
                    {
                        car_state = CAR_TURN;
                        turn_state = TURN_BACK;
                        track_flag = 0;
                        turn_flag = 1;
                    }
//	        	if((Angle_gz>=yaw_adjust-5)&&(Angle_gz <= yaw_adjust+5))
//	        	{
//	        		turn_finish_flag=1;
//	        	}
//	        	else
//	        	{
//	        		car_state= CAR_TURN;
//					turn_state= TURN_BACK;
//					track_flag=0;
//					turn_flag=1;
//	        	}
                    break;
            }
            if (turn_finish_flag) // 转向完成，继续巡线
            {
                car_state = CAR_TRACK;
                turn_state = TURN_STRAIGHT;
                track_flag = 1;
                turn_flag = 0;

            }
            break;
        case CAR_STOP:
            switch (stop_state)
            {
                case STOP_NUM:
                    //car_state = CAR_TRACK;
                    if (num_flag)  // 收到转向数据即数字识别完成
                    {
                        turn_state = turn_dir; // 需要储存转向数据
                        car_state = CAR_TRACK;
                        stop_state = STOP_NUM;
                        back_dir[i] = turn_dir; // 需要储存转向数据
                        i++;
                        track_flag = 1;
                        turn_flag = 0;
                    }
//	            else if(K210_stop_flag == 2)// 如果识别不到默认左转直接发左转就好了
//	            {
//	            	turn_state =turn_dir;
//	            	//这里可以手动赋值转180也可以去采用k210发送180
//	            	//turn_state=TURN_BACK;
//	            	car_state = CAR_TURN;
//	            	track_flag=0;
//					turn_flag=0;
//					go_right=1;
//	            }
                    else
                    {
                        turn_state = TURN_STRAIGHT; // 需要储存转向数据
                        car_state = CAR_STOP;
                        stop_state = STOP_NUM;
                        track_flag = 0;
                        turn_flag = 0;
                    }
                    break;
                case STOP_CROSSING:  //只有走到所需数字前的十字才会进入停止状态这里只是一个中转
                    car_state = CAR_TURN;
                    track_flag = 0;
                    turn_flag = 0;
                    break;
                case STOP_END:
                    if (deliver_return_flag)  // 判断返回先转180
                    {
                        //HAL_Delay(500); // 返回延时
                        //deliver_return_flag = true;
                        car_state = CAR_TURN;
                        turn_state = TURN_BACK;
                        track_flag = 0;
                        turn_flag = 0;
                    } else
                    {
                        car_state = CAR_STOP;
                        stop_state = STOP_END;
                        track_flag = 0;
                        turn_flag = 0;
                    }
                    break;
            }
            break;
        default:
            break;
    }
}

