#include "moto_control.h"




void MotoControlTimInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	   //ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);   //ʹ������ʱ��ʹ�� 
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);		 //TIM4ѡ��ȫ���ù���ʹ�� 

	//��������Ϊ�����������,���TIM4 CH1��TIM4 CH2��TIM4 CH3��TIM4 CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;		//TIM_CH1��TIM_CH2��TIM_CH3��TIM_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	TIM_TimeBaseStructure.TIM_Period = arr;		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;		  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	  //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100; 			//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);		  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	//CH1Ԥװ��ʹ��

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100; 			//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //�������:TIM����Ƚϼ��Ը�	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);		  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	//CH2Ԥװ��ʹ��  

	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100; 			//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);		  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	//CH3Ԥװ��ʹ��  


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	 //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100; 			//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	  //�������:TIM����Ƚϼ��Ը�	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);		  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	//CH4Ԥװ��ʹ��  
	
	TIM_ARRPreloadConfig(TIM4, ENABLE);	   //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM4, ENABLE); 		  //ʹ��TIM3  

}

void LeftMotorStop(void)
{
	TIM4->CCR3 = 3200;
	TIM4->CCR4 = 3200;
}

void RightMotorStop(void)
{
	TIM4->CCR1 = 3200;
	TIM4->CCR2 = 3200;
}

void ENC_Calc_Average_Speed(void)//�������ε����ƽ��������
{   
    u32 i;
	signed long long wtempLL=0;
	signed long long wtempRR=0;

    //�ۼӻ�������ڵ��ٶ�ֵ
	for (i=0;i<SPEED_BUFFER_SIZE;i++)
	{
		wtempLL += hSpeed_BufferL[i];
		wtempRR += hSpeed_BufferR[i];
	}
    
    //ȡƽ��
	wtempLL /= (SPEED_BUFFER_SIZE);
	wtempRR /= (SPEED_BUFFER_SIZE);	
    
    //��ƽ����������λתΪ r/min
	wtempLL = (wtempLL * SPEED_SAMPLING_FREQ)*60/(ENCODER_L_PPR);
	wtempRR = (wtempRR * SPEED_SAMPLING_FREQ)*60/(ENCODER_R_PPR); 

	if(wtempLL < 0){wtempLL = 0 - wtempLL;}//ת��Ϊ����
	if(wtempRR < 0){wtempRR = 0 - wtempRR;}//ת��Ϊ����
	
	hRot_Speed_L= ((s16)(wtempLL));//ƽ��ת�� r/min
	hRot_Speed_R= ((s16)(wtempRR));//ƽ��ת�� r/min
	SpeedL=hRot_Speed_L;//ƽ��ת�� r/min
	SpeedR=hRot_Speed_R;//ƽ��ת�� r/min
#ifdef DEBUG
	printf("ENC_Calc_Average_Speed() SpeedL:%d,SpeedR:%d \r\n",SpeedL,SpeedR);
#endif 

}

float MaxValue=9;//�������޷�
float MinValue=-9;//�����С�޷�
float OutputValue;//PID����ݴ����,���ڻ��ֱ�������


float PID_calculate(struct PID *Control,float CurrentValue_left )//λ��PID����B
{
	
	float Value_Kp;//��������
	float Value_Ki;//���ַ���
	float Value_Kd;//΢�ַ���
	
	Control->error_0 = Control->OwenValue - CurrentValue_left + 0*span;//����������Control->OwenValueΪ��Ҫ���ٶȣ�CurrentValue_leftΪ�����ʵ�ٶ�
	Value_Kp = Control->Kp * Control->error_0 ;
	Control->Sum_error += Control->error_0;     
	
    /***********************���ֱ�������********************************************/
    OutputValue = Control->OutputValue;
    if(OutputValue>5 || OutputValue<-5)	
    {
        Control->Ki = 0; 
    }
    /*******************************************************************/
	
	Value_Ki = Control->Ki * Control->Sum_error;
	Value_Kd = Control->Kd * ( Control->error_0 - Control->error_1);
	Control->error_1 = Control->error_0;//����һ��г��
	Control->OutputValue = Value_Kp  + Value_Ki + Value_Kd;//���ֵ���㣬ע��Ӽ�
	
    //�޷�
	if( Control->OutputValue > MaxValue)
		Control->OutputValue = MaxValue;
	if (Control->OutputValue < MinValue)
		Control->OutputValue = MinValue;
    
	return (Control->OutputValue) ;
}


void Gain_R(void)//�����ҵ�� PID����
{
    
	span=1*(SpeedL-SpeedR);//�ɼ��������������ٶȲ�ֵ
	pulse_R= pulse_R + PID_calculate(&Control_right,hRot_Speed_R);//PID����
    
    //pwm��������
	if(pulse_R > 3200) pulse_R = 3200;
	if(pulse_R < 0) pulse_R = 0;
}


void Gain_L(void)//�������� PID���� 
{
    float pulse_L_IN1,pulse_L_IN2,pulse_R_IN1,pulse_R_IN2;
	span=1*(SpeedR-SpeedL);//�ɼ��������������ٶȲ�ֵ
	pulse_L= pulse_L + PID_calculate(&Control_left,hRot_Speed_L);//PID����
    
    ////pwm ��������
	if(pulse_L > 3200) pulse_L = 3200;
	if(pulse_L < 0) pulse_L = 0;
	
	if(L_ForwardReverse == Forward)
	{
		pulse_L_IN2 = 0;
		pulse_L_IN1 = pulse_L;	
	}
	else if(L_ForwardReverse == Reverse)
	{
		pulse_L_IN2 = pulse_L;
		pulse_L_IN1 = 0;		
	}

	if(R_ForwardReverse == Forward)
	{
		pulse_R_IN1 = 0;
		pulse_R_IN2 = pulse_R;
	}
	else if(R_ForwardReverse == Reverse)
	{
		pulse_R_IN1 = pulse_R;
		pulse_R_IN2 = 0;	
	}

	TIM4->CCR1 = pulse_R_IN2;//�ҵ����ֵPWM
    TIM4->CCR2 = pulse_R_IN1;//�ҵ����ֵPWM
	TIM4->CCR3 = pulse_L_IN1;//������ֵPWM
    TIM4->CCR4 = pulse_L_IN2;//������ֵPWM    
#ifdef DEBUG
	printf("TIM4->CCR1:%f\r\n",pulse_R_IN2);
	printf("TIM4->CCR2:%f\r\n",pulse_R_IN1);
	printf("TIM4->CCR3:%f\r\n",pulse_L_IN1);
	printf("TIM4->CCR4:%f\r\n",pulse_L_IN2);
#endif

}


void TIM3_IRQHandler(void)//С���ٶȼ��㶨ʱ���жϺ���
{
#ifdef DEBUG
	printf("enter Tim3 IRQ\r\n");
#endif 
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{						      
        if (hSpeedMeas_Timebase_500us !=0)//����������ɼ�ʱ����δ��
        {
            hSpeedMeas_Timebase_500us--;//��ʼ����	
        }
        else    //����������ɼ�ʱ��������
        {
            s32 wtempR,wtempL;
            
            hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;//�ָ�����������ɼ�ʱ����
            
            /************************ 1 ***************************/
            
            wtempL = readEncoder(LEFT); //��ȡ�����ӵı�����
            wtempR = readEncoder(RIGHT); //��ȡ�����ӵı�����
            
//            //���Ϊָֹͣ����������ٶ�Ϊ�㣬������ٶȴ洢����ֹǰ���ٶȲ�̫�����С����ת
//            if((wtempR == 0) && (wtempL == 0))
//            {
//                pulse=pulse1=0;
//            }
             
            /************************ 2 ***************************/
            
            //���������������������������̼Ƽ���
            Milemeter_L_Motor= (float)wtempL; //����������
            Milemeter_R_Motor= (float)wtempR;
            
            main_sta|=0x02;//ִ�м�����̼����ݲ���

            /************************ 3 ***************************/
            
            //��ʼ���������ֱ�����������
            hSpeed_BufferL[bSpeed_Buffer_Index] = wtempL;
            hSpeed_BufferR[bSpeed_Buffer_Index] = wtempR;
            bSpeed_Buffer_Index++;//������λ
            
            //���������ֱ���������������ж�
            if(bSpeed_Buffer_Index >=SPEED_BUFFER_SIZE)
            {
                bSpeed_Buffer_Index=0;//���������ֱ������������������
            }
            
            /************************ 4 ***************************/
            
            ENC_Calc_Average_Speed();//�������ε����ƽ��������
            Gain_R(); //���Aת��PID���ڿ��� ��
            Gain_L(); //���Bת��PID���ڿ��� ��
        }
        
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);//����жϱ�־λ    		 
	}		 
}

void LeftMovingSpeedW(unsigned int val)//���ַ�����ٶȿ��ƺ���
{     
    if(val>10000)
    {  
    	/* ��motor  ��ת*/
		L_ForwardReverse = Forward;
        Control_left.OwenValue=(val-10000);//PID���ڵ�Ŀ�������			
    }
    else if(val<10000)
    {  
    	/* ��motor  ��ת*/
		L_ForwardReverse = Reverse;
        Control_left.OwenValue=(10000-val);//PID���ڵ�Ŀ�������	 
    }	
    else
    {
		 L_ForwardReverse = STOP;
         Control_left.OwenValue=0;//PID���ڵ�Ŀ�������
    }	
#ifdef 0
		printf("LeftMovingSpeedW() L_ForwardReverse %d \r\n",L_ForwardReverse);
		printf("LeftMovingSpeedW() Control_left.OwenValue %f \r\n",Control_left.OwenValue);
#endif  

}

void RightMovingSpeedW(unsigned int val2)//���ַ�����ٶȿ��ƺ���
{    
    if(val2>10000)
    {  
        /* ��motor  ��ת*/
		R_ForwardReverse = Forward;
        Control_right.OwenValue=(val2-10000);//PID���ڵ�Ŀ�������
    }
    else if(val2<10000)
    {  
        /* ��motor  ��ת*/
		R_ForwardReverse = Reverse;
        Control_right.OwenValue=(10000-val2);//PID���ڵ�Ŀ�������	       
    }	
    else
    {
        R_ForwardReverse = STOP;
        Control_right.OwenValue=0;//PID���ڵ�Ŀ�������
    }	
#ifdef 0
	printf("RightMovingSpeedW() R_ForwardReverse %d \r\n",R_ForwardReverse);
	printf("RightMovingSpeedW() Control_right.OwenValue %f \r\n",Control_right.OwenValue);
#endif  
}

void car_control(float rightspeed,float leftspeed)//С���ٶ�ת���Ϳ��ƺ���
{
    float k2=14.1947;      //�������ٶ�ת���ɵ��ת�� mm/sת���� r/min              	��ʽ=60/pi/����ֱ��*���ٱ�
    
    int right_speed=(int)k2*rightspeed;
    int left_speed=(int)k2*leftspeed;
    RightMovingSpeedW(right_speed+10000);//10000��ֵȷ�����ɻ����������ʻ�ٶȾ��������������ʻ�ٶ�Ϊ1000mm/s,��ôת����17179,�˴���Intת��unsigned int
    LeftMovingSpeedW(left_speed+10000);
}

