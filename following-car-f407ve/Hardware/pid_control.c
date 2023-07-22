/*
 * pid_control.c
 *
 *  Created on: Aug 15, 2022
 *      Author: HP
 */
#include "pid_control.h"
#include "motor_control.h"
#include "us100.h"
#include <math.h>
#include "stage.h"
#define PWM_MAX_LIMIT 7000


PID motor_pid_l; //���pid����
PID motor_pid_r;
PID motor_pid;
PID Distance;
PID Turn;
PID Turn_Angle;
PID local;
float Limit_distance;
float distance;
float bias_error; //循迹偏差
float position_distance = 300; //目标距离差
int8_t speed_l; //应当期待的左轮速度
int8_t speed_r; //应当期待的右轮速度
int8_t Speed_differ = 0;
int8_t expect_speed = 20; //期待速度
float defult = 20; //这里所有的控制速度有关都用default 避免状态改变导致expect_speed改变
float stage; //这里指用来写状态


//void turn_left()
//{
//	yaw_adjust=90;
//	if((Angle_gz <= yaw_adjust+5)&&(Angle_gz>= yaw_adjust-5))//转到为
//	{
//		track_flag=0;
//		turn_flag=0;
//	}
//	else
//	{
//		track_flag=0;
//		turn_flag=1;
//		turn_state = TURN_LEFT;
//	}
//	follow_speed_adjust();
//}

void follow_adjust()
{
	if(track_flag && (turn_flag!=1))
	{
		Track(20);
	}
	else if((track_flag!=1)&& turn_flag)
	{
		if((turn_state == TURN_LEFT)&&(yaw <yaw_adjust))
		{
			motor_pid_l.pidout = -1000;
			motor_pid_r.pidout = 1000;
			give_pwm();
		}
		else if((turn_state == TURN_RIGHT)&&(yaw >yaw_adjust))
		{
			motor_pid_l.pidout = 1000;
			motor_pid_r.pidout = -1000;
			give_pwm();
		}
		else if((turn_state == TURN_BACK)&&(yaw >yaw_adjust))
		{
			motor_pid_l.pidout = 1000;
			motor_pid_r.pidout = -1000;
			give_pwm();
		}
		else
		{
			motor_pid_l.pidout = 0;
			motor_pid_r.pidout = 0;
			give_pwm();
		}
	}
	else
	{
		motor_pid_l.pidout = 0;
		motor_pid_r.pidout = 0;
		give_pwm();
	}
}
void follow_speed_adjust(void)
{
	if(track_flag && (turn_flag!=1))
		{
			Track(20);
		}
#if USE_GYRO == 1
	else if((track_flag==0)&& turn_flag)
	{
		if(turn_state == TURN_LEFT)
		{
			Motor_PID(0, 10);
		}
		else if(turn_state == TURN_RIGHT)
		{
			Motor_PID(10, 0);
		}
		else if(turn_state == TURN_BACK)
		{
			Motor_PID(10, -10);
		}
		else
		{
		    Motor_PID(0,0);
		}
	}
#else
	else if((track_flag==0)&& turn_flag)
	{
		if(turn_state == TURN_LEFT)
		{
			//Motor_PID(-40, 40);
		    Motor_PID(-20,20);
		}
		else if(turn_state == TURN_RIGHT)
		{
		    Motor_PID(20,-20);
		}
		else if(turn_state == TURN_BACK)
		{
		    Motor_PID(20,-20);
		}
		else
		{
			//motor_pid_l.pidout = 0;
			Motor_PID(0,0);
		}
	}
#endif
	else
	{
	    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1, 0);//180为死区电压对应的值
	    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2, 0);
//		motor_pid_l.pidout = 0;
//		motor_pid_r.pidout = 0;
//		give_pwm();
	}
}
void pid_inti(PID *pid)
{
  pid->instate = 0;
  pid->perr = 0;
  pid->errdat = 0;
  pid->pidout = 0;
  pid->last_pidout = 0;
  pid->last_derivative = 0;
}
//增量式pid
void PidIncCtrl(PID *pid)
{
  pid->last_pidout = pid->pidout;
  //pid->instate+=pid->errdat*pid->iGain;
  //pid->ilimit=pid->instate>500?0:pid->instate;
  pid->pidout += pid->pGain * (pid->errdat - pid->perr)
      + pid->iGain * pid->errdat
      + pid->dGain * (pid->errdat - 2 * pid->perr + pid->lastperr_errdat);
  pid->lastperr_errdat = pid->perr;
  pid->perr = pid->errdat;
}
//位置式pid, 积分分离
void PidLocCtrl(PID *pid, float limit)
{
  pid->instate += pid->errdat;
  if (pid->errdat >= limit)
    pid->iGain = 0;
  //constrain_float(pid->instate, -pid->ilimit, pid->ilimit);
  pid->pidout = pid->pGain / 100 * pid->errdat + pid->iGain * pid->instate
      + pid->dGain / 100 * (pid->errdat - pid->perr);
  pid->perr = pid->errdat;
}
void Motor_PID(int speed_l, int speed_r)
{
  motor_pid_l.errdat = speed_l - motor_l.Encoder;
  motor_pid_r.errdat = speed_r - motor_r.Encoder;
  PidIncCtrl(&motor_pid_l);
  PidIncCtrl(&motor_pid_r);
  if (motor_pid_l.pidout > PWM_MAX_LIMIT)
  {
    motor_pid_l.pidout = PWM_MAX_LIMIT;
  }
  else if (motor_pid_l.pidout < -PWM_MAX_LIMIT)
  {
    motor_pid_l.pidout = -PWM_MAX_LIMIT;
  }
  if (motor_pid_r.pidout > PWM_MAX_LIMIT)
  {
    motor_pid_r.pidout = PWM_MAX_LIMIT;
  }
  else if (motor_pid_r.pidout < -PWM_MAX_LIMIT)
  {
    motor_pid_r.pidout = -PWM_MAX_LIMIT;
  }
  Give_Motor_PWM(motor_pid_l.pidout, motor_pid_r.pidout);
}

void give_pwm ()
{
	if (motor_pid_l.pidout > PWM_MAX_LIMIT)
	  {
	    motor_pid_l.pidout = PWM_MAX_LIMIT;
	  }
	  else if (motor_pid_l.pidout < -PWM_MAX_LIMIT)
	  {
	    motor_pid_l.pidout = -PWM_MAX_LIMIT;
	  }
	  if (motor_pid_r.pidout > PWM_MAX_LIMIT)
	  {
	    motor_pid_r.pidout = PWM_MAX_LIMIT;
	  }
	  else if (motor_pid_r.pidout < -PWM_MAX_LIMIT)
	  {
	    motor_pid_r.pidout = -PWM_MAX_LIMIT;
	  }
	  Give_Motor_PWM(motor_pid_l.pidout, motor_pid_r.pidout);
}

void Turn_Control(void)
{
  Turn.errdat = bias_error;
  PidLocCtrl(&Turn, 0);
}

//单独小车循迹
void Track(uint8_t expect_speed)
{
  //defult = expect_speed;
  Turn.errdat = bias_error * 0.7 + Turn.lastperr_errdat * 0.3;
//		if(fabsf(Turn.errdat)>15)
//			Turn.pGain=25;
//		else 
//			Turn.pGain=20;
  //Turn.errdat=0;
  PidLocCtrl(&Turn, 0);
  Speed_differ = Turn.pidout;
  speed_l = defult + Speed_differ;
  speed_r = defult - Speed_differ;
  Motor_PID(speed_l, speed_r);
  Turn.lastperr_errdat = Turn.errdat;
}
//根据偏航角来进行90度旋转
void Turn_Angle_Control(float angle)
{
  Turn_Angle.errdat = angle - fabs(Angle_gz);
  PidLocCtrl(&Turn_Angle, 0);
}
//原地旋转90度  
// dir 0左转 1右转
void Turn_In_Place(float dir)
{
  Turn_Angle_Control(90);
  if (dir == 0)	//左转
  {
    speed_r = Turn_Angle.pidout;
    speed_l = -Turn_Angle.pidout;
  }
  else if (dir == 1)	//左转
  {
    speed_r = -Turn_Angle.pidout;
    speed_l = +Turn_Angle.pidout;
  }
  Get_Speed();
  Motor_PID(speed_l, speed_r);
  if (fabs(Angle_gz) == 90)
  {
    stage = 1;
  }

}
void local_speed_speed()
{
	Get_Encoder();
	a=motor_l.Encoder>0 ? motor_l.Encoder:-motor_l.Encoder;
	b=motor_r.Encoder>0 ? motor_r.Encoder:-motor_r.Encoder;
	pathlength+=(a+b)*0.5;
	local.errdat=expectlength-pathlength;
	PidLocCtrl(&local,0);
	motor_pid_l.errdat=local.pidout-motor_l.Encoder;
	motor_pid_r.errdat=-local.pidout-motor_r.Encoder;
	PidIncCtrl(&motor_pid_l);
	PidIncCtrl(&motor_pid_r);
}
//适用于双车跟随 固定位置差以及固定速度
//limit 这里是指双车之间距离偏差为多大时启用积分项
//串级pid要注意外环和内环的极性
//思考这里位置和速度到底是并级还是串级，这里位置环的输出到底是应该给后面小车的速度？还是说后面车应该变的差速？
void Speed_position_double_close()
{
  static int time;

  if (time == 2)
  {
    Distance.errdat = US100_Distance - position_distance;
    PidLocCtrl(&Distance, Limit_distance);
    time = 0;
  }
  defult += Distance.pidout;
  Track(defult);
  time++;
}
/**
 * @function: int Up_balance(float Angle,float Gyro,float Mechanical_Angle)
 * @description:直立环PD控制
 * @param {float} Angle 数据 :mpu6050测量的俯仰角
 * @param {float} Gyro 数据 :mpu6050测量的沿y轴加速度
 * @param {float} Mechanical_Angle 数据 :机械平衡角度（机械中值),这里设置为0
 * @return {int} Up_balance :直立环控制PWM
 */
int Up_balance(float Angle, float Gyro, float Mechanical_Angle)
{
  float Bias; //角度误差值
  int balance_up; //直立环控制PWM
  Bias = Angle - Mechanical_Angle; //角度误差值==测量的俯仰角-理想角度（机械平衡角度）
  balance_up = Up_balance_KP * Bias + Up_balance_KD * Gyro; //计算平衡控制的电机PWM  PD控制   Up_balance_KP是P系数,Up_balance_KD是D系数
  return balance_up;
}

/**
 * @function:int Velocity(int Encoder_left,int Encoder_right,int Mechanical_velocity)
 * @description:速度环PI控制
 * @param {int} Encoder_left 数据 :左电机编码器的值
 * @param {int} Encoder_right 数据 :右电机编码器的值
 * @param {int} Mechanical_velocity 数据 :目标速度 ，因为只是让小车尽快平衡并静止，这里目标速度也设为0
 * @return {int} Up_balance :速度环控制PWM
 */
/*int Velocity(int Encoder_A,int Encoder_B,int Mechanical_velocity)
 {
 int velocity,Encoder_Least; //速度环控制PWM,获取最新速度偏差
 static float Encoder,Encoder_Integral; //一阶低通滤波后的速度值，速度的积分；因为积分累加和滤波的需要，故设置为静态量，存储在全局区域类似于全局变量
 Encoder_Least =(Encoder_A+Encoder_B)-Mechanical_velocity; //获取最新速度偏差==测量速度（左右编码器之和）-目标速度
 Encoder = Encoder * 0.7+Encoder_Least*0.3 ; //一阶低通滤波器,减小速度环对于直立环的负面影响
 Encoder_Integral +=Encoder;               //积分，就是累加
 Encoder_Integral =Encoder_Integral>10000?10000:(Encoder_Integral<-10000?-10000:Encoder_Integral); //积分限幅
 velocity=Velocity_KP*Encoder+Velocity_KI*Encoder_Integral;        //速度控制 PI控制   Velocity_KP是P系数,Velocity_KI是I系数
 if(pitch<-30||pitch>30) 			Encoder_Integral=0;     						//电机关闭后清除积分
 return velocity;
 }*/
int Velocity(int Encoder_A, int encoder_B, int Mechanical_velocity)
{
  static float Velocity, Encoder_Least, Encoder;
  static float Encoder_Integral;
  //=============速度PI控制器=======================//
  Encoder_Least = (Encoder_Left + Encoder_Right) - Mechanical_velocity;
  //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）
  Encoder *= 0.7;          //===一阶低通滤波器
  Encoder += Encoder_Least * 0.3;   //===一阶低通滤波器
  Encoder_Integral += Encoder; //===积分出位移 积分时间：10ms
  Encoder_Integral =
      Encoder_Integral > 10000 ?
          10000 : (Encoder_Integral < -10000 ? -10000 : Encoder_Integral); //积分限幅
  Velocity = Encoder * Velocity_KP + Encoder_Integral * Velocity_KI;
  //===速度控制
  if (pitch < -30 || pitch > 30)
    Encoder_Integral = 0;
  //===电机关闭后清除积分
  return Velocity;
}
/*****************  
 转向环：系数*Z轴角速度
 ******************/
int Turn_out(int gyro_Z, int RC)
{
  int PWM_out;

  PWM_out = Turn_KD * gyro_Z + Turn_KP * RC;

  return PWM_out;
}

