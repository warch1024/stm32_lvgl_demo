#include "tools.h"
#include "stm32f4xx.h"


void delay(uint32_t ms){
	int i,j;
	
	for(i=0;i<ms;i++)
		for(j=0;j<17500;j++);
}


