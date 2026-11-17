#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"
float Servo_Angle=90;
extern uint8_t Servo_Flag;
void Servo_Init(void)
{
	PWM_Init();	
}

// 0      500
// 180    2500

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare4(Angle/180*2000+500);
}

