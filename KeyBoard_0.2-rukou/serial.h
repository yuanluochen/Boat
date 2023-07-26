#pragma once
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdint.h>
#include "KeyBoard.h"
 
#if 1   //开启DEBUG打印
#define LOGD(...) printf(__VA_ARGS__)
#else   //关闭DEBUG打印
#define LOGD(...)
#endif
 
#if 1   //开启ERROR打印
#define LOGE(...) printf(__VA_ARGS__)
#else   //关闭ERROR打印
#define LOGE(...)
#endif
 
//缓冲区大小
#define BUF_SIZE    2048
#define EXIT_STR    "exit"
#define I_EXIT      "I exit.\r\n"
#define I_RECEIVE   "I receive.\r\n"

#define TX_SIZE 2
#define TXHEAD 0x34//发送头

//串口收发 
typedef struct 
{
    //串口端口号
    char* SerialName;
    
    //串口服务
    HANDLE SerialHandle;
    
    //串口状态
    uint8_t SerialStatus;
    
    //串口发送数组
    uint8_t SerialTx[2];
}serial_t;



//串口初始化
void SerialInit(void);

//串口发送任务
void SerialTxTask(void);
