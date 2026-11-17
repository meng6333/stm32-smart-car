#include "stm32f10x.h"                  // Device header
#include "Motor.h"

int8_t Motor_Speed=0;
uint8_t speedControlEnabled = 0;  // 加减速度控制是否启用

void SmartCar_Init(void)
{
	Motor_Init();
	speedControlEnabled = 0;
}

void Move_Forward(int8_t Speed)//前进
{
	Motor_Speed=Speed;
	speedControlEnabled = 1;  // 前进时启用速度控制
	LeftMotor_Speed(Speed);
	RightMotor_Speed(Speed);
}
	
void Move_Backward(int8_t Speed)//后退
{
	Motor_Speed=-Speed;
	speedControlEnabled = 1;  // 后退时启用速度控制
	LeftMotor_Speed(-Speed);
	RightMotor_Speed(-Speed);
}

void Car_Stop(void)//停止
{
	Motor_Speed=0;
	speedControlEnabled = 1;  // 停止时启用速度控制
	LeftMotor_Speed(0);
	RightMotor_Speed(0);
}
void Turn_Left(int8_t Speed)//左转
{
	speedControlEnabled = 0; 
	LeftMotor_Speed(0);
	RightMotor_Speed(Speed);
}
void Turn_Right(int8_t Speed)//右转
{
	speedControlEnabled = 0; 
	LeftMotor_Speed(Speed);
	RightMotor_Speed(0);
}
void Clockwise_Rotation(int8_t Speed)//顺时针旋转
{
	speedControlEnabled = 0; 
	LeftMotor_Speed(Speed);
	RightMotor_Speed(-Speed);
}
void CounterClockwise_Rotation(int8_t Speed)//逆时针旋转
{
	speedControlEnabled = 0; 
	LeftMotor_Speed(-Speed);
	RightMotor_Speed(Speed);
}

void Speed_Up(void)//加速
{
	   if(speedControlEnabled == 0) // 速度控制未启用，直接返回
			 return;
		 
     if (Motor_Speed >= 0)
			 {
        Motor_Speed = Motor_Speed + 10; // 正方向加速
        if (Motor_Speed > 100)
					{
            Motor_Speed = 100;
					}
			 } 
			 else
			 {
        Motor_Speed = Motor_Speed - 10; // 负方向加速（更负）
        if (Motor_Speed < -100)
				{
            Motor_Speed = -100;
        }
    }
    LeftMotor_Speed(Motor_Speed);
    RightMotor_Speed(Motor_Speed);
 }

void Slow_Down(void)//减速
{
	if(speedControlEnabled == 0) return;  // 速度控制未启用，直接返回
	if (Motor_Speed > 0)
		{
        Motor_Speed = Motor_Speed - 10; // 正方向减速
        if (Motor_Speed < 0) 
					{
            Motor_Speed = 0;
					}
    } 
		else if (Motor_Speed < 0) 
			{
        Motor_Speed = Motor_Speed + 10; // 负方向减速（向0靠近）
        if (Motor_Speed > 0)
				{
            Motor_Speed = 0;
        }
    }
    // 如果Motor_Speed为0，保持不变
    LeftMotor_Speed(Motor_Speed);
    RightMotor_Speed(Motor_Speed);

}
