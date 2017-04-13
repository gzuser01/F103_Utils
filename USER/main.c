
/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"
#include "linked_list.h"
#include <stdlib.h>



uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; //正极接3.3V，负极接 C14 口
uint16_t UART1_LED_GPIO_Pin2 = GPIO_Pin_12;

unsigned char uart1_received_count = 0;


struct Node *UART1_Received_Linked = NULL;
struct Node *UART1_Received_Head = NULL;
struct Node *UART1_Received_Current = NULL;

int Received_Buffer_Idx = 0;

/*接收一个数据，马上返回接收到的这个数据*/
void UART1_Send_Received_Data(unsigned char c)
{
  //long i;
	//long j;
	Led_Auto_Off(GPIOC,UART1_LED_GPIO_Pin,8);
	Led_Auto_Off(GPIOB,UART1_LED_GPIO_Pin2,200);
	//UART1_Send_Byte(c);

 
}

void init_linked()
{
	int i;
	UART1_Received_Linked = (struct Node *)malloc(4 * sizeof(struct Node));
	for(i=0;i<8;i++)
	{
		UART1_Received_Linked[i].c = (unsigned char *)malloc(8* sizeof(unsigned char));
	}
	
}

void reset_c_buffer(unsigned char *c,int len)
{
	int i=0;
	for(i = 0;i<len;i++)
	{
		c[i] = 0x00;
	}
}


void UART1_Received_To_Buffer(unsigned char c)
{
	
	UART1_Send_Byte(c);

	if(UART1_Received_Current == NULL)
	{
		UART1_Received_Current = linked_pick_up_node(&UART1_Received_Head,UART1_Received_Linked,4);
		reset_c_buffer(UART1_Received_Current->c,8);
		Received_Buffer_Idx = 0;
	}

	//添加到Buffer
	if(Received_Buffer_Idx < 8)
	{
		UART1_Received_Current->c[Received_Buffer_Idx + 1] = c;
		Received_Buffer_Idx ++;
	}
	
	
	//满或者 c 为 0，则添加到链表
	if(Received_Buffer_Idx == 7 || c == 0x00)
	{

		UART1_Received_Current->state = 2;//标记为写完可读
		linked_add_last(&UART1_Received_Head,&UART1_Received_Current);
		
		//
		UART1_Send_Byte(linked_list_size(UART1_Received_Head));//debug
		
		//重新创建Buffer
		//如果没有可空的节点，那么 linked_pick_up_node 会删除head，
		UART1_Received_Current = linked_pick_up_node(&UART1_Received_Head,UART1_Received_Linked,4);
		reset_c_buffer(UART1_Received_Current->c,8);
		Received_Buffer_Idx = 0;
	}
	

	
	
	

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


void UART1_Send_Buffer_Data(void)
{
	int i = 0;
	unsigned char *data = NULL;
	while(1)
	{
		data = linked_first(UART1_Received_Head);
		if(data == NULL)
		{
			break;
		}
		for(i = 0;i< 16;i++)
		{
			UART1_Send_Byte(data[i]);
		}
		linked_remove_first(&UART1_Received_Head);
	}
	
	
	
}



int main(void)
{  
	     
	SystemInit();	//配置系统时钟为 72M 
   
	USART1_Config(); //USART1 配置 		
  TIM3_Configuration();  
	Flash_Led_Config(GPIOC,UART1_LED_GPIO_Pin); 
	Flash_Led_Config(GPIOB,UART1_LED_GPIO_Pin2); 
	
	Register_USART1_Callback(UART1_Send_Received_Data);
	//Register_USART1_Callback(UART1_Received_Count);
	Register_USART1_Callback(UART1_Received_To_Buffer);
	
	
	//Register_TIM3_Callback(UART1_Send_Buffer_Data);

	init_linked();

  while (1)
  {	 
    //UART1Test();
  }
}




