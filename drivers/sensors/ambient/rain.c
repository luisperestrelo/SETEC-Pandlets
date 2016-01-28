/*
 * rain.c
 *
 *  Created on: Apr 17, 2015
 *      Author: João Oliveira
 */


#include "rain.h"

#if RAIN_ENABLED == 1

uint32_t rain_init(ble_ambient_t *m_amb_init){
	rain_printf("rain_init() \r\n");

	m_rain.timer_count           = 0;
	m_rain.m_amb                 = m_amb_init;
	m_rain.IS_RAIN_ENABLED    = false;

	return NRF_SUCCESS;
}


uint32_t rain_configs_update(){
	rain_printf("\r\nh Rain Configurations Update\r\n");

	uint8_t configuration = (m_rain.m_amb)->rain_configuration;

	//Reset count
	m_rain.timer_count  = 0;

	//Sensors enable configs
	m_rain.IS_RAIN_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_rain.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_rain.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_rain.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_rain.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_rain.ticks = msec_to_ticks(2000);     //0.5 Hz
			break;
	}

	rain_printf("rain_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}



uint32_t rain_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	uint16_t rain_buffer;
	//return NRF_SUCCESS;
	//SparkFunTSL2561_init();
	
	//err_code = SparkFunTSL2561_bring_the_light(&lum_buffer);
	err_code = SparkFunMS1_read(&rain_buffer);
	
	
	if (err_code != NRF_SUCCESS) {
		rain_printf("rain: Rain_read failed.\r\n");
		return err_code;
	}
	
    char buf[12];
    sprintf(buf, "%d,%d,%d,%d,\n", DEVICE_ID,SENSOR_RAIN_ID,(int)rain_buffer,000);
	log2sd(buf, "TEMP.txt");

	rain_printf("Rain: %d\r\n", (int)rain_buffer);

	err_code = ble_ambient_sensor_update(m_rain.m_amb, (uint8_t *) &rain_buffer,
			AMB_RAIN_MAX_PACKET_VALUE, BLE_AMBIENT_RAIN);
	check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}


uint32_t rain_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_rain.IS_RAIN_ENABLED) {

		//Increment timer
		m_rain.timer_count++;

		//Clear timer if finished
		if(m_rain.timer_count >= m_rain.ticks)
			m_rain.timer_count = 0;

		//Start reading
		if ((m_rain.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			err_code = rain_values_handler();

			if (err_code != NRF_SUCCESS) {
				rain_printf("rain_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_rain.timer_count == 1) && twi_busy) {
			m_rain.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t rain_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_rain.m_amb, RAIN_INITIAL_CONFIG, BLE_AMBIENT_RAIN);

	if(err_code != NRF_SUCCESS){
		rain_printf("ble_ambient_config_update() for RAIN failed.\r\n");
		return err_code;
	}

	err_code = rain_configs_update();

	if(err_code != NRF_SUCCESS){
		rain_printf("rain_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t rain_buffer[AMB_RAIN_MAX_PACKET_VALUE];
	for(uint8_t i = 0; i < AMB_RAIN_MAX_PACKET_VALUE; i++) rain_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(
			ble_ambient_sensor_update(m_rain.m_amb, rain_buffer, AMB_RAIN_MAX_PACKET_VALUE, BLE_AMBIENT_RAIN));

	return NRF_SUCCESS;
}

#endif /* RAIN_ENABLED */
