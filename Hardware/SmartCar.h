#ifndef __SMARTCAR_H
#define __SMARTCAR_H
void SmartCar_Init(void);
void Move_Forward(int8_t Speed);
void Move_Backward(int8_t Speed);
void Car_Stop(void);
void Turn_Left(int8_t Speed);
void Turn_Right(int8_t Speed);
void Clockwise_Rotation(int8_t Speed);
void CounterClockwise_Rotation(int8_t Speed);
void Speed_Up(void);
void Slow_Down(void);
extern int8_t Motor_Speed;
extern uint8_t speedControlEnabled; 
#endif
