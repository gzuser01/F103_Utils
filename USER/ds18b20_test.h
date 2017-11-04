#ifndef __DS18B20_TEST_H
#define	__DS18B20_TEST_H


/**
 * ds18b20测试
 * PA1 - out 
 * 5V
 *
 */


#include "usart1_util.h"
#include "timx.h"

#define  SkipROM    0xCC     //跳过ROM  
#define  SearchROM  0xF0  //搜索ROM  
#define  ReadROM    0x33  //读ROM  
#define  MatchROM   0x55  //匹配ROM  
#define  AlarmROM   0xEC  //告警ROM  
  
#define  StartConvert    0x44  //开始温度转换，在温度转换期间总线上输出0，转换结束后输出1  
#define  ReadScratchpad  0xBE  //读暂存器的9个字节  
#define  WriteScratchpad 0x4E  //写暂存器的温度告警TH和TL  
#define  CopyScratchpad  0x48  //将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1  
#define  RecallEEPROM    0xB8    //将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1  
#define  ReadPower       0xB4    //读电源的供电方式：0为寄生电源供电；1为外部电源供电  


#define EnableINT()    
#define DisableINT()   
  
#define DS_PORT   GPIOA  
#define DS_DQIO   GPIO_Pin_1  
#define DS_RCC_PORT  RCC_APB2Periph_GPIOA  
#define DS_PRECISION 0x7f   //精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;  
#define DS_AlarmTH  0x64  
#define DS_AlarmTL  0x8a  
#define DS_CONVERT_TICK 1000  
  
#define ResetDQ() GPIO_ResetBits(DS_PORT,DS_DQIO)  
#define SetDQ()  GPIO_SetBits(DS_PORT,DS_DQIO)  
#define GetDQ()  GPIO_ReadInputDataBit(DS_PORT,DS_DQIO)  
   
   
static unsigned char TempX_TAB[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};  
  
  
void Delay_us(u32 Nus)   
{    
	SysTick->LOAD=Nus*9;          //时间加载         
	SysTick->CTRL|=0x01;             //开始倒数       
	while(!(SysTick->CTRL&(1<<16))); //等待时间到达    
	SysTick->CTRL=0X00000000;        //关闭计数器   
	SysTick->VAL=0X00000000;         //清空计数器        
}    
  
   
  
unsigned char ResetDS18B20(void)  
{  
	unsigned char resport;  
	SetDQ();  
	Delay_us(50);  
	 
	ResetDQ();  
	Delay_us(500);  //500us （该时间的时间范围可以从480到960微秒）  
	SetDQ();  
	Delay_us(40);  //40us  
	//resport = GetDQ();  
	while(GetDQ());  
	Delay_us(500);  //500us  
	SetDQ();  
	return resport;  
}  
  
void DS18B20WriteByte(unsigned char Dat)  
{  
	unsigned char i;  
	for(i=8;i>0;i--)  
	{  
		ResetDQ();     //在15u内送数到数据线上，DS18B20在15-60u读数  
		Delay_us(5);    //5us  
		if(Dat & 0x01)  
			SetDQ();  
		else  
			ResetDQ();  
		Delay_us(65);    //65us  
		SetDQ();  
		Delay_us(2);    //连续两位间应大于1us  
		Dat >>= 1;   
	}   
}  
  
  
unsigned char DS18B20ReadByte(void)  
{  
	unsigned char i,Dat;  
	SetDQ();  
	Delay_us(5);  
	for(i=8;i>0;i--)  
	{  
		Dat >>= 1;  
    ResetDQ();     //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后  
		Delay_us(5);   //5us  
		SetDQ();  
		Delay_us(5);   //5us  
		if(GetDQ())  
			Dat|=0x80;  
		else  
			Dat&=0x7f;    
		Delay_us(65);   //65us  
		SetDQ();  
	}  
	return Dat;  
}  
  
  
void ReadRom(unsigned char *Read_Addr)  
{  
	unsigned char i;  

	DS18B20WriteByte(ReadROM);  
		
	for(i=8;i>0;i--)  
	{  
		*Read_Addr=DS18B20ReadByte();  
		Read_Addr++;  
	}  
}  
  
  
void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)  
{  
	DisableINT();  
	ResetDS18B20();  
	DS18B20WriteByte(SkipROM);   
	DS18B20WriteByte(WriteScratchpad);  
	DS18B20WriteByte(AlarmTL);  
	DS18B20WriteByte(AlarmTH);  
	DS18B20WriteByte(Precision);  

	ResetDS18B20();  
	DS18B20WriteByte(SkipROM);   
	DS18B20WriteByte(CopyScratchpad);  
	EnableINT();  

	while(!GetDQ());  //等待复制完成 ///////////  
}  
  
  
void DS18B20StartConvert(void)  
{  
	DisableINT();  
	ResetDS18B20();  
	DS18B20WriteByte(SkipROM);   
	DS18B20WriteByte(StartConvert);   
	EnableINT();  
}  
  
void DS18B20_Configuration(void)  
{  
	GPIO_InitTypeDef GPIO_InitStructure;  
	 
	RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = DS_DQIO;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //开漏输出  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //2M时钟速度  
	GPIO_Init(DS_PORT, &GPIO_InitStructure);  
}  
  
  
void ds18b20_start(void)  
{  
	DS18B20_Configuration();  
	DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);  
	DS18B20StartConvert();  
}  
  
  
float ds18b20_read(void)
{  
	unsigned char TemperatureL,TemperatureH;  
	unsigned short TemperatureData;
	float Temperature;


	DisableINT();  
	ResetDS18B20();  
	DS18B20WriteByte(SkipROM);   
	DS18B20WriteByte(ReadScratchpad);  
	TemperatureL=DS18B20ReadByte();  
	TemperatureH=DS18B20ReadByte();   
	ResetDS18B20();  
	EnableINT();  
  
	if(TemperatureH & 0x80)  
	{  
		TemperatureH=(~TemperatureH) | 0x08;  
		TemperatureL=~TemperatureL+1;  
		if(TemperatureL==0)  
			TemperatureH+=1;  
	}  
  
	TemperatureData = TemperatureH;
	TemperatureData <<= 8;
	TemperatureData |= TemperatureL;

	Temperature = (float)((float)TemperatureData * 0.0625); //分辨率为0.0625度

	DS18B20StartConvert();  

	return  Temperature;  
}  



void ds18b20_test(void)
{
	//struct Linked_List_Data USART1_Data;
	
	float val;
	char buffer[16];
	uint32_t i;

	TIM_TypeDef* USART1_Callback_TIMx = TIM3;

	GPIO_TypeDef *USART1_Tx_GPIOx = GPIOA;
	uint16_t USART1_Tx_GPIO_Pin = GPIO_Pin_9;

	GPIO_TypeDef *USART1_Rx_GPIOx = GPIOA;
	uint16_t USART1_Rx_GPIO_Pin = GPIO_Pin_10;

	TIMx_With_NVIC_Config(USART1_Callback_TIMx,7199,99,NVIC_PriorityGroup_0,0,0); 

	USART1_Config(USART1_Tx_GPIOx,USART1_Tx_GPIO_Pin,USART1_Rx_GPIOx,USART1_Rx_GPIO_Pin); //USART1 配置 

	//USART1_Data_Init(&USART1_Data);
	
	
	
	ds18b20_start();
	while(1)
	{ 
		for(i=10000000;i>0;i--);
		val = ds18b20_read();
		
		for(i=0;i<16;i++)
			buffer[i] = 0;
		
		sprintf(buffer, "%f", val);
		
		for(i=0;i<16;i++)
			USART1_Send_Byte(buffer[i]);

		USART1_Send_Byte('\r');
		USART1_Send_Byte('\n');


		
	}
	
}



#endif 
