

#include "key_pressed.h"
#include "usart1.h"
#include "timx.h"


#define KEY_ON 0
#define KEY_OFF 1

#define KEY_GPIOx GPIOA
#define KEY_GPIO_Pin GPIO_Pin_15
#define KEY_RCC_APB2Periph_GPIOx RCC_APB2Periph_GPIOA
#define KEY_PinSource GPIO_PinSource15
#define KEY_EXTI_Line EXTI_Line15
#define KEY_NVIC_IRQChannel EXTI15_10_IRQn //EXTI13��15������ EXTI15_10


void KEY_EXTI_Config(void)
{
 EXTI_InitTypeDef EXTI_InitStruct; //����һ��EXTI_InitTypeDef���͵Ľṹ��
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,KEY_PinSource); //ѡ��GPIOA�� PIN13��PIN15
 EXTI_InitStruct.EXTI_Line = KEY_EXTI_Line; //�ж���ѡ��
 EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //�ⲿ�ж�
 EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش����ж�
 EXTI_InitStruct.EXTI_LineCmd = ENABLE; //ʹ��
 EXTI_Init(&EXTI_InitStruct); //��ʼ��
}

void KEY_NVIC_Config(void) 
{
  NVIC_InitTypeDef NVIC_InitStruct;  //����ṹ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//ʹ�÷���
  NVIC_InitStruct.NVIC_IRQChannel = KEY_NVIC_IRQChannel; 
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; //�������ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //��Ӧ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //ʹ��
  NVIC_Init(&NVIC_InitStruct); //��ʼ��
}


void KEY_GPIO_Config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY_RCC_APB2Periph_GPIOx ,ENABLE); //ʹ��GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //ʹ��AFIOʱ�� �漰��IO������ �����迪��
	
	GPIO_InitStruct.GPIO_Pin = KEY_GPIO_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; //��������
	GPIO_Init(KEY_GPIOx,&GPIO_InitStruct); //��ʼ��GPIOA


}

u8 KEY_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;  
	}
	
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(KEY_EXTI_Line) != RESET)
	{
		//����ȥ���ظ�
		Delay_Nms(1);
		
		if(KEY_Scan(KEY_GPIOx,KEY_GPIO_Pin) == KEY_ON)
		{
			USART1_Send_Byte(0xEF);
			USART1_Send_Byte(0xFF);
			USART1_Send_Byte(0x01);
			USART1_Send_Byte('\n');
		}
	 
		else if(KEY_Scan(KEY_GPIOx,KEY_GPIO_Pin) == KEY_OFF)
		{
			USART1_Send_Byte(0xEF);
			USART1_Send_Byte(0xFF);
			USART1_Send_Byte(0x00);
			USART1_Send_Byte('\n');
		}
		
		
		
		EXTI_ClearFlag(KEY_EXTI_Line);
		EXTI_ClearITPendingBit(KEY_EXTI_Line); //����жϱ�� 
	}

	
}

void KEY_Config(void)
{
	//USART1 ���� 
	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t USART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t USART1_Rx_GPIO_Pin = GPIO_Pin_10;
	
	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); 
	//--USART1 ���� 
	
	KEY_GPIO_Config(); //����GPIO
	KEY_EXTI_Config(); //�����ж�
	KEY_NVIC_Config(); //����NVIC
	
	USART1_Send_Byte(0xFF);
	USART1_Send_Byte(0xFF);
	USART1_Send_Byte('\n');
}




















