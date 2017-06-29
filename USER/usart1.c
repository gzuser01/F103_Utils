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
void (*_m_usart1_irqhandler_ptr[])(unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART1_Config(GPIO_TypeDef *tx_gpiox,uint16_t tx_gpio_pin,GPIO_TypeDef *rx_gpiox,uint16_t rx_gpio_pin)
{

	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ʹ�� USART1 �ж� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ����鷽ʽ2
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	/* USART1 ʹ��IO�˿����� */    
	GPIO_Config(tx_gpiox,tx_gpio_pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//Tx �����������
	GPIO_Config(rx_gpiox,rx_gpio_pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);//Rx ��������
	
	  
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
void USART1_Send_Byte(unsigned char send_data)
{	   
	USART_SendData(USART1,send_data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*����һ���ֽ�����*/
unsigned char USART1_Get_Byte(unsigned char* get_data)
{   	   
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//û���յ����� 
	}
	
	*get_data = USART_ReceiveData(USART1); 
	
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



