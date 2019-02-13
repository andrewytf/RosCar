#include "odometry.h"

u8 once=1;

void TIM1_UP_IRQHandler(void)//��̼Ʒ�����ʱ���жϺ���
{
#ifdef DEBUG
		printf("enter Tim1 IRQ\r\n");
#endif 

	if( TIM_GetITStatus(TIM1 , TIM_IT_Update) != RESET ) 
	{	
		main_sta|=0x01;//ִ�з�����̼����ݲ���
		
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);//����жϱ�־λ  		 
	}		 
}

//��̼Ƽ��㺯��
void odometry(float right,float left) //dtʱ���ڵ��������ٶ�,������̼Ƽ���
{	
	if(once)  //����������һ��
	{
		const_frame=wheel_diameter*pi/(line_number*deceleration_ratio);//ÿ���������ߵľ���(��λmm)
		const_angle=const_frame/wheel_interval;//ÿ���������ߵĽǶ�
		once=0;
	}
    
	distance_sum = 0.5f*(right+left);//�ں̵ܶ�ʱ���ڣ�С����ʻ��·��Ϊ�����ٶȺ�
	distance_diff = right-left;//�ں̵ܶ�ʱ���ڣ�С����ʻ�ĽǶ�Ϊ�����ٶȲ�

    //���������ֵķ��򣬾�����ʱ���ڣ�С����ʻ��·�̺ͽǶ���������
	if((RecRightWheelSpeed>0)&&(RecLeftWheelSpeed>0))            //���Ҿ���
	{
		delta_distance = distance_sum;
		delta_oriention = distance_diff;
	}
	else if((RecRightWheelSpeed<0)&&(RecLeftWheelSpeed<0))       //���Ҿ���
	{
		delta_distance = -distance_sum;
		delta_oriention = -distance_diff;
	}
	else if((RecRightWheelSpeed<0)&&(RecLeftWheelSpeed>0))       //�����Ҹ�
	{
		delta_distance = -distance_diff;
		delta_oriention = -2.0f*distance_sum;		
	}
	else if((RecRightWheelSpeed>0)&&(RecLeftWheelSpeed<0))       //������
	{
		delta_distance = distance_diff;
		delta_oriention = 2.0f*distance_sum;
	}
	else
	{
		delta_distance=0;
		delta_oriention=0;
	}
    
	oriention_interval = delta_oriention * const_angle;//����ʱ�����ߵĽǶ�	
	oriention = oriention + oriention_interval;//�������̼Ʒ����
	oriention_1 = oriention + 0.5f * oriention_interval;//��̼Ʒ��������λ���仯���������Ǻ�������
	
    sin_ = sin(oriention_1);//���������ʱ����y����
	cos_ = cos(oriention_1);//���������ʱ����x����
	
    position_x = position_x + delta_distance * cos_ * const_frame;//�������̼�x����
	position_y = position_y + delta_distance * sin_ * const_frame;//�������̼�y����
    
	velocity_linear = delta_distance*const_frame / dt;//�������̼����ٶ�
	velocity_angular = oriention_interval / dt;//�������̼ƽ��ٶ�
	
    //����ǽǶȾ���
	if(oriention > pi)
	{
		oriention -= pi_2_1;
	}
	else
	{
		if(oriention < -pi)
		{
			oriention += pi_2_1;
		}
	}
}
