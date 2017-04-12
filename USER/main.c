
/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"


uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_14; //正极接3.3V，负极接 C14 口


/*接收一个数据，马上返回接收到的这个数据*/
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
	     
	SystemInit();	//配置系统时钟为 72M 
   
	USART1_Config(); //USART1 配置 		
	LED_Config(UART1_LED_GPIO_Pin);
  TIM3_Configuration();  
	Flash_Led_Config(UART1_LED_GPIO_Pin); 

  while (1)
  {	 
    UART1Test();
  }
}




