#include "stm32f10x.h"                  // Device header
#include <stdio.h>
uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	// 1.开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2.GPIO初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;// A9,TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;// A10, RX 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3.USART初始化
	
	USART_InitTypeDef USART_InitStructure ;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None ;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No ;
	USART_InitStructure.USART_StopBits=USART_StopBits_1 ;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //开启串口数据接收中断
	
	// NVIC配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	//4.UART使能
	
	USART_Cmd(USART1,ENABLE);
	
}

//串口发送一个字节
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}
//串口发送一个数组
void Serial_SendArry(uint8_t *Arry,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Arry[i]);
	}
}
//串口发送字符串
void Serial_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial_SendByte(String[i]);
	}
}

//次方函数
uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t Result=1;
	while(y--)
	{
		Result*=x;
	}
	return Result;
}

//串口发送数字
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+0x30);
	}
}

int fputc(int ch,FILE*f)
{
	Serial_SendByte(ch);
	return ch;
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}

//获取串口接收的数据
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData; // 返回接收的数据变量
}

// UART1的中断函数
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//判断是否是 UART1的接收事件触发的中断
	{
		Serial_RxData=USART_ReceiveData(USART1);// 读取数据寄存器
		Serial_RxFlag=1;                        //置接收标志位为1 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除 UART1的RXNE标志位
	}
}
