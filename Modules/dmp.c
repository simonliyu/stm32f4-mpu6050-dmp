#include "dmp.h"
#include "delay.h"

Euler euler;

void dmp_init(void)
{
	AnBT_DMP_MPU6050_Init();
}

uint8_t dmp_get_euler(Euler *e)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	uint8_t more;
	long quat[4];

	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	

	if (sensors & INV_WXYZ_QUAT )
	{
		q0=quat[0] / q30;
		q1=quat[1] / q30;
		q2=quat[2] / q30;
		q3=quat[3] / q30;
		e->pitch = asin(-2*q1*q3+2*q0*q2)*57.3f; 							// pitch
		e->roll = atan2(2*q2*q3+2*q0*q1,-2*q1*q1-2*q2*q2+1)*57.3f; 			// roll
		e->yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;//感觉没有价值，注掉
		return 0;
	}
	else{
		return 1;
	}
}


