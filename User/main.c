#include "includes.h"

unsigned char  DataStr[]=__DATE__;
unsigned char  TimeStr[]=__TIME__;

union recieveData  //���յ�������
{
	float d;    //�������ٶ�
	unsigned char data[4];
}leftdata,rightdata;       //���յ�����������

union odometry  //��̼����ݹ�����
{
	float odoemtry_float;
	unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;     //Ҫ��������̼����ݣ��ֱ�Ϊ��X��Y�����ƶ��ľ��룬��ǰ�Ƕȣ����ٶȣ����ٶ�


int main(void)
{
	u8 t=0;
	u8 i=0,j=0,m=0;

	//ϵͳʱ�ӳ�ʼ������
	SystemInit();
	UartInit(115200);
	printf("%s_%s_%d_%d \r\n", __DATE__, __TIME__, sizeof(__DATE__),sizeof(__TIME__));
	printf("Kenny Ros car!\r\n");
	//�����Դ�������ų�ʼ��
	IO_PeripheralPowerInit();
	//��ʼ�����ƺ�����ٹܿ���IO
	IO_InfraredSendingTubeInit();
	//�������Դ����
	PeripheralPowerOnOff(1);
	//�򿪺�����ٹܿ���
	InfraredSendingTubeOnOff(1);
	//�������ֱ������ж�
	IO_EXIT_LeftWheelSpeedInit();
	IO_EXIT_RightWheelSpeedInit();
	//����0.05ms   Ƶ��20Khz
	MotoControlTimInit(3199,0);
	TIM3_SpeedCalculate_Init();//�ٶȼ��㶨ʱ����ʼ��
	//TIM1_OdomUpdate_Init();//��̼Ʒ�����ʱ����ʼ��
	//����ȫ���ж����ȼ�
	NVIC_Configuration();
	
	while(1)
	{
		if(main_sta&0x01)//ִ�з�����̼����ݲ���
		{
			//��̼����ݻ�ȡ
			x_data.odoemtry_float=position_x;//��λmm
			y_data.odoemtry_float=position_y;//��λmm
			theta_data.odoemtry_float=oriention;//��λrad
			vel_linear.odoemtry_float=velocity_linear;//��λmm/s
			vel_angular.odoemtry_float=velocity_angular;//��λrad/s

			//��������̼����ݴ浽Ҫ���͵�����
			for(j=0;j<4;j++)
			{
			odometry_data[j]=x_data.odometry_char[j];
			odometry_data[j+4]=y_data.odometry_char[j];
			odometry_data[j+8]=theta_data.odometry_char[j];
			odometry_data[j+12]=vel_linear.odometry_char[j];
			odometry_data[j+16]=vel_angular.odometry_char[j];
			}

			odometry_data[20]='\n';//��ӽ�����

			//��������Ҫ����
			for(i=0;i<21;i++)
			{
				USART_ClearFlag(USART1,USART_FLAG_TC);	//�ڷ��͵�һ������ǰ�Ӵ˾䣬�����һ�����ݲ����������͵�����				
				USART_SendData(USART1,odometry_data[i]);//����һ���ֽڵ�����	
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //�ȴ����ͽ���			
			}

			main_sta&=0xFE;//ִ�м�����̼����ݲ���
		}
		if(main_sta&0x02)//ִ�м�����̼����ݲ���
		{
			odometry(Milemeter_R_Motor,Milemeter_L_Motor);//������̼�

			main_sta&=0xFD;//ִ�з�����̼����ݲ���
		} 
		if(main_sta&0x08)		 //������ָ��û����ȷ����ʱ
		{
			USART_ClearFlag(USART1,USART_FLAG_TC);	//�ڷ��͵�һ������ǰ�Ӵ˾䣬�����һ�����ݲ����������͵�����
			for(m=0;m<3;m++)
			{
				USART_SendData(USART1,0x00);	
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			}		
			USART_SendData(USART1,'\n');	
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
			main_sta&=0xF7;
		}
		if(USART_RX_STA&0x8000)  // ����1���պ���
		{			
		//�����������ٶ�
			for(t=0;t<4;t++)
			{
				rightdata.data[t]=USART_RX_BUF[t];
				leftdata.data[t]=USART_RX_BUF[t+4];
			}

			//�����������ٶ�
			RecRightWheelSpeed=rightdata.d;//��λmm/s
			RecLeftWheelSpeed=leftdata.d;//��λmm/s

			USART_RX_STA=0;//������ձ�־λ
		}

		//car_control(rightdata.d,leftdata.d);	 //�����յ����������ٶȸ���С��
		car_control(-500,-500);
	}
}
