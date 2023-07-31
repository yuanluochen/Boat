#include "KeyBoard.h"
#include "windows.h"

static void JudgeKeyBoardInput(int input);

static KeyBoardControl_t KeyBoardControl;

// 键盘任务
void KeyBoardTask(void)
{
    // int input;
    // //信号采集，判断按下次数
    // if(_kbhit())
    // {
    //     //键盘按下
    //     //获取键盘输入，
    //     input = _getch();
    //     //延时等待消除抖动，抖动为在按下时会出现，认为他未按下的情况，延时会消除抖动
    //     //Sleep(30);

    // }
    // else
    // {
    //     input = 0;
    // }

    static int input = 0;
    static int lastIn = 0;

    if (input != 0 && lastIn == 0)
    {
        if (input != 'X' && input != 'x' && input != 'C' && input != 'c')
            Sleep(800);
    }
    lastIn = input;

    if (_kbhit())
    {
        input = _getch();
    }
    else
    {
        input = 0;
    }

    JudgeKeyBoardInput(input);

    // 判断键盘输入是否符合输入要求
}

static void JudgeKeyBoardInput(int input)
{
    // 将键盘按下的数值转化为运动状态，以备，制作串口发送数据段
    switch (input)
    {
        // 前进
    case 'W':
    case 'w':
        KeyBoardControl.KeyBoardInput = Front;
        printf("Front\n");
        break;
        // 左平移
    case 'A':
    case 'a':
        KeyBoardControl.KeyBoardInput = Left;
        printf("Left\n");
        break;
        // 后退
    case 'S':
    case 's':
        KeyBoardControl.KeyBoardInput = Back;
        printf("Back\n");
        break;
        // 右平移
    case 'D':
    case 'd':
        KeyBoardControl.KeyBoardInput = Right;
        printf("Right\n");
        break;
        // 左转
    case 'Q':
    case 'q':
        KeyBoardControl.KeyBoardInput = TurnLeft;
        printf("Turn Left\n");
        break;
        // 右转
    case 'E':
    case 'e':
        KeyBoardControl.KeyBoardInput = TurnRight;
        printf("Turn Right\n");
        break;
        // 警报
    case 'X':
    case 'x':
        KeyBoardControl.KeyBoardInput = IdleOpen;
        printf("Alarm open\n");
        break;
    case 'C':
    case 'c':
        KeyBoardControl.KeyBoardInput = IdleClose;
        printf("Alarm close\n");
        break;
        // 其他
    default:
        KeyBoardControl.KeyBoardInput = stop;
        break;
    }

    // 由于警铃与船的运动要处于同以状态，所以未防止数据覆盖，则要保存历史数据
    if (KeyBoardControl.KeyBoardInput != IdleOpen && KeyBoardControl.KeyBoardInput != IdleClose)
    {
        // 当前按键不是警铃控制按键，而是船体运动按键，保留船体运动状态，防止下次为警铃控制，覆盖掉船体运动
        KeyBoardControl.KeyBoardInputRunLast = KeyBoardControl.KeyBoardInput;
    }
    else if (KeyBoardControl.KeyBoardInput == IdleOpen || KeyBoardControl.KeyBoardInput == IdleClose)
    {
        // 如果当前按键是警铃控制按键，保留船体警铃控制，防止下次为运动控制，覆盖掉警铃控制
        KeyBoardControl.KeyBoardInputIdleLast = KeyBoardControl.KeyBoardInput;
    }
}

KeyBoardControl_t *GetKeyBoardVal(void)
{
    return &KeyBoardControl;
}

// 读文件 -> 文件数据 -> 大脑  // 串口
// 写文件 -> 大脑 -> 文件
