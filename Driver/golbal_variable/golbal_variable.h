#ifndef __GOLBAL_VARIABLE_H
#define __GOLBAL_VARIABLE_H


#include "includes.h"


#define Forward 1
#define Reverse 2
#define STOP 0

#define LEFT  1
#define RIGHT 2
#define ITENABLE 1
#define ITDISABLE 2


#define SPEED_SAMPLING_TIME  9    // (9+1)*500usec = 5MS   ,200hz
#define SPEED_BUFFER_SIZE 3       //�������ٶȻ��������С

#define SPEED_SAMPLING_FREQ (u16)(2000/(SPEED_SAMPLING_TIME+1))  //200hz��С���ٶȲ���Ƶ��
#define ENCODER_L_PPR           (u16)(16)  // ��	�����������
#define ENCODER_R_PPR           (u16)(16)  // �ҵ����������


struct PID
{
    float Kp;
    float Ki;
    float Kd;
    float error_0;//��������
    float error_1;//һ��г������
    float error_2;//����г������
    long  Sum_error;
    float OutputValue;//ʵ�������
    float OwenValue;//�����ʱ�ı�׼�����
};



extern u8 main_sta; //������������������if��ȥ�������flag��1��ӡ��̼ƣ���2���ü�����̼����ݺ�������3���ڽ��ճɹ�����4���ڽ���ʧ�ܣ�


/***********************************************  ״̬  *****************************************************************/
extern u8 L_ForwardReverse ;// 0��ֹ	1��ǰ	2���(����)
extern u8 R_ForwardReverse ;// 0��ֹ	1��ǰ	2���(�ҵ��)

/***********************************************  ���  *****************************************************************/

extern float position_x,position_y,oriention,velocity_linear,velocity_angular;
extern char odometry_data[21];   //���͸����ڵ���̼���������

/***********************************************  ����  *****************************************************************/
#define USART_REC_LEN  			200  	//�����������ֽ��� 200

extern u8 USART_RX_BUF[USART_REC_LEN];     //���ڽ��ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART_RX_STA;                   //���ڽ���״̬���	

extern float RecRightWheelSpeed,RecLeftWheelSpeed;//���ڵõ����������ٶ�

/***********************************************  ���̲���  *****************************************************************/

//���У��ֵ=ԭ���/0.987
extern float wheel_interval; //У�������ֵ����λmm       
extern float deceleration_ratio;  //���ٱ� 1:52
extern float wheel_diameter;     //����ֱ������λmm
extern float pi_1_2;			 //��/2
extern float pi;              //��
extern float pi_3_2;			 //��*3/2
extern float pi_2_1;			 //��*2
extern float dt;                 //����ʱ����5ms
extern float line_number;       //��������
extern float oriention_interval;  //dtʱ���ڷ���仯ֵ

extern float sin_;        //�Ƕȼ���ֵ
extern float cos_;

extern float delta_distance,delta_oriention;   //����ʱ�������˶��ľ���

extern float const_frame,const_angle,distance_sum,distance_diff;

extern float oriention_1;


extern int span;//�ɼ��������������ٶȲ�ֵ
extern s32 hSpeed_BufferL[SPEED_BUFFER_SIZE], hSpeed_BufferR[SPEED_BUFFER_SIZE];//�������ٶȻ�������
extern  unsigned int hRot_Speed_L;//����ƽ��ת�ٻ���
extern  unsigned int hRot_Speed_R;//�ҵ��ƽ��ת�ٻ���
extern unsigned int SpeedL; //����ƽ��ת�� r/min��PID����
extern unsigned int SpeedR; //�ҵ��ƽ��ת�� r/min��PID����
extern  unsigned short int hSpeedMeas_Timebase_500us;//����������ɼ�ʱ����
extern float  Milemeter_L_Motor,Milemeter_R_Motor;//dtʱ���ڵ��������ٶ�,������̼Ƽ���
extern u8 bSpeed_Buffer_Index;//���������ֱ��������������
extern float pulse_L;//���� PID���ں��PWMֵ����
extern float pulse_R;//�ҵ�� PID���ں��PWMֵ����

extern u8 serial_rec;   //���մ������ݱ���


extern s32 LeftWhellCount;//����������
extern s32 RightWhellCount;//����������
extern struct PID Control_left ;//����PID�����������µ��4096
extern struct PID Control_right;//����PID�����������µ��4096



#endif
