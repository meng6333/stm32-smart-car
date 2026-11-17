#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "SmartCar.h"
#include "OLED.h"
#include "Servo.h"
#include "Buzzer.h"
#include "Serial.h"

int16_t CountNum;
uint16_t a,b,c,d,e,f,g;

void HCSR04_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;       // tiger引脚，B14
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;			// echo引脚，B15
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	
	TIM_InternalClockConfig(TIM1);//72MHz
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=60000-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
}
float HCSR04_Distance(void)    //超声波获取距离函数
{
	float Distance=0;
	float Last_Distance=0;
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	Delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==RESET);
	TIM1->CNT=0;
	TIM_Cmd(TIM1,ENABLE);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==SET);
	TIM_Cmd(TIM1,DISABLE);
	CountNum=TIM_GetCounter(TIM1);
	Distance=(CountNum*1.0*0.034)/2;
	TIM1->CNT=0;
	Delay_ms(100);
	if(Distance < 2.0 || Distance > 400.0)
	{
		Distance=Last_Distance;
		
	}
	else
	{
		Last_Distance=Distance;
	}

	return Distance;

}
void Following(void)//直线跟随
{
		 static uint8_t last_state = 0;  //  1:前进, 2:后退
      uint8_t current_state = 0;
	    OLED_ShowString(1,1,"Distance:       ");
			OLED_ShowString(2,4,"CM              ");
			OLED_ShowString(3,1,"Status:         ");
      OLED_ShowString(4,1,"gensui          ");
	while(1)
	{
		a=HCSR04_Distance();
		OLED_ShowNum(2,1,a,3);
		if(a < 19) 
		{
			current_state = 2;       // 后退
		}
     else if(a > 24) 
		 {
			 current_state = 1;  // 前进
		 }
		  // 只有状态改变时才更新电机
     if(current_state != last_state)
        {
            switch(current_state)
            {
                case 1: Move_Forward(30); break;
                case 2: Move_Backward(30); break;
            }
						
            last_state = current_state;
        }
			Delay_ms(100);  // 降低控制频率
		if (Serial_RxFlag==1) 
			{
          break;
       }
   }
	
}

void AvoidObstacle(void)//超声波避障
{
		a=HCSR04_Distance();
		Move_Forward(30);
		OLED_ShowNum(2,1,a,3);
		if(a<25&&a!=0)
		{
				Car_Stop();
				Buzzer();
				Servo_SetAngle(0);
				Delay_ms(1000);
				
				b=HCSR04_Distance();
				OLED_ShowNum(2,1,b,3);
				if(b>25)
				{
						Servo_SetAngle(90);
						Delay_ms(1000);
						Turn_Right(40);
						Delay_ms(1000);
						Move_Forward(30);	
				}
				else
				{
				
						Servo_SetAngle(180);
						Delay_ms(1000);
						
						c=HCSR04_Distance();
						OLED_ShowNum(2,1,c,3);
						if(c>25)
						{
							Servo_SetAngle(90);
							Delay_ms(1000);
							Turn_Left(40);
							Delay_ms(1000);
							Move_Forward(30);
				
						}
						else
						{
							Servo_SetAngle(90);
							Delay_ms(1000);
							CounterClockwise_Rotation(40);
							Delay_ms(1000);
							Car_Stop();
							d=HCSR04_Distance();
							OLED_ShowNum(2,1,d,3);	
							
								if(d>25)
								{
								Move_Forward(30);
								}
								else
								{
									Servo_SetAngle(0);
									Delay_ms(1000);
									e=HCSR04_Distance();
									OLED_ShowNum(2,1,e,3);
									if(e>25)
									{
									Servo_SetAngle(90);
									Delay_ms(1000);
									Turn_Right(40);
									Delay_ms(1000);
									Move_Forward(30);	
									}
									else
									{
									Servo_SetAngle(180);
									Delay_ms(1000);
									f=HCSR04_Distance();	
									OLED_ShowNum(2,1,f,3);	
						
										if(f>25)
										{
										Servo_SetAngle(90);
										Delay_ms(1000);
										Turn_Left(40);
										Delay_ms(1000);
										Move_Forward(30);	
										
										}
									else
									{
									Car_Stop();
									Servo_SetAngle(90);
									Delay_ms(1000);
									while(1)
										{
											Buzzer();
										}
									}
			
									}
								}
							}
					}
			}
}

