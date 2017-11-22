/***************************************
 * ------------------------
 * | PA9  - USART1(Tx)      |
 * | PA10 - USART1(Rx)      |
 * ------------------------
 * 
**********************************************************************************/


#include "usart.h"
#include "gpio.h" 
#include "misc.h" 


/**
 * �����жϻص�������ָ��
 */
void (*_m_USART1_IRQHandler_Ptr[])(USART_TypeDef* USARTx,unsigned char) = {0,0,0,0,0,0,0,0,0,0};

void (*_m_USART2_IRQHandler_Ptr[])(USART_TypeDef* USARTx,unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART_Config(USART_TypeDef* USARTx,uint32_t USART_BaudRate,
		GPIO_TypeDef *Tx_GPIOx,uint16_t Tx_GPIO_Pin,GPIO_TypeDef *Rx_GPIOx,uint16_t Rx_GPIO_Pin)
{

	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* ʹ�� USARTx �ж� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ����鷽ʽ2
  
	if(USARTx == USART1)
	{
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	}
	else if(USARTx == USART2)
	{
		NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	}
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
	
	
	/* ʹ�� USART1 ʱ��*/
	if(USARTx == USART1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	}
	else if(USARTx == USART2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
	}
	
	
	/* USART1 ʹ��IO�˿����� */    
	GPIO_Config(Tx_GPIOx,Tx_GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//Tx �����������
	GPIO_Config(Rx_GPIOx,Rx_GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);//Rx ��������
	
	  
	/* USART1 ����ģʽ���� */
	USART_InitStruct.USART_BaudRate = USART_BaudRate;	//����������
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStruct.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStruct);  //��ʼ��USART1
	
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//ʹ�ܽ����жϣ��ڽ�����λ�Ĵ������������ǲ���
	//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//ʹ�ܷ����жϣ��ڷ��������ݺ������
	
	USART_Cmd(USARTx, ENABLE);// USART1ʹ��
	
	

}

 /*����һ���ֽ�����*/
void USART_Send_Byte(USART_TypeDef* USARTx,unsigned char Send_Data)
{	   
	USART_SendData(USARTx,Send_Data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char USART_Get_Byte(USART_TypeDef* USARTx,unsigned char* Get_Data)
{   	   
	if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//û���յ����� 
	}
	
	*Get_Data = USART_ReceiveData(USARTx); 
	
	return 1;//�յ�����
}

/*����1�ж�*/
void USART1_IRQHandler(void)
{
	int i;
	unsigned char c = 0;
		
	while(USART_Get_Byte(USART1,&c))
	{
		
		
		for(i = 0;i<10;i++)
		{
			if(_m_USART1_IRQHandler_Ptr[i] == 0)
			{
				break;
			}

			(* _m_USART1_IRQHandler_Ptr[i])(USART1,c);
		}
	}
}

/*����2�ж�*/
void USART2_IRQHandler(void)
{
	int i;
	unsigned char c = 0;
		
	while(USART_Get_Byte(USART2,&c))
	{
		
		
		for(i = 0;i<10;i++)
		{
			if(_m_USART2_IRQHandler_Ptr[i] == 0)
			{
				break;
			}

			(* _m_USART2_IRQHandler_Ptr[i])(USART2,c);
		}
	}
}


/**
 * ע���жϻص�����
 */
void Register_USART_Callback(USART_TypeDef* USARTx,void (* ptr)(USART_TypeDef* USARTx,unsigned char))
{
	int i;
	void (**_m_USARTx_IRQHandler_Ptr)(USART_TypeDef* USARTx,unsigned char);
	if(USARTx == USART1)
	{
		_m_USARTx_IRQHandler_Ptr = _m_USART1_IRQHandler_Ptr;
	}
	else if(USARTx == USART2)
	{
		_m_USARTx_IRQHandler_Ptr = _m_USART2_IRQHandler_Ptr;
	}
	
	
	for(i = 0;i<10;i++)
	{
		//�Ѿ�ע��
		if(_m_USARTx_IRQHandler_Ptr[i] == ptr)
		{
			return;
		}
		//����ƶ���δ���û�δռ��
		if(_m_USARTx_IRQHandler_Ptr[i] == 0)
		{
			_m_USARTx_IRQHandler_Ptr[i] = ptr;
			return;
		}
	}
}



