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

uint16_t aaverageAnalogRead(){

  uint8_t numberOfReadings = 8;
  uint8_t runningValue[2];
  uint16_t finalValue=0;

 runningValue[0]= runningValue[1] = 0;
  for(uint8_t x = 0 ; x < numberOfReadings ; x++){
     max1161x_read_channel(1, runningValue);
     
     finalValue += (runningValue[1] << 4 )| (runningValue[0] & 0b00001111);
   }

  finalValue /= numberOfReadings;
  return finalValue;
}

//The Arduino Map function but for floats
uint8_t amapfloat(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


uint8_t agetUV(uint16_t *uvLevel){

  printf("GETUV \n");
   *uvLevel = aaverageAnalogRead();
 // uint16_t refLevel = 33; // averageAnalogRead();

  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  //uint16_t outputVoltage = 33 / refLevel * (*uvLevel);
  //uint16_t uvIntensity = amapfloat(outputVoltage*330/128, 99, 280, 0, 1500); //Convert the voltage to a UV intensity level

  //printf("output: %d\n", (int) refLevel);
  //printf("ML8511 output: %d\n", (int) (*uvLevel));
  //printf("ML8511 voltage: %d\n", (int) outputVoltage);
  //printf("UV Intensity (mW/cm^2): %d\n", (int) uvIntensity);

  nrf_delay_ms(100);
  return NRF_SUCCESS;

}

bool SparkFunMS1_read2(uint16_t *molhadinho){
	
	
	
	//agetUV(molhadinho);
	
	
	
	
	uint8_t moist[2];
	moist[0]=moist[1]=0;
	max1161x_read_channel(1, moist);
	//(*molhadinho) = moist[1];
	(*molhadinho) = (moist[1] << 4 )| (moist[0] & 0b00001111);		
	printf(" UV := %d\n", (*molhadinho));
	nrf_delay_ms(5);
	return NRF_SUCCESS;	
	
	//uint8_t moist[2];
	//moist[0]=moist[1]=0;
	//max1161x_read_channel(1, moist);
	//printf(" Humidade[0] := %d   Humidade[1] := %d\n", moist[0], moist[1]);
	//(*molhadinho) = moist [1];
	////printf(" Humidade := %d\n", (*molhadinho));		
	//nrf_delay_ms(5);
	
	//if(*molhadinho<128)
		//return 1;
	//else 
		//return 0;
}
