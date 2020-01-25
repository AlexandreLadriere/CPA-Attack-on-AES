
int main(void)
{
	
/*	    volatile int i;
  int n = 1;
    int brightness = 0;
	
	GPIO_InitTypeDef 	 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
*/	
//	USART_InitTypeDef 	 USART_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStructure; 
	
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* Configure all unused GPIO port pins in Analog Input mode (floating input
     trigger OFF), this will reduce the power consumption and increase the device
     immunity against EMI/EMC *************************************************/
  /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO |
                         RCC_APB2Periph_GPIOE, ENABLE);*/

	
	
	
/*	
	  RCC_APB2PeriphClockCmd(    RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
            RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
            RCC_APB2Periph_AFIO, ENABLE );
 
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
 
    // Set the Vector Table base address at 0x08000000.
    NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );
 
    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
 
    // Configure HCLK clock as SysTick clock source.
    SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );


    // Setup Blue LED on STM32-Discovery Board to use PWM.
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            // Alt Function - Push Pull
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE );        // Map TIM3_CH3 to GPIOC.Pin8
 
    // Let PWM frequency equal 100Hz.
    // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
    // Solving for prescaler gives 240.
    
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1000;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 240;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );
 
    
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    // Initial duty cycle equals 0%. Value can range from zero to 1000.
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC3Init( TIM3, &TIM_OCInitStruct );
    TIM_Cmd( TIM3, ENABLE ); 
	
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
 /*
 // Set USART1 Tx on PA9
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 // Set USART1 Rx on PA10
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  
  
 
  USART_ClockStructInit(&USART_ClockInitStructure);
  USART_ClockInit(USART1, &USART_ClockInitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
  //Write USART1 parameters
  USART_Init(USART1, &USART_InitStructure);  
*/



 

  /*RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO |
                         RCC_APB2Periph_GPIOE, DISABLE);*/

  /* Initialize Leds LD3 and LD4 mounted on STM32VLDISCOVERY board */
  //STM32vldiscovery_LEDInit(LED3);
  //STM32vldiscovery_LEDInit(LED4);








/*
  while (1)
  {
    //Turn on LD2 and LD3
    //STM32vldiscovery_LEDOn(LED3);
    //STM32vldiscovery_LEDOff(LED4);    
   // Insert delay
    //Delay(0xAFFFF);
    // Turn off LD3 and LD4
    //STM32vldiscovery_LEDOff(LED3);
    //STM32vldiscovery_LEDOn(LED4);
    // Insert delay
    //Delay(0xAFFFF);
		
		 //USART_SendData(USART1, 0xFFFF);
		 //while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ; 
		

    if (((brightness + n) >= 1000) || ((brightness + n) <= 0))
      n = -n; // if  brightness maximum/maximum change direction
 
        brightness += n;
 
    TIM3->CCR3 = brightness; // set brightness
    TIM3->CCR4 = 1000 - brightness; // set brightness
 
    for(i=0;i<10000;i++);  // delay
 
		
		

  }
*/



/*

  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;
 
    volatile int i;
  int n = 1;
    int brightness = 0;
 
    RCC_APB2PeriphClockCmd(
            RCC_APB2Periph_GPIOC |
            RCC_APB2Periph_AFIO, ENABLE );
 
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
 
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure
 
    // Setup Blue & Green LED on STM32-Discovery Board to use PWM.
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;            // Alt Function - Push Pull
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE );        // Map TIM3_CH3 to GPIOC.Pin8, TIM3_CH4 to GPIOC.Pin9
 
    // Let PWM frequency equal 100Hz.
    // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
    // Solving for prescaler gives 240.
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;   // 0..999
    TIM_TimeBaseInitStruct.TIM_Prescaler = 240 - 1; // Div 240
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );
 
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    // Initial duty cycle equals 0%. Value can range from zero to 1000.
    TIM_OCInitStruct.TIM_Pulse = 500; // 0 .. 1000 (0=Always Off, 1000=Always On)
 
    TIM_OC3Init( TIM3, &TIM_OCInitStruct ); // Channel 3 Blue LED
    TIM_OC4Init( TIM3, &TIM_OCInitStruct ); // Channel 4 Green LED
 
    TIM_Cmd( TIM3, ENABLE );
 
  while(1)  // Do not exit
  {
    //if (((brightness + n) >= 1000) || ((brightness + n) <= 0))
      n = -n; // if  brightness maximum/maximum change direction
 
        brightness += n;
 
    TIM3->CCR3 = brightness; // set brightness
    TIM3->CCR4 = 1000 - brightness; // set brightness
 
    for(i=0;i<10000;i++);  // delay
  }
 */
	
	return 0;
}