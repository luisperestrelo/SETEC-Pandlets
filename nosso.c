#include "nosso.h"
#include <stdint.h>


static unsigned long timeStamp2 = 0;
static unsigned long flagBAT = 0;
static unsigned long flagACC = 0;
static int flag_sd = 0;

unsigned long getTimeStamp(){
	
	return timeStamp2;
}

unsigned long getflagBAT(){
	
	return flagBAT;
}
unsigned long getflagACC(){
	
	return flagACC;
}

unsigned long getflag_sd(){
	
	return flag_sd;
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

void setflag_sd(unsigned long x){
	
	flag_sd=x;
}

void increTimeStamp(){
	
	timeStamp2++;
}
