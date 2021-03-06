/*
 * lum.c
 *
 *  Created on: Apr 17, 2015
 *      Author: João Oliveira
 */


#include "lum.h"

#if LUM_ENABLED == 1

uint32_t lum_init(ble_ambient_t *m_amb_init){
	lum_printf("lum_init() \r\n");

	m_lum.timer_count           = 0;
	m_lum.m_amb                 = m_amb_init;
	m_lum.IS_LUM_ENABLED        = true;
	
	return SparkFunTSL2561_init();

	//return NRF_SUCCESS;
}


uint32_t lum_configs_update(){
	lum_printf("\r\nLuminosity Configurations Update\r\n");

	uint8_t configuration = (m_lum.m_amb)->lum_configuration;

	//Reset count
	m_lum.timer_count  = 0;

	//Sensors enable configs
	m_lum.IS_LUM_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_lum.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_lum.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_lum.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_lum.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_lum.ticks = msec_to_ticks(READ_FREQ);     //0.5 Hz
			break;
	}

	lum_printf("lum_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}



uint32_t lum_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	uint32_t lum_buffer;
	//return NRF_SUCCESS;
	//SparkFunTSL2561_init();
	
	err_code = SparkFunTSL2561_bring_the_light(&lum_buffer);
	//err_code = SparkFunMS1_read(&lum_buffer);
	
	
	if (err_code != NRF_SUCCESS) {
	//	lum_printf("lum: SparkFunTSL2561_bring_the_light failed.\r\n");
		return err_code;
	}
	
   	char val[20];
	add_zeroes((int)lum_buffer, val);
    char buf[20];
	sprintf(buf, ",%s", val);
	log2sd(buf, "READINGS.txt");

   // lum_printf("li\n");    
//	lum_printf("Luminosity: %d\r\n", (int)lum_buffer);

	err_code = ble_ambient_sensor_update(m_lum.m_amb, (uint8_t *) &lum_buffer,
			AMB_LUM_MAX_PACKET_VALUE, BLE_AMBIENT_LUM);
	check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}


uint32_t lum_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_lum.IS_LUM_ENABLED) {

		//Increment timer
		m_lum.timer_count++;

		//Clear timer if finished
		if(m_lum.timer_count >= m_lum.ticks)
			m_lum.timer_count = 0;

		//Start reading
		if ((m_lum.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			err_code = lum_values_handler();

			if (err_code != NRF_SUCCESS) {
				lum_printf("lum_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_lum.timer_count == 1) && twi_busy) {
			m_lum.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t lum_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_lum.m_amb, LUM_INITIAL_CONFIG, BLE_AMBIENT_LUM);

	if(err_code != NRF_SUCCESS){
		lum_printf("ble_ambient_config_update() for LUM failed.\r\n");
		return err_code;
	}

	err_code = lum_configs_update();

	if(err_code != NRF_SUCCESS){
		lum_printf("lum_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t lum_buffer[AMB_LUM_MAX_PACKET_VALUE];
	for(uint8_t i = 0; i < AMB_LUM_MAX_PACKET_VALUE; i++) lum_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(
			ble_ambient_sensor_update(m_lum.m_amb, lum_buffer, AMB_LUM_MAX_PACKET_VALUE, BLE_AMBIENT_LUM));

	return NRF_SUCCESS;
}

#endif /* LUM_ENABLED */
