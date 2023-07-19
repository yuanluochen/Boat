#ifndef __BSP_H__
#define __BSP_H__

#include "main.h"
#include "tim.h"

#define ESC_MIN 1000
#define ESC_MAX 2000
#define ESC_MID 1500

#define LORA_RX_BUFF_LEN 64

#define ESC1_CHANNEL TIM_CHANNEL_1
#define ESC2_CHANNEL TIM_CHANNEL_2
#define SERVO1_CHANNEL TIM_CHANNEL_3
#define SERVO2_CHANNEL TIM_CHANNEL_4

//舵机定时器
#define SERVO_TIM htim1
//前舵机通道
#define SERVO_FRONT_CHANNEL SERVO1_CHANNEL
//后舵机通道
#define SERVO_BACK_CHANNEL SERVO2_CHANNEL

/**
 * @brief 前舵机130 后舵机148 
 * 
 */
//前舵机角度中值
#define SERVO_ANGLE_FRONT_MID 130
//后舵机角度中值
#define SERVO_ANGLE_BACK_MID 148
//前舵机正向
#define SERVO_FRONT_ANGLE_FRONT SERVO_ANGLE_FRONT_MID
//前舵机左
#define SERVO_FRONT_ANGLE_LEFT  SERVO_ANGLE_FRONT_MID - 90
//前舵机右
#define SERVO_FRONT_ANGLE_RIGHT SERVO_ANGLE_FRONT_MID + 90
//后舵机正向
#define SERVO_BACK_ANGLE_FRONT SERVO_ANGLE_BACK_MID
//后舵机左
#define SERVO_BACK_ANGLE_LEFT  SERVO_ANGLE_BACK_MID - 90
//后舵机右
#define SERVO_BACK_ANGLE_RIGHT SERVO_ANGLE_BACK_MID + 90


//电机定时器
#define MOTOR_TIM htim1
//前电机通道
#define MOTOR_FRONT_CHANNEL ESC1_CHANNEL
//后舵机通道
#define MOTOR_BACK_CHANNEL ESC2_CHANNEL
#if 1
//电机正转
#define MOTOR_FRONT (ESC_MIN + 300)
//电机反转
#define MOTOR_BACK (ESC_MAX - 300)
#else
//电机正转
#define MOTOR_FRONT ESC_MID
//电机反转
#define MOTOR_BACK ESC_MID

#endif

//电机停止
#define MOTOR_STOP ESC_MID 



typedef enum _Dir_t {
	motion_Stop = 0,//停止
	motion_front = 1,
	motion_back = 2,
	motion_left = 3,
	motion_right = 4,
	motion_TurnLeft = 5,
	motion_TurnRight = 6,
} Motion_t;

//警报判断
typedef enum
{
	IdleClose = 0,
	IdleOpen = 1,
}idle_t;

//警报
typedef struct
{
	idle_t idle;
}idleControl_t;


//键盘数据解码宏定义
typedef enum 
{
	KeyBoard_w = 0,
	KeyBoard_a = 1,
	KeyBoard_s = 2,
	KeyBoard_d = 3,
	KeyBoard_TurnLeft = 4,
	KeyBoard_TurnRight = 5,
	KeyBoard_sw = 6,

}KeyBoard_t;

typedef struct _BoatCtrl_t {
	Motion_t motion;
	idleControl_t idleControl;
} BoatCtrl_t;


extern volatile BoatCtrl_t boat_ctrl;


void servo_ctrl(TIM_HandleTypeDef * htim,uint32_t Channel,uint16_t angle);
/**
 * @brief 数据解码
 * 
 * @param recv 发送数据位
 * @param w 前进
 * @param a 左平移
 * @param s 后退
 * @param d 右平移
 * @param TurnLeft 原地左转
 * @param TurnRight 原地右转
 * @param sw 警报
 */
void lora_decode(uint8_t* recv, 
				uint8_t* w, 
				uint8_t* a, 
				uint8_t* s, 
				uint8_t* d, 
				uint8_t* TurnLeft, 
				uint8_t* TurnRight, 
				uint8_t* sw);



#endif