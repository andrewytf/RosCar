#include "golbal_variable.h"



u8 main_sta=0; //������������������if��ȥ�������flag��1��ӡ��̼ƣ���2���ü�����̼����ݺ�������3���ڽ��ճɹ�����4���ڽ���ʧ�ܣ�



/***********************************************  ״̬  *****************************************************************/
u8 L_ForwardReverse = STOP;// 0��ֹ	1��ǰ	2���(����)
u8 R_ForwardReverse = STOP;// 0��ֹ	1��ǰ	2���(�ҵ��)


/***********************************************  ���  *****************************************************************/

float position_x=0,position_y=0,oriention=0,velocity_linear=0,velocity_angular=0;
char odometry_data[21]={0};   //���͸����ڵ���̼���������

/***********************************************  ����  *****************************************************************/
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;   //����״̬���	

float RecRightWheelSpeed,RecLeftWheelSpeed;//���ڵõ����������ٶ�

/***********************************************  ���̲���  *****************************************************************/

//���У��ֵ=ԭ���/0.987
float wheel_interval= 234.0426f; //У�������ֵ����λmm       
float deceleration_ratio=52.0f;  //���ٱ� 1:52
float wheel_diameter=70.0f;     //����ֱ������λmm
float pi_1_2=1.570796f;			 //��/2
float pi=3.141593f;              //��
float pi_3_2=4.712389f;			 //��*3/2
float pi_2_1=6.283186f;			 //��*2
float dt=0.005f;                 //����ʱ����5ms
float line_number=16.0f;       //��������
float oriention_interval=0;  //dtʱ���ڷ���仯ֵ

float sin_=0;        //�Ƕȼ���ֵ
float cos_=0;

float delta_distance=0,delta_oriention=0;   //����ʱ�������˶��ľ���

float const_frame=0,const_angle=0,distance_sum=0,distance_diff=0;

float oriention_1=0;

int span;//�ɼ��������������ٶȲ�ֵ
s32 hSpeed_BufferL[SPEED_BUFFER_SIZE]={0}, hSpeed_BufferR[SPEED_BUFFER_SIZE]={0};//�������ٶȻ�������
unsigned int hRot_Speed_L;//����ƽ��ת�ٻ���
unsigned int hRot_Speed_R;//�ҵ��ƽ��ת�ٻ���
unsigned int SpeedL=0; //����ƽ��ת�� r/min��PID����
unsigned int SpeedR=0; //�ҵ��ƽ��ת�� r/min��PID����
unsigned short int hSpeedMeas_Timebase_500us = SPEED_SAMPLING_TIME;//����������ɼ�ʱ����
float  Milemeter_L_Motor=0,Milemeter_R_Motor=0;//dtʱ���ڵ��������ٶ�,������̼Ƽ���
u8 bSpeed_Buffer_Index = 0;//���������ֱ��������������
float pulse_L = 0;//���� PID���ں��PWMֵ����
float pulse_R = 0;//�ҵ�� PID���ں��PWMֵ����

s32 LeftWhellCount = 0;//����������
s32 RightWhellCount = 0;//����������

u8 serial_rec=0x31;   //���մ������ݱ���

struct PID Control_left  ={0.01,0.1,0.75,0,0,0,0,0,0};//����PID�����������µ��4096
struct PID Control_right ={0.01,0.1,0.75,0,0,0,0,0,0};//����PID�����������µ��4096


