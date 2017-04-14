/***************************************
 * �ļ���  ��usart1.c
 * ����    ������USART1         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  

**********************************************************************************/

#include "usart1.h"
#include "misc.h" 


/**
 * �����жϻص�������ָ��
 */
void (*_m_usart1_irqhandler_ptr[])(unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART1_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�� USART1 �ж� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//ѡ����鷽ʽ0�����
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//���
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	  
	/* USART1 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܽ����жϣ��ڽ�����λ�Ĵ������������ǲ���
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//ʹ�ܷ����жϣ��ڷ��������ݺ������
	
	USART_Cmd(USART1, ENABLE);// USART1ʹ��
	
	

}

 /*����һ���ֽ�����*/
void UART1_Send_Byte(unsigned char SendData)
{	   
	USART_SendData(USART1,SendData);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char UART1_Get_Byte(unsigned char* GetData)
{   	   
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//û���յ����� 
	}
	
	*GetData = USART_ReceiveData(USART1); 
	
	return 1;//�յ�����
}

/*�����ж�*/
void USART1_IRQHandler(void)
{
	int i;
	unsigned char c = 0;
		
	while(UART1_Get_Byte(&c))
	{
		
		
		for(i = 0;i<10;i++)
		{
			if(_m_usart1_irqhandler_ptr[i] == 0)
			{
				break;
			}

			(* _m_usart1_irqhandler_ptr[i])(c);
		}
	}
}


/**
 * ע���жϻص�����
 */
void Register_USART1_Callback(void (* ptr)(unsigned char))
{
	int i;
	for(i = 0;i<10;i++)
	{
		//�Ѿ�ע��
		if(_m_usart1_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//����ƶ���δ���û�δռ��
		if(_m_usart1_irqhandler_ptr[i] == 0)
		{
			_m_usart1_irqhandler_ptr[i] = ptr;
			return;
		}
	}
}



