
/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"



uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; //正极接3.3V，负极接 C14 口
uint16_t UART1_LED_GPIO_Pin2 = GPIO_Pin_12;

unsigned char uart1_received_count = 0;

/*接收一个数据，马上返回接收到的这个数据*/
void UART1_Send_Received_Data(unsigned char c)
{
  long i;
	long j;
	Led_Auto_Off(GPIOC,UART1_LED_GPIO_Pin,8);
	Led_Auto_Off(GPIOB,UART1_LED_GPIO_Pin2,1000);
	UART1_Send_Byte(c);
	
	for(i = 0;i<10000;i++)
		for(j = 0;j<10000;j++);
 
}


void UART1_Received_Count(unsigned char c)
{

	uart1_received_count ++;
	
	if(uart1_received_count == 0xFF)
	{
		uart1_received_count = 0;
	}
	
	UART1_Send_Byte(' ');
	
	UART1_Send_Byte(uart1_received_count);
	
	UART1_Send_Byte(' ');
 
}




int main(void)
{  
	     
	SystemInit();	//配置系统时钟为 72M 
   
	USART1_Config(); //USART1 配置 		
  TIM3_Configuration();  
	Flash_Led_Config(GPIOC,UART1_LED_GPIO_Pin); 
	Flash_Led_Config(GPIOB,UART1_LED_GPIO_Pin2); 
	
	Register_USART1_Callback(UART1_Send_Received_Data);
	Register_USART1_Callback(UART1_Received_Count);
	

  while (1)
  {	 
    //UART1Test();
  }
}




