
#include "usart.h"
//
//
#define AnBT_MPU6050_INT			GPIO_Pin_3		//PB3
#define AnBT_MPU6050_INT_PORT	GPIOB
//
#define ANBT_MPU6050_INT_STATE   GPIO_ReadInputDataBit(AnBT_MPU6050_INT_PORT, AnBT_MPU6050_INT)
//
#define ANBT_I2C_SDA 		GPIO_Pin_11	 
#define ANBT_I2C_SCL 		GPIO_Pin_10	
#define ANBT_I2C_PORT   GPIOB
//
#define ANBT_I2C_SCL_0 		GPIO_ResetBits(ANBT_I2C_PORT, ANBT_I2C_SCL)
#define ANBT_I2C_SCL_1 		GPIO_SetBits(ANBT_I2C_PORT, ANBT_I2C_SCL)
#define ANBT_I2C_SDA_0 		GPIO_ResetBits(ANBT_I2C_PORT, ANBT_I2C_SDA)
#define ANBT_I2C_SDA_1   	GPIO_SetBits(ANBT_I2C_PORT, ANBT_I2C_SDA)
//
#define ANBT_I2C_SDA_STATE   	GPIO_ReadInputDataBit(ANBT_I2C_PORT, ANBT_I2C_SDA)
#define ANBT_I2C_DELAY 				ANBT_I2C_Delay(100000)
#define ANBT_I2C_NOP					ANBT_I2C_Delay(10) 
//
#define ANBT_I2C_READY		0x00
#define ANBT_I2C_BUS_BUSY	0x01	
#define ANBT_I2C_BUS_ERROR	0x02
//
#define ANBT_I2C_NACK	  0x00 
#define ANBT_I2C_ACK		0x01
//

u8 AnBT_DMP_I2C_Write(u8 anbt_dev_addr, u8 anbt_reg_addr, u8 anbt_i2c_len, u8 *anbt_i2c_data_buf);
u8 AnBT_DMP_I2C_Read(u8 anbt_dev_addr, u8 anbt_reg_addr, u8 anbt_i2c_len, u8 *anbt_i2c_data_buf);
void AnBT_DMP_Delay_us(u32 dly);
void AnBT_DMP_Delay_ms(u32 dly);


