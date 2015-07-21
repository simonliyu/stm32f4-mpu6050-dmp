#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "stm32f4_discovery.h"
#include "dmp.h"

int main()
{
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	print_clock_freq();
	
	dmp_init();
	
	while(1){
		if(dmp_get_euler(&euler) == 0){
			printf("pitch=%6.3f, roll=%6.3f, yaw=%6.3f\r\n", euler.pitch, euler.roll, euler.yaw);
		}
		else{
			printf("dmp get euler angle error!\r\n");
		}
		delay_ms(100);
	}
}

