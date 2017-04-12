
/**************************************
 * �ļ���  ��main.c
 * ����    ��ͨ�����ڵ������������ӷ������ݣ����ӽ��յ����ݺ������ش������ԡ�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"


uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_14; //������3.3V�������� C14 ��


/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
void UART1Test(void)
{
	unsigned char c = 0;
	
	

	while(1)
	{ 
		
		while(UART1GetByte(&c))
		{
			Led_Auto_Off(8);
			USART_SendData(USART1,c);
		}    
		
	}     
}





int main(void)
{  
	     
	SystemInit();	//����ϵͳʱ��Ϊ 72M 
   
	USART1_Config(); //USART1 ���� 		
	LED_Config(UART1_LED_GPIO_Pin);
  TIM3_Configuration();  
	Flash_Led_Config(UART1_LED_GPIO_Pin); 

  while (1)
  {	 
    UART1Test();
  }
}




