/***************************************
 * 文件名  ：usart1.c
 * 描述    ：配置USART1         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  

**********************************************************************************/

#include "usart1.h"
#include "misc.h" 


/**
 * 串口中断回调函数的指针
 */
void (*_m_usart1_irqhandler_ptr[])(unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART1_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 使能 USART1 中断 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//选择分组方式0，最高
  
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//最高
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接受中断，在接受移位寄存器中有数据是产生
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能发送中断，在发送完数据后产生。
	
	USART_Cmd(USART1, ENABLE);// USART1使能
	
	

}

 /*发送一个字节数据*/
void UART1_Send_Byte(unsigned char SendData)
{	   
	USART_SendData(USART1,SendData);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*接收一个字节数据*/
unsigned char UART1_Get_Byte(unsigned char* GetData)
{   	   
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//没有收到数据 
	}
	
	*GetData = USART_ReceiveData(USART1); 
	
	return 1;//收到数据
}

/*串口中断*/
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
 * 注册中断回调函数
 */
void Register_USART1_Callback(void (* ptr)(unsigned char))
{
	int i;
	for(i = 0;i<10;i++)
	{
		//已经注册
		if(_m_usart1_irqhandler_ptr[i] == ptr)
		{
			return;
		}
		//序号移动到未设置或未占用
		if(_m_usart1_irqhandler_ptr[i] == 0)
		{
			_m_usart1_irqhandler_ptr[i] = ptr;
			return;
		}
	}
}



