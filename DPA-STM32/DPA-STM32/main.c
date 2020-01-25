/**
  ******************************************************************************
  * @file    DPA-STM32/main.c
  * @author  LSAS
  * @version V3.0.0
  * @date    MAJ 25/02/2017
  * @brief   Chiffrement AES sur STM32 avec communication RS232
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_gpio.h"
#include "system_stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>


/** @addtogroup DPA-STM32
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/


/*
 * AES
 */
typedef uint8_t byte;
/* Sbox Lookup Table */
const byte sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
    0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
    0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
    0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
    0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
    0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
    0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
    0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
    0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
    0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
    0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
    0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
    0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
    0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
    0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
    0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
    0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

byte rcon [] = {
0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab
};

byte state[16];
byte key[16];

byte check[16];




void addRoundKey(void)
{
	int i;

	for(i=0; i < 16; i++)
	{
		state[i] ^= key[i];
	}
}


/* subBytes
 * Table Lookup
 */
void subBytes(void)
{
	int i;
	bool ok = false;
	time_t t;
	srand((unsigned) time(&));
	ini_check();
	while(!arrayIsFull()) 
	{
		int r = rand(0) % 16;
		if(check[r] == 0) 
		{
			check[r] = 1;
			state[r] = sbox[ state[r] ];
		}
		
	}

	/*
	for(i = 0; i < 16; i++)
	{
		state[i] = sbox[ state[i] ];
	}
	*/
}

void init_check() 
{
	for(int i = 0; i <16; i++)
	{
		check[i] = 0;
	}
}

bool arrayIsFull(void) 
{
	bool ok = true;
	for(int i = 0; i <16; i++)
	{
		if(check[i] == 0) {
			ok = false;
			return ok;
		}
	}
	return ok;
}

/* shiftRows                Before         After
 * Row 1: No change        0 4  8 12      0  4  8 12    
 * Row 2: Left Shift by 1  1 5  9 13  =>  5  9 13  1    
 * Row 3: Left Shift by 2  2 6 10 14     10 14  2  6   
 * Row 4: Left Shift by 3  3 7 11 15     15 3   7 11  
 */
void shiftRows(void)
{
	byte temp;

	//Row 2
	temp = state[1]; state[1] = state[5]; state[5] = state[9];
    state[9] = state[13]; state[13] = temp;
	//Row 3
	temp = state[10]; state[10] = state[2]; state[2] = temp;
	temp = state[14]; state[14] = state[6]; state[6] = temp;
	//Row 4
	temp = state[3]; state[3] = state[15]; state[15] = state[11]; 
    state[11] = state[7]; state[7] = temp;
}

/* MixColumns Shortcut 
 * Vunerable to timing attack, must add nop
 */
byte xtime(byte x)
{
	return (x & 0x80) ? ((x << 1) ^ 0x1b) : (x<<1);
}

/* Mix Columns */
void mixColumns(void)
{
	byte i, a, b, c, d, e;
	
	/* Process a column at a time */
	for(i = 0; i < 16; i+=4)
	{
		a = state[i]; b = state[i+1]; c = state[i+2]; d = state[i+3];
		e = a ^ b ^ c ^ d;
		state[i]   ^= e ^ xtime(a^b);
		state[i+1] ^= e ^ xtime(b^c);
		state[i+2] ^= e ^ xtime(c^d);
		state[i+3] ^= e ^ xtime(d^a);
	}
}
/* Compute a round of the key
 * 0 4  8 12
 * 1 5  9 13
 * 2 6 10 14
 * 3 7 11 15
 */
void computeKey(byte rcon)
{
	byte buf0, buf1, buf2, buf3;
	buf0 = sbox[ key[13] ];
	buf1 = sbox[ key[14] ];
	buf2 = sbox[ key[15] ];
	buf3 = sbox[ key[12] ];

	key[0] ^= buf0 ^ rcon;
	key[1] ^= buf1;
	key[2] ^= buf2;
	key[3] ^= buf3;

	key[4] ^= key[0];
	key[5] ^= key[1];
	key[6] ^= key[2];
	key[7] ^= key[3];

	key[8]  ^= key[4];
	key[9]  ^= key[5];
	key[10] ^= key[6];
	key[11] ^= key[7];

	key[12] ^= key[8];
	key[13] ^= key[9];
	key[14] ^= key[10];
	key[15] ^= key[11];
}
	

	

/**
  * @brief  Chiffrement AES
  * @retval None
  */
void AES_Run(void)
{
	int i;

	addRoundKey();
	for(i = 0; i < 9; i++)
	{
		if(i==0)
		{
			GPIOB->BSRR = GPIO_Pin_8;
			subBytes();	
			GPIOB->BRR = GPIO_Pin_8;
		}
		else
		{
			subBytes();	
		}	
		shiftRows();
		mixColumns();
		computeKey(rcon[i]);
		addRoundKey();
	}
	
	
	subBytes();
	shiftRows();
	computeKey(rcon[i]);
	addRoundKey();
}


/**
  * @brief  Initialisation de l'horloge principale et des horloges pour les bus et périphériques
  * @retval None
  */
void Clock_Init()
{
	ErrorStatus HSEStartUpStatus;
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	/* Wait till HSE is ready */
	{
		RCC_PLLConfig(/*RCC_PLLSource_HSE_Div1*/RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_ClockSecuritySystemCmd(ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SRAM|RCC_AHBPeriph_FLITF, ENABLE );
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
													 RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO |
													 RCC_APB2Periph_GPIOE, ENABLE);	
		
	} 
	else while (1);
	
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

}



/**
  * @brief  Initialisation du NVIC
  * @retval None
  */
void NVIC_Initialisation()
{

  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_SetPriority (MemoryManagement_IRQn, 0xF0);
	NVIC_SetPriority (BusFault_IRQn, 0x80);
	NVIC_SetPriority (UsageFault_IRQn, 0x10);
	
}


/**
  * @brief  Initialisation de la clé AES - Clé FIPS 197
  * @retval None
  */
void AES_InitKey()
{
	key[0]=0x17;
	key[1]=0x9e;
	key[2]=0x48;
	key[3]=0x16;
	key[4]=0x28;
	key[5]=0xae;
	key[6]=0xd2;
	key[7]=0xa6;
	key[8]=0xab;
	key[9]=0xf7;
	key[10]=0x15;
	key[11]=0x88;
	key[12]=0x09;
	key[13]=0xcf;
	key[14]=0x4f;
	key[15]=0x3c;
}


void AES_InitState()
{
	state[0]=0x00;
	state[1]=0x00;
	state[2]=0x00;
	state[3]=0x00;
	state[4]=0x00;
	state[5]=0x00;
	state[6]=0x00;
	state[7]=0x00;
	state[8]=0x00;
	state[9]=0x00;
	state[10]=0x00;
	state[11]=0x00;
	state[12]=0x00;
	state[13]=0x00;
	state[14]=0x00;
	state[15]=0x00;
}

/**
  * @brief  Initialisation du matériel
  * @retval None
  */
void Hardware_Init()
{
	GPIO_InitTypeDef 	 GPIO_InitStructure;
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef 	USART_ClockInitStructure; 	
	
	Clock_Init();
	
	// Fault Interruptions
	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA | SCB_SHCSR_BUSFAULTENA | SCB_SHCSR_MEMFAULTENA;
	
	// Programmable Voltage Detector
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
	PWR_PVDCmd (ENABLE);
	
	// USART1 Tx sur PA9
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// USART1 Rx sur PA10
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Trigger sur PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// NVIC
	NVIC_Initialisation();
 
	// Parametres USART
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockStructInit(&USART_ClockInitStructure);
  USART_ClockInit(USART1, &USART_ClockInitStructure);
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_Init(USART1, &USART_InitStructure);  
	USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  Réception du texte clair en RS232
  * @retval None
  */
void USART_ReceivePlainText()
{
	volatile char data;
	int i;
	while(data != 'G')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		data = USART_ReceiveData(USART1);
	}

	for(i=0;i<16;i++)
	{	
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		data = USART_ReceiveData(USART1);
		state[i] = data;
	}
   
	data=state[0];
}



/**
  * @brief  Renvoi du chiffré en RS232
  * @retval None
  */
void USART_SendCipherText()
{	
	int i;
	/*volatile char data*/;
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
	USART_SendData(USART1, 'R');
	
	for(i=0;i<16;i++)
	{	
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
		USART_SendData(USART1, state[i]);		
		/*data= state[0]*/;
	}

}



int main(void)
{
	int i=0;
	Hardware_Init();
	while(1)
	{
		i++;
		AES_InitState();
		AES_InitKey();

		USART_ReceivePlainText();

		AES_Run();
				
		USART_SendCipherText();
	}

	return 0;
}





#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

