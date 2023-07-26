#include "bsp.h"
volatile BoatCtrl_t boat_ctrl;

void servo_ctrl(TIM_HandleTypeDef * htim,uint32_t Channel,uint16_t angle) {
	uint16_t compare_value=0;
	if(angle <= 270)
	{
		compare_value=0.5 * 20000 / 20 + angle  *20000 /20 / 135;
		__HAL_TIM_SET_COMPARE(htim, Channel, compare_value);
	}
}

void lora_decode(uint8_t* recv, 
				uint8_t* w, 
				uint8_t* a, 
				uint8_t* s, 
				uint8_t* d, 
				uint8_t* TurnLeft, 
				uint8_t* TurnRight, 
				uint8_t* sw)
{
	//二进制转�?	
	*w = 		(*recv >> KeyBoard_w) & 0x01;
	*a = 		(*recv >> KeyBoard_a) & 0x01;
	*s = 		(*recv >> KeyBoard_s) & 0x01;
	*d = 		(*recv >> KeyBoard_d) & 0x01;
	*TurnLeft = (*recv >> KeyBoard_TurnLeft) & 0x01;
	*TurnRight = (*recv >> KeyBoard_TurnRight) & 0x01;
	*sw = (*recv >> KeyBoard_sw) & 0x01;
}
