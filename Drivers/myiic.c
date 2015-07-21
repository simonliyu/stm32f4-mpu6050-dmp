#include "myiic.h"
#include "delay.h"
#include "gpio.h"

//��ʼ��IIC
void myiic_init(void)
{
	gpio_out_pp_up_init(GPIOB, GPIO_Pin_8|GPIO_Pin_9);
	IIC_SCL=1;
	IIC_SDA=1;
}
//����IIC��ʼ�ź�
void myiic_start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void myiic_stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t myiic_wait_ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			myiic_stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void myiic_ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void myiic_nack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void myiic_send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t myiic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        myiic_nack();//����nACK
    else
        myiic_ack(); //����ACK   
    return receive;
}

uint8_t myiic_write_one_byte(uint8_t dev,uint8_t reg,uint8_t data)			 
{ 
    myiic_start(); 
	myiic_send_Byte(dev);//����������ַ+д����	
	if(myiic_wait_ack())	//�ȴ�Ӧ��
	{
		myiic_stop();		 
		return 1;		
	}
    myiic_send_Byte(reg);	//д�Ĵ�����ַ
    myiic_wait_ack();		//�ȴ�Ӧ�� 
	myiic_send_Byte(data);//��������
	if(myiic_wait_ack())	//�ȴ�ACK
	{
		myiic_stop();	 
		return 1;		 
	}		 
    myiic_stop();	 
	return 0;
}
//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
uint8_t myiic_read_one_byte(uint8_t dev,uint8_t reg)
{
	uint8_t res;
    myiic_start(); 
	myiic_send_Byte(dev);//����������ַ+д����	
	myiic_wait_ack();		//�ȴ�Ӧ�� 
    myiic_send_Byte(reg);	//д�Ĵ�����ַ
    myiic_wait_ack();		//�ȴ�Ӧ��
    myiic_start();
	myiic_send_Byte(dev+1);//����������ַ+������	
    myiic_wait_ack();		//�ȴ�Ӧ�� 
	res=myiic_read_byte(0);//��ȡ����,����nACK 
    myiic_stop();			//����һ��ֹͣ���� 
	return res;		
}

uint8_t myiic_write_bytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *buffer)
{
	uint8_t i;
	myiic_start(); 
	myiic_send_Byte(dev);		//����������ַ+д����	
	if(myiic_wait_ack())		//�ȴ�Ӧ��
	{
		myiic_stop();		 
		return 1;		
	}
    myiic_send_Byte(reg);		//д�Ĵ�����ַ
    myiic_wait_ack();			//�ȴ�Ӧ�� 
	for(i=0; i<len; i++){
		myiic_send_Byte(buffer[i]);		//��������
		if(myiic_wait_ack())			//�ȴ�ACK
		{
			myiic_stop();	 
			return 1;		 
		}		 
	}
    myiic_stop();	 
	return 0;
}

uint8_t myiic_read_bytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *buffer)
{
	uint8_t i;
    myiic_start(); 
	myiic_send_Byte(dev);	//����������ַ+д����	
	myiic_wait_ack();		//�ȴ�Ӧ�� 
    myiic_send_Byte(reg);	//д�Ĵ�����ַ
    myiic_wait_ack();		//�ȴ�Ӧ��
    myiic_start();
	myiic_send_Byte(dev+1); //����������ַ+������	
    myiic_wait_ack();		//�ȴ�Ӧ�� 
	for(i=0; i<len; i++){
		if(i == len-1){
			buffer[i] = myiic_read_byte(0);//��ȡ����,����nACK 
		}
		else{
			buffer[i] = myiic_read_byte(1);//��ȡ����,����ACK 
		}
	}
    myiic_stop();			//����һ��ֹͣ���� 
	return 0;	
}

