#pragma once
#include <stdio.h>
#include <stdint.h>
#include <conio.h>

//按键标签数值
typedef enum
{
    Front = 0,//前进
    Left = 1,//左平移
    Back = 2,//后退
    Right = 3,//右平移
    TurnLeft = 4,//左转
    TurnRight = 5,//右转
    IdleOpen = 6,//警报开
    IdleClose = 7,//警报关
    stop = 8,
}KeyBoard_t;

//按键总控制结构体
typedef struct 
{
    KeyBoard_t KeyBoardInputRunLast;//船体上次运行状态
    KeyBoard_t KeyBoardInputIdleLast;//船体上次警铃状态
    //键盘输入
    KeyBoard_t KeyBoardInput;
}KeyBoardControl_t;

void KeyBoardTask(void);
//获取按键数组
KeyBoardControl_t* GetKeyBoardVal(void);