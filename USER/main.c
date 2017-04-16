
/**************************************
 * �ļ���  ��main.c
 * ����    ��ͨ�����ڵ������������ӷ������ݣ����ӽ��յ����ݺ������ش������ԡ�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"
#include "usart1_data.h"


#include <stdlib.h>



//uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; //������3.3V�������� C14 ��
uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_12;

unsigned long uart1_received_count = 0;



/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
void UART1_Flash_LED_On_Received(unsigned char c)
{

	Led_Auto_Off(GPIOB,UART1_LED_GPIO_Pin,1);
 
}




void UART1_Received_To_Buffer(unsigned char c)
{
	
	//UART1_Send_Byte(c);debug

	_UART1_Received_To_Buffer(c);
	

}


void UART1_Received_Count(unsigned char c)
{

	uart1_received_count ++;
	if(uart1_received_count == 0xFFFEUL)
	{
		uart1_received_count = 0;
	}
	 
}


void UART1_Send_Buffer_Data(void)
{
	
	int i = 0;
	
	/*
	int j = 0;

	int n;
	int c;
	
	char buf[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	*/
	
	unsigned char buff[usart1_data_node_char_len];
	
	//for(i = 0;i<10000;i++)
	//	for(j = 0;j<10000;j++);
	
	while(1)
	{
		i = _UART1_Read_To_Buffer(buff);
		if(i == 0)
		{
			return;
		}
		for(i = 0;i< usart1_data_node_char_len;i++)
		{
			UART1_Send_Byte(buff[i]);
		}
		
	}
	
	/*
	UART1_Send_Byte(0xFF);
	UART1_Send_Byte(0x00);
	sprintf(buf, "%06lx", (unsigned long)uart1_received_count & 0xFFFFUL);
	UART1_Send_Byte(buf[0]);
	UART1_Send_Byte(buf[1]);
	UART1_Send_Byte(buf[2]);
	UART1_Send_Byte(buf[3]);
	UART1_Send_Byte(buf[4]);
	UART1_Send_Byte(buf[5]);
	UART1_Send_Byte(0x00);
	UART1_Send_Byte(0xFF);
	*/
	
	
}



int main(void)
{  
	     
	SystemInit();	//����ϵͳʱ��Ϊ 72M 
   
	USART1_Config(); //USART1 ���� 		
	
	TIMx_Configuration(TIM2,NVIC_PriorityGroup_2,0,0);
  TIMx_Configuration(TIM3,NVIC_PriorityGroup_2,1,0);  
	TIMx_Configuration(TIM4,NVIC_PriorityGroup_2,2,0); 
	
	Flash_Led_Config(GPIOB,UART1_LED_GPIO_Pin); 
	
	
	Register_USART1_Callback(UART1_Flash_LED_On_Received);
	Register_USART1_Callback(UART1_Received_Count);
	Register_USART1_Callback(UART1_Received_To_Buffer);
	
	
	Register_TIMx_Callback(TIM3,UART1_Send_Buffer_Data);

	usart1_data_init();
  

  while (1)
  {	 
    
  }
	
}




