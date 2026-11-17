#include "stm32f10x.h"                  // Device header
#include "SmartCar.h"
#include "Buzzer.h"
#include "Delay.h"

#define HIGH (1)
#define LOW (0)
uint8_t L1,L2,R1,R2;
uint8_t Top_L=1,Top_R=1;
uint8_t flag_left,flag_right;
void LineWalking_Init(void)//循迹和红外避障初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//循迹四个引脚，B10,B11,B12,B13
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;//红外避障两个引脚，C14,C15
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

void Get_LineWalking(uint8_t *L1,uint8_t *L2,uint8_t *R1,uint8_t *R2,uint8_t *Top_L,uint8_t *Top_R)
{
	*L1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10);
	*L2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	*R1=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
	*R2=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
	*Top_L=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);
	*Top_R=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);
	
	
}

void LineWalking(void)//循迹函数
{
			Get_LineWalking(&L1,&L2,&R1,&R2,&Top_L,&Top_R);
		
		if(Top_L==HIGH&&Top_R==HIGH)
		{
			if(L2==HIGH&&L1==LOW&&R1==LOW&&R2==HIGH)
			{
				Move_Forward(30);//直行
			}
			
			else if(L2==HIGH&&L1==LOW&&R1==HIGH&&R2==HIGH)
			{
				Turn_Left(30);//左小弯
				flag_left=1;
			}
			else if(L2==LOW&&L1==LOW&&R1==LOW&&R2==HIGH)
			{
				Turn_Left(40);//左直角
				Delay_ms(700);
			}
			
			else if(L2==LOW&&L1==HIGH&&R1==HIGH&&R2==HIGH)
			{
				Turn_Left(50);//左大弯
			}
			
			else if(L2==HIGH&&L1==HIGH&&R1==LOW&&R2==HIGH)
			{
				Turn_Right(30);//右小弯
				flag_right=1;
			}
			else if(L2==HIGH&&L1==LOW&&R1==LOW&&R2==LOW)
			{
				Turn_Right(40);//右直角
				Delay_ms(700);
			}
			else if(L2==HIGH&&L1==HIGH&&R1==HIGH&&R2==LOW)
			{
				Turn_Right(50);//右大弯
			}
				else if(L2==HIGH&&L1==HIGH&&R1==HIGH&&R2==HIGH)
				{
					if(flag_left==1)
					{
						flag_left=0;
						Turn_Left(40);// 左中弯
					}
					else if(flag_right==1)
					{
						flag_right=0;
						Turn_Right(40);//右中弯
					}
				}
		}
		else if(Top_L==LOW&&Top_R==LOW)
		{
			Car_Stop();//停止
			Buzzer();//蜂鸣器报警
		}
}
