/***************************************
 * ------------------------
 * | PA9  - USART1(Tx)      |
 * | PA10 - USART1(Rx)      |
 * ------------------------
 * 
**********************************************************************************/


#include "usart1.h"
#include "gpio.h" 
#include "misc.h" 


/**
 * �����жϻص�������ָ��
 */
void (*_m_USART1_IRQHandler_Ptr[])(unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART1_Config(GPIO_TypeDef *Tx_GPIOx,uint16_t Tx_GPIO_Pin,GPIO_TypeDef *Rx_GPIOx,uint16_t Rx_GPIO_Pin)
{

	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* ʹ�� USART1 �ж� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ����鷽ʽ2
  
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
	
	
	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	/* USART1 ʹ��IO�˿����� */    
	GPIO_Config(Tx_GPIOx,Tx_GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//Tx �����������
	GPIO_Config(Rx_GPIOx,Rx_GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);//Rx ��������
	
	  
	/* USART1 ����ģʽ���� */
	USART_InitStruct.USART_BaudRate = 115200;	//���������ã�115200
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStruct.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStruct);  //��ʼ��USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܽ����жϣ��ڽ�����λ�Ĵ������������ǲ���
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//ʹ�ܷ����жϣ��ڷ��������ݺ������
	
	USART_Cmd(USART1, ENABLE);// USART1ʹ��
	
	

}

 /*����һ���ֽ�����*/
void USART1_Send_Byte(unsigned char Send_Data)
{	   
	USART_SendData(USART1,Send_Data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char USART1_Get_Byte(unsigned char* Get_Data)
{   	   
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//û���յ����� 
	}
	
	*Get_Data = USART_ReceiveData(USART1); 
	
	return 1;//�յ�����
}

/*�����ж�*/
void USART1_IRQHandler(void)
{
	int i;
	unsigned char c = 0;
		
	while(USART1_Get_Byte(&c))
	{
		
		
		for(i = 0;i<10;i++)
		{
			if(_m_USART1_IRQHandler_Ptr[i] == 0)
			{
				break;
			}

			(* _m_USART1_IRQHandler_Ptr[i])(c);
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
		if(_m_USART1_IRQHandler_Ptr[i] == ptr)
		{
			return;
		}
		//����ƶ���δ���û�δռ��
		if(_m_USART1_IRQHandler_Ptr[i] == 0)
		{
			_m_USART1_IRQHandler_Ptr[i] = ptr;
			return;
		}
	}
}



