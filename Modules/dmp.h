#ifndef __DMP_H__
#define __DMP_H__

#include "anbt_dmp_driver.h"
#include "anbt_dmp_mpu6050.h"

#define q30  1073741824.0f

typedef struct{
	float pitch;
	float roll;
	float yaw;
}Euler;

extern Euler euler;

void dmp_init(void);
uint8_t dmp_get_euler(Euler *e);

#endif
