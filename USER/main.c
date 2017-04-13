
/**************************************
 * �ļ���  ��main.c
 * ����    ��ͨ�����ڵ������������ӷ������ݣ����ӽ��յ����ݺ������ش������ԡ�         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * ��汾  ��ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "flash_led.h"
#include "linked_list.h"
#include <stdlib.h>



uint16_t UART1_LED_GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14; //������3.3V�������� C14 ��
uint16_t UART1_LED_GPIO_Pin2 = GPIO_Pin_12;

unsigned char uart1_received_count = 0;


struct Node *UART1_Received_Linked = NULL;
struct Node *UART1_Received_Head = NULL;
struct Node *UART1_Received_Current = NULL;

int Received_Buffer_Idx = 0;

/*����һ�����ݣ����Ϸ��ؽ��յ����������*/
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

	//��ӵ�Buffer
	if(Received_Buffer_Idx < 8)
	{
		UART1_Received_Current->c[Received_Buffer_Idx + 1] = c;
		Received_Buffer_Idx ++;
	}
	
	
	//������ c Ϊ 0������ӵ�����
	if(Received_Buffer_Idx == 7 || c == 0x00)
	{

		UART1_Received_Current->state = 2;//���Ϊд��ɶ�
		linked_add_last(&UART1_Received_Head,&UART1_Received_Current);
		
		//
		UART1_Send_Byte(linked_list_size(UART1_Received_Head));//debug
		
		//���´���Buffer
		//���û�пɿյĽڵ㣬��ô linked_pick_up_node ��ɾ��head��
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
	     
	SystemInit();	//����ϵͳʱ��Ϊ 72M 
   
	USART1_Config(); //USART1 ���� 		
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




