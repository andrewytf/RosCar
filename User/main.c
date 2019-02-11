#include "stm32f10x.h"
#include "init_function.h"
#include "power_manage.h"
#include "encoder_data.h"




int main(void)
{
	//ϵͳʱ�ӳ�ʼ������
	SystemInit();
	UartInit(115200);
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
	IO_EXIT_LeftWheelSpeed();
	IO_EXIT_RightWheelSpeed();
	while(1)
	{

	}
}
