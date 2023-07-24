#include <stdio.h>
#include "KeyBoard.h"
#include "serial.h"
#include <windows.h>


void Task()
{
    //按键数据读取
    KeyBoardTask();
    //将按键读取的数据转化为串口数据并发送
    SerialTxTask();
}

int main()
{
    //串口初始化
    SerialInit();
    
    while(1)
    {
        
        //任务
        Task();
    }
    return 0;
}

