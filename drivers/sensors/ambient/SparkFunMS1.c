#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "max1161x.h"
#include "SparkFunMS1.h"

int SparkFunMS1_read1(uint8_t *molhadinho){
	
	uint8_t moist[2];
	moist[0]=moist[1]=0;
	max1161x_read_channel(3, moist);
	printf(" Humidade[0] := %d   Humidade[1] := %d\n", moist[0], moist[1]);
	(*molhadinho) = moist [1];
	printf(" Humidade := %d\n", (*molhadinho));		
	nrf_delay_ms(5);
	
	if(*molhadinho<128)
		return 1;
	else 
		return 0;
}

bool SparkFunMS1_read3(uint16_t *molhadinho){
	
	uint8_t moist[2];
	moist[0]=moist[1]=0;
	max1161x_read_channel(2, moist);
	(*molhadinho) = (moist[1] << 4 )| (moist[0] & 0b00001111);		
	printf(" UV := %d\n", (*molhadinho));
	nrf_delay_ms(5);
return NRF_SUCCESS;
}

bool SparkFunMS1_read2(uint8_t *molhadinho){
	
	
	uint8_t moist[2];
	moist[0]=moist[1]=0;
	max1161x_read_channel(1, moist);
	printf(" Humidade[0] := %d   Humidade[1] := %d\n", moist[0], moist[1]);
	(*molhadinho) = moist [1];
	printf(" Humidade := %d\n", (*molhadinho));		
	nrf_delay_ms(5);
	
	if(*molhadinho<128)
		return 1;
	else 
		return 0;
}

