
/**************************************

*********************************************************/



#include "stm32f10x.h"


//#include "gpio.h"

//#include "usart1_test.h"
//#include "pwm_led_test.h"
#include "srf05_test.h"

#include <stdlib.h>




int main(void)
{  
	     
	SystemInit();	//配置系统时钟为 72M 
	
  
	//TIMx_With_NVIC_Config(TIM2,7199,99,NVIC_PriorityGroup_2,0,0);
  //TIMx_With_NVIC_Config(TIM3,7199,99,NVIC_PriorityGroup_2,1,0);  
	//TIMx_With_NVIC_Config(TIM4,7199,99,NVIC_PriorityGroup_2,2,0); 
	
	//usart1_test();
	
	//pwm_led_test();
	
	//GPIO_Config(GPIOC,GPIO_Pin_13,GPIO_Mode_IPD,GPIO_Speed_2MHz);
	
	srf05_test();

  while (1)
  {	 
		
		//ReadValue = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
    //UART1_Send_Byte(ReadValue);
  }
	
}




