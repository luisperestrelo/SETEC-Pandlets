#include "nosso.h"
#include <stdint.h>


static unsigned long timeStamp2 = 0;
static unsigned long GPS = 0;
static unsigned long flagBAT = 0;
static unsigned long flagACC = 0;


unsigned long getTimeStamp(){
	
	return timeStamp2;
}

unsigned long getflagBAT(){
	
	return flagBAT;
}
unsigned long getflagACC(){
	
	return flagACC;
}
unsigned long getGPS(){
	
	return GPS;
}

void setTimeStamp(unsigned long x){
	
	timeStamp2=x;
}
void setflagBAT(unsigned long x){
	
	flagBAT=x;
}
void setflagACC(unsigned long x){
	
	flagACC=x;
}
void setGPS(unsigned long x){
	
	GPS=x;
}


void increTimeStamp(){
	
	timeStamp2++;
	
}
