#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "SmartCar.h"
#include "Serial.h"
#include "HCSR04.h"
#include "Servo.h"
#include "LineWalking.h"
#include "Buzzer.h"

uint8_t RxData,Num;

int main(void)
{
	HCSR04_Init();
	OLED_Init();	
	SmartCar_Init();
	Serial_Init();
	Servo_Init();
	LineWalking_Init();
	Buzzer_Init();
	Servo_SetAngle(90);
	Delay_ms(500);
	Motor_Speed = 0;
  speedControlEnabled = 1;
	OLED_ShowString(1,1,"Smart Car Ready");
	while(1)
	{
		
		if(Serial_RxFlag==1)
		{
			RxData=Serial_GetRxData();
		
		if(RxData==0x40)//Ç°½ø
		{
			Num=1;	
		}
		if(RxData==0x41)//ºóÍË
		{
			Num=2;		
		}
		if(RxData==0x42)//Í£Ö¹
		{
			Num=3;	
		}
		if(RxData==0x43)//×ó×ª
		{
			Num=4;		
		}
		if(RxData==0x44)//ÓÒ×ª
		{
			Num=5; 		
		}
		if(RxData==0x45)//Ë³Ê±ÕëÐý×ª
		{
			Num=6;		
		}
		if(RxData==0x46)// ÄæÊ±ÕëÐý×ª
		{
			Num=7;
			
		}
		if(RxData==0x47)//Ñ­¼£
		{
			Num=8;
		}
		if(RxData==0x48)//³¬Éù²¨±ÜÕÏ
		{
			Num=9;
		}	
		if(RxData==0x49)//Ö±Ïß¸úËæ£¨³¬Éù²¨£©
		{
			Num=10;
		}
		
		if(RxData==0x70)//¼ÓËÙ
		{
			Num=11;
		}
		
		if(RxData==0x71)//¼õËÙ
		{
			Num=12;
		}
		Serial_RxFlag = 0;
		
		switch(Num)
		{
			case 1:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Move_Forward    ");
			Move_Forward(30);
		  break;
			
			case 2:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Move_Backward   ");
			Move_Backward(30);
		  break;
			
			case 3:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Stop            ");
			Car_Stop();
		  break;
			
			case 4:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Turn_Lef        ");
			Turn_Left(40);
		  break;
			
			case 5:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Turn_Right      ");	
			Turn_Right(40);
		  break;
			
			case 6:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Rotation_Right  ");
			Clockwise_Rotation(40);
		  break;
			
			case 7:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			OLED_ShowString(4,1,"Rotation_Left   ");
			CounterClockwise_Rotation(40);
		  break;
			
			case 8:
			OLED_ShowString(1,1,"               ");
			OLED_ShowString(2,1,"               ");
			OLED_ShowString(3,1,"Status:        ");
			OLED_ShowString(4,1,"LineWalking    ");	
				while(Num== 8)
				{
					LineWalking();
					if (Serial_RxFlag==1)
					{
						break;
					}
				}
		  break;
				
			case 9:
			OLED_ShowString(1,1,"Distance:      ");
			OLED_ShowString(2,4,"CM             ");
			OLED_ShowString(3,1,"Status:        ");
			OLED_ShowString(4,1,"bizhang        ");
				while(Num==9)
				{
					AvoidObstacle();
					if (Serial_RxFlag==1)
					 {
						  break; 
					 }
				}
		  break;
				
			case 10:Following();
		  break;
				
			case 11:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
		 if(speedControlEnabled)
       {
					OLED_ShowString(4,1,"Speed up        ");
       }
    else
			{
        OLED_ShowString(4,1,"Speed N/A       ");
			}
			Speed_Up();
			break;
			
			case 12:
			OLED_ShowString(1,1,"                ");
			OLED_ShowString(2,1,"                ");
			OLED_ShowString(3,1,"Status:         ");
			if(speedControlEnabled)
			{
        OLED_ShowString(4,1,"Slow down       ");
			}
    else
			{	
        OLED_ShowString(4,1,"Speed N/A       ");
			}
			Slow_Down();
			break;
			
			default:
			break;
		}
	}
}
		
}
	
	


