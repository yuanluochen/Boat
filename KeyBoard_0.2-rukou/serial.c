#include "serial.h"

static HANDLE OpenSerial(serial_t* SerialInit, //串口名称，如COM1，COM2
    int baud,       //波特率：常用取值：CBR_9600、CBR_19200、CBR_38400、CBR_115200、CBR_230400、CBR_460800
    int byteSize,   //数位大小：可取值7、8；
    int parity,     //校验方式：可取值NOPARITY、ODDPARITY、EVENPARITY、MARKPARITY、SPACEPARITY
    int stopBits);   //停止位：ONESTOPBIT、ONE5STOPBITS、TWOSTOPBITS；


// static void MakeIdleData(uint8_t* IdleData);
static void MakeData(uint8_t* Data);

#define DATA_VAL(DATA ,KEYBOARD) (DATA | ((0X01) << KEYBOARD))

static serial_t Serial;//串口变量


//打开串口
static HANDLE OpenSerial(serial_t* SerialInit, //串口名称，如COM1，COM2
    int baud,       //波特率：常用取值：CBR_9600、CBR_19200、CBR_38400、CBR_115200、CBR_230400、CBR_460800
    int byteSize,   //数位大小：可取值7、8；
    int parity,     //校验方式：可取值NOPARITY、ODDPARITY、EVENPARITY、MARKPARITY、SPACEPARITY
    int stopBits)   //停止位：ONESTOPBIT、ONE5STOPBITS、TWOSTOPBITS；
{    
    
    DCB dcb;
    BOOL b = FALSE;
	COMMTIMEOUTS CommTimeouts;	
    HANDLE comHandle = INVALID_HANDLE_VALUE;
 
    //打开串口
	comHandle = CreateFile(Serial.SerialName,            //串口名称
		GENERIC_READ | GENERIC_WRITE,      //可读、可写   				 
		0,            // No Sharing                               
		NULL,         // No Security                              
		OPEN_EXISTING,// Open existing port only                     
		FILE_ATTRIBUTE_NORMAL,            // Non Overlapped I/O                           
		NULL);        // Null for Comm Devices
 
	if (INVALID_HANDLE_VALUE == comHandle) {
        LOGE("CreateFile fail\r\n");
        SerialInit->SerialStatus = 0;
        return comHandle;
        
    }
 
    // 设置读写缓存大小
	b = SetupComm(comHandle, BUF_SIZE, BUF_SIZE);
    if (!b) {
        LOGE("SetupComm fail\r\n");
        SerialInit->SerialStatus = 0;
    }
 
    //设定读写超时
	CommTimeouts.ReadIntervalTimeout = MAXDWORD;//读间隔超时
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;//读时间系数
	CommTimeouts.ReadTotalTimeoutConstant = 0;//读时间常量
	CommTimeouts.WriteTotalTimeoutMultiplier = 1;//写时间系数
	CommTimeouts.WriteTotalTimeoutConstant = 1;//写时间常量
	b = SetCommTimeouts(comHandle, &CommTimeouts); //设置超时
    if (!b) {
        LOGE("SetCommTimeouts fail\r\n");
        SerialInit->SerialStatus = 0;
    }
 
    //设置串口状态属性
    GetCommState(comHandle, &dcb);//获取当前
	dcb.BaudRate = baud; //波特率
	dcb.ByteSize = byteSize; //每个字节有位数
	dcb.Parity   = parity; //无奇偶校验位
	dcb.StopBits = stopBits; //一个停止位
	b = SetCommState(comHandle, &dcb);//设置
    if (!b) {
        LOGE("SetCommState fail\r\n");
        SerialInit->SerialStatus = 0;//串口创建失败
    }
 
    return comHandle;
}
 
void SerialInit()
{
    char SerialCOM[10] = { 0 };
    Serial.SerialStatus = 0;
    while(Serial.SerialStatus == 0)
    {
        //输入串口名称
        printf("please input COM name:>\n");
        scanf("%s", SerialCOM);
        Serial.SerialName = SerialCOM;
        Serial.SerialStatus = 1;
        //打开串口
        Serial.SerialHandle = OpenSerial(&Serial, CBR_9600, 8, NOPARITY, ONESTOPBIT);
    }
    printf("===========================\n");
    printf("==Serial Init Successful===\n");
    printf("===========================\n");
    printf("====Please Control Boat====\n");
    printf("===========================\n");
}
uint8_t last[3];
void SerialTxTask(void)
{

    uint8_t TxHead = TXHEAD;//头
    uint8_t TxData = 0;//数据段
    
    //读取键盘数据制作数据段
    MakeData(&TxData);

    //配置发送段
    Serial.SerialTx[0] = TxHead;
    Serial.SerialTx[1] = TxData;

    //输出串口发送数据
    if(memcmp(last, Serial.SerialTx, sizeof(uint8_t) * 3) != 0)
    {
        char s[10];
        itoa(Serial.SerialTx[1], s, 2);//二进制输出转换
        printf("0x%x %s 0X%x\n", Serial.SerialTx[0], s, Serial.SerialTx[3]);
        memcpy(last, Serial.SerialTx, sizeof(uint8_t) * 3);
    }
    //数据发送
    WINBOOL judge =  WriteFile(Serial.SerialHandle, 
            (LPCVOID)Serial.SerialTx, 
            2,
            NULL,
            NULL);
    //判断数据是否发送成功
    if(judge == 0)
    {
        //文件发送失败；程序结束
        perror("send message fail, we will close serial");
        exit(-1);
    }
    
    
}

//制作数据段
static void MakeData(uint8_t* Data)
{
    
    
    //如果按键的信息为运动控制
    if(GetKeyBoardVal()->KeyBoardInput != IdleOpen && GetKeyBoardVal()->KeyBoardInput != IdleClose)
    {
        //判断是否为停止
        if (GetKeyBoardVal()->KeyBoardInput == stop)
        {
            //是停止
            *Data = 0;//数据清零
        }
        else //不是停止
        {
            *Data = 0; // 数据重置
            // 配置数据
            *Data = DATA_VAL(*Data, GetKeyBoardVal()->KeyBoardInput);
        }

        // 保留警报段
        //判断历史警报段是否存在
        if (GetKeyBoardVal()->KeyBoardInputIdleLast == IdleClose || GetKeyBoardVal()->KeyBoardInputIdleLast == IdleOpen)
        {
            //历史数据存在，且符合需求
            *Data = DATA_VAL(*Data, GetKeyBoardVal()->KeyBoardInputIdleLast);
        }
    }
    //如果为警报控制
    else if(GetKeyBoardVal()->KeyBoardInput == IdleOpen || GetKeyBoardVal()->KeyBoardInput == IdleClose)
    {
        //数据重置
        *Data = 0;
        // 添加警报段
        *Data = DATA_VAL(*Data, GetKeyBoardVal()->KeyBoardInput);
        
        // 保留运动段
        //判断是否为运动数据
        if (GetKeyBoardVal()->KeyBoardInputRunLast == Front ||
            GetKeyBoardVal()->KeyBoardInputRunLast == Left ||
            GetKeyBoardVal()->KeyBoardInputRunLast == Back ||
            GetKeyBoardVal()->KeyBoardInputRunLast == Right ||
            GetKeyBoardVal()->KeyBoardInputRunLast == TurnLeft ||
            GetKeyBoardVal()->KeyBoardInputRunLast == TurnRight ||
            GetKeyBoardVal()->KeyBoardInputRunLast == stop)
        {
            //数据符合,添加运动数据
            *Data = DATA_VAL(*Data, GetKeyBoardVal()->KeyBoardInputRunLast);
        }
    }
}




