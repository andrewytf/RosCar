#ifndef __MOTO_CONTROL_H
#define __MOTO_CONTROL_H

#include "includes.h"

void MotoControlTimInit(u16 arr,u16 psc);
void LeftMotorStop(void);
void RightMotorStop(void);
void car_control(float rightspeed,float leftspeed);
void TIM3_IRQHandler(void);//С���ٶȼ��㶨ʱ���жϺ���



#endif
