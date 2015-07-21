#ifndef __MYIIC_H__
#define __MYIIC_H__

#include "stm32f4xx.h"
#include "bitband.h"
   	   		   
//IO��������
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define IIC_SCL    PbOutBit(8) //SCL
#define IIC_SDA    PbOutBit(9) //SDA	 
#define READ_SDA   PbInBit(9)  //����SDA 

//IIC���в�������
void myiic_init(void);                //��ʼ��IIC��IO��				 
void myiic_start(void);				//����IIC��ʼ�ź�
void myiic_stop(void);	  			//����IICֹͣ�ź�
void myiic_send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t myiic_read_byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t myiic_wait_ack(void); 				//IIC�ȴ�ACK�ź�
void myiic_ack(void);					//IIC����ACK�ź�
void myiic_nack(void);				//IIC������ACK�ź�

uint8_t myiic_write_one_byte(uint8_t dev,uint8_t reg,uint8_t data);
uint8_t myiic_read_one_byte(uint8_t dev,uint8_t reg);
uint8_t myiic_write_bytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *buffer);
uint8_t myiic_read_bytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *buffer);

#endif
