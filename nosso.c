#include "nosso.h"
#include <stdint.h>


static unsigned long timeStamp2 = 0;

unsigned long getTimeStamp(){
	
	return timeStamp2;
}

void setTimeStamp(unsigned long x){
	
	timeStamp2=x;
}

void increTimeStamp(){
	
	timeStamp2++;
}
