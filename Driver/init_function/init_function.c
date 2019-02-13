#include "init_function.h"



void SystemInit (void)
{
	__IO uint32_t HSIStartUpStatus = 0;
	// �� RCC �����ʼ���ɸ�λ״̬������Ǳ����
	RCC_DeInit();

	//ʹ�� HSI
	RCC_HSICmd(ENABLE);

	// �ȴ� HSI ����
	HSIStartUpStatus = RCC->CR & RCC_CR_HSIRDY;

	// ֻ�� HSI ����֮�����������ִ��
	if (HSIStartUpStatus == RCC_CR_HSIRDY) 
	{
		//-------------------------------------------------------------//

		// ʹ�� FLASH Ԥ��ȡ������
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		// SYSCLK �������������ʱ��ı������ã�����ͳһ���ó� 2
		// ���ó� 2 ��ʱ��SYSCLK ���� 48M Ҳ���Թ�����������ó� 0 ���� 1 ��ʱ��
		// ������õ� SYSCLK �����˷�Χ�Ļ���������Ӳ�����󣬳��������
		// 0��0 < SYSCLK <= 24M
		// 1��24< SYSCLK <= 48M
		// 2��48< SYSCLK <= 72M
		FLASH_SetLatency(FLASH_Latency_2);
		//------------------------------------------------------------//

		// AHB Ԥ��Ƶ��������Ϊ 1 ��Ƶ��HCLK = SYSCLK
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		// APB2 Ԥ��Ƶ��������Ϊ 1 ��Ƶ��PCLK2 = HCLK
		RCC_PCLK2Config(RCC_HCLK_Div1);

		// APB1 Ԥ��Ƶ��������Ϊ 1 ��Ƶ��PCLK1 = HCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);

		//-----------���ø���Ƶ����Ҫ��������������-------------------//
		// ���� PLL ʱ����ԴΪ HSE������ PLL ��Ƶ����
		// PLLCLK = 4MHz * 16
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);
		//-- -----------------------------------------------------//

		// ���� PLL
		RCC_PLLCmd(ENABLE);

		// �ȴ� PLL �ȶ�
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}

		// �� PLL �ȶ�֮�󣬰� PLL ʱ���л�Ϊϵͳʱ�� SYSCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// ��ȡʱ���л�״̬λ��ȷ�� PLLCLK ��ѡΪϵͳʱ��
		while (RCC_GetSYSCLKSource() != 0x08) {}
	} 
	else 
	{
		// ��� HSI ����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
		// �� HSE ����ʧ�ܻ��߹��ϵ�ʱ�򣬵�Ƭ�����Զ��� HSI ����Ϊϵͳʱ�ӣ�
		// HSI ���ڲ��ĸ���ʱ�ӣ�8MHZ
		while (1) {}
	}
}


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
	//��׼����Ҫ��֧�ֺ���                 
	struct __FILE 
	{ 
		int handle; 

	}; 

	FILE __stdout;       
	//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
	_sys_exit(int x) 
	{ 
		x = x; 
	} 
	//�ض���fputc���� 
	int fputc(int ch, FILE *f)
	{      
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	    USART1->DR = (u8) ch;      
		return ch;
	}
#endif 

void UartInit(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}


void IO_PeripheralPowerInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void IO_InfraredSendingTubeInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void IO_EXIT_LeftWheelSpeedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//1.ʹ��GPIOD��AFIOʱ��,ֵ��ע����ǣ���ʹ���ⲿ�жϵ�ʱ�����ʹ��AFIOʱ�ӡ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
	//2.GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO�ٶ�Ϊ50MHz
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//3.����EXTI��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource10);//��EXIT��10���ӵ�PD10
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж���
	EXTI_Init(&EXTI_InitStructure);//��ʼ���ж�
#if 0

	//4.�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

void IO_EXIT_LeftWheelSpeed_DE(u8 enable_or_disable)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	if(enable_or_disable == ITENABLE)
	{
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);	
	}
	else if(enable_or_disable == ITDISABLE)
	{
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);			
	}
}
void IO_EXIT_RightWheelSpeedInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//1.ʹ��GPIOE��AFIOʱ��,ֵ��ע����ǣ���ʹ���ⲿ�жϵ�ʱ�����ʹ��AFIOʱ�ӡ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
	//2.GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO�ٶ�Ϊ50MHz
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	//3.����EXTI��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);//��EXIT��4���ӵ�PE4
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//�����½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж���
	EXTI_Init(&EXTI_InitStructure);//��ʼ���ж�
#if 0
	//4.�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

void IO_EXIT_RightWheelSpeed_DE(u8 enable_or_disable)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	if(enable_or_disable == ITENABLE)
	{
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

	}
	else if(enable_or_disable == ITDISABLE)
	{
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
		NVIC_Init(&NVIC_InitStructure);

	}	
}

void TIM3_SpeedCalculate_Init(void)//�ٶȼ��㶨ʱ����ʼ��
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	
   /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
   TIM_TimeBaseStructure.TIM_Period=3999;  //0.5ms 2000hz
	
   /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
   TIM_TimeBaseStructure.TIM_Prescaler= 7;
	
   /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
   TIM_ClearFlag(TIM3, TIM_FLAG_Update);
   TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);		
   TIM_Cmd(TIM3, ENABLE);			
}

void TIM1_OdomUpdate_Init(void)//��̼Ʒ�����ʱ����ʼ��
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
   TIM_DeInit(TIM1);
	

   /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */ 
   TIM_TimeBaseStructure.TIM_Period=399;  //20HZ  500*100us=50ms 
   TIM_TimeBaseStructure.TIM_Prescaler=7999; 
	
	 /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;	
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
   TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
   TIM_ClearFlag(TIM1, TIM_FLAG_Update);
   TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);		
   TIM_Cmd(TIM1, ENABLE);	
}

void NVIC_Configuration(void)//���ȼ�����
{
    NVIC_InitTypeDef NVIC_InitStructure;	
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//���ж�ʸ���ŵ�Flash��0��ַ
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�������ȼ����õ�ģʽ,��1��:��ռ���ȼ�0(0:7),��ռ���ȼ�1(0:7),

    /****************************ʹ�ܴ���1�жϣ����������ȼ�***********************/
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			   //USART1ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	   //��ռ���ȼ� 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	           //��ռ���ȼ� 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  		 	   //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);	     	
		  	
	
	NVIC_InitStructure.NVIC_IRQChannel =EXTI4_IRQn;                //(��)�����ⲿ�жϺ���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel =EXTI15_10_IRQn;             //(��)�����ⲿ�жϺ���
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
                                                        
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	 
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}	

void USART1_IRQHandler(void)//�����жϺ���
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�Ƿ���ܵ�����
    {
		serial_rec =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
					
		if((USART_RX_STA&0x8000)==0)//����δ���
        {
            if(USART_RX_STA&0x4000)//���յ���0x0d
            {
                if(serial_rec==0x0a)
                {
                    if((USART_RX_STA&0x3f)==8)
                    {							
                        USART_RX_STA|=0x8000;	//��������� 
                        main_sta|=0x04;
                        main_sta&=0xF7;
                    }
                    else
                    {
                        main_sta|=0x08;
                        main_sta&=0xFB;
                        USART_RX_STA=0;//���մ���,���¿�ʼ
                    }
                }
                else 
                {
                    main_sta|=0x08;
                    USART_RX_STA=0;//���մ���,���¿�ʼ
                }
            }
            else //��û�յ�0X0D
            {	
                if(serial_rec==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=serial_rec ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))
                    {
                        main_sta|=0x08;
                        USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
                    }							
                }		 
            }
        }   		 
    }
}


