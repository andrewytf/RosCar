#include "encoder_data.h"


u8 LeftWheelFirstMes = 1,RightWheelFirstMes = 1;


void InfraredSendingTubeOnOff(u8 OnOff)
{
	if(OnOff == 1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_11); 
	}
	else if(OnOff == 0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_11); 
	}
}

void EXTI15_10_IRQHandler(void)//���ּ���
{
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)//EXTI10�������ж�
	{
		if(LeftWheelFirstMes == 1)
		{
			LeftWheelFirstMes = 0;
			//printf("LeftWheelFirstMes  \r\n");
		}
		else
		{
			if(L_ForwardReverse == Forward) //��ǰ
			{
				LeftWhellCount++;
				//printf("LeftWheelCurrentCount ++\r\n");
				//printf("LeftWheelCurrentCount %d \r\n",LeftWhellCount);
			}
			else if(L_ForwardReverse == Reverse) //���
			{
				LeftWhellCount--;
				//printf("LeftWheelCurrentCount --\r\n");
				//printf("LeftWheelCurrentCount %d \r\n",LeftWhellCount);
			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line10);//����ж�
}

void EXTI4_IRQHandler(void)//���ּ���
{
	if(RightWheelFirstMes == 1)
	{
		RightWheelFirstMes = 0;
		//printf("RightWheelFirstMes  \r\n");
	}
	else
	{
		if(R_ForwardReverse == Forward) //��ǰ
		{
			RightWhellCount++;
			//printf("RightWheelCurrentCount ++\r\n");
			//printf("RightWheelCurrentCount %d \r\n",RightWhellCount);
		}
		else if(R_ForwardReverse == Reverse) //���
		{
			RightWhellCount--;
			//printf("RightWheelCurrentCount --\r\n");
			//printf("RightWheelCurrentCount %d \r\n",RightWhellCount);
		}

	}
	EXTI_ClearITPendingBit(EXTI_Line4);//����ж�	
}


s32 readEncoder(u8 LeftOrRight)
{
	s32 encVal =0;
	if(LeftOrRight == LEFT)
	{
		IO_EXIT_LeftWheelSpeed_DE(ITDISABLE);//�ر��ж�
		encVal = LeftWhellCount;
		LeftWhellCount = 0;//����������ڼ�������ֵ
		IO_EXIT_LeftWheelSpeed_DE(ITENABLE);//���ж�
	}
	else if(LeftOrRight == RIGHT)
	{
		IO_EXIT_RightWheelSpeed_DE(ITDISABLE);//�ر��ж�
		encVal = RightWhellCount;
		RightWhellCount = 0;//����ҵ�����ڼ�������ֵ
		IO_EXIT_RightWheelSpeed_DE(ITENABLE);//���ж�
	}
	return encVal;
}

