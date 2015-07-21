#include "myiic.h"
#include "delay.h"
#include "gpio.h"

//初始化IIC
void myiic_init(void)
{
	gpio_out_pp_up_init(GPIOB, GPIO_Pin_8|GPIO_Pin_9);
	IIC_SCL=1;
	IIC_SDA=1;
}
//产生IIC起始信号
void myiic_start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void myiic_stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t myiic_wait_ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void myiic_send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t myiic_read_byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        myiic_nack();//发送nACK
    else
        myiic_ack(); //发送ACK   
    return receive;
}

uint8_t myiic_write_one_byte(uint8_t dev,uint8_t reg,uint8_t data)			 
{ 
    myiic_start(); 
	myiic_send_Byte(dev);//发送器件地址+写命令	
	if(myiic_wait_ack())	//等待应答
	{
		myiic_stop();		 
		return 1;		
	}
    myiic_send_Byte(reg);	//写寄存器地址
    myiic_wait_ack();		//等待应答 
	myiic_send_Byte(data);//发送数据
	if(myiic_wait_ack())	//等待ACK
	{
		myiic_stop();	 
		return 1;		 
	}		 
    myiic_stop();	 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
uint8_t myiic_read_one_byte(uint8_t dev,uint8_t reg)
{
	uint8_t res;
    myiic_start(); 
	myiic_send_Byte(dev);//发送器件地址+写命令	
	myiic_wait_ack();		//等待应答 
    myiic_send_Byte(reg);	//写寄存器地址
    myiic_wait_ack();		//等待应答
    myiic_start();
	myiic_send_Byte(dev+1);//发送器件地址+读命令	
    myiic_wait_ack();		//等待应答 
	res=myiic_read_byte(0);//读取数据,发送nACK 
    myiic_stop();			//产生一个停止条件 
	return res;		
}

uint8_t myiic_write_bytes(uint8_t dev, uint8_t reg, uint8_t len, uint8_t *buffer)
{
	uint8_t i;
	myiic_start(); 
	myiic_send_Byte(dev);		//发送器件地址+写命令	
	if(myiic_wait_ack())		//等待应答
	{
		myiic_stop();		 
		return 1;		
	}
    myiic_send_Byte(reg);		//写寄存器地址
    myiic_wait_ack();			//等待应答 
	for(i=0; i<len; i++){
		myiic_send_Byte(buffer[i]);		//发送数据
		if(myiic_wait_ack())			//等待ACK
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
	myiic_send_Byte(dev);	//发送器件地址+写命令	
	myiic_wait_ack();		//等待应答 
    myiic_send_Byte(reg);	//写寄存器地址
    myiic_wait_ack();		//等待应答
    myiic_start();
	myiic_send_Byte(dev+1); //发送器件地址+读命令	
    myiic_wait_ack();		//等待应答 
	for(i=0; i<len; i++){
		if(i == len-1){
			buffer[i] = myiic_read_byte(0);//读取数据,发送nACK 
		}
		else{
			buffer[i] = myiic_read_byte(1);//读取数据,发送ACK 
		}
	}
    myiic_stop();			//产生一个停止条件 
	return 0;	
}

