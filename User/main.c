#include "stm32f10x.h"

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

 
int main(void)
{
	SystemInit();
}
