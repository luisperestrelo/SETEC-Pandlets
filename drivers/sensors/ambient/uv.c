/*
 * uv.c
 *
 *  Created on: Apr 17, 2015
 *      Author: João Oliveira
 */


#include "uv.h"

#if UV_ENABLED == 1

uint32_t uv_init(ble_ambient_t *m_amb_init){
	uv_printf("uv_init() \r\n");

	m_uv.timer_count           = 0;
	m_uv.m_amb                 = m_amb_init;
	m_uv.IS_UV_ENABLED    = false;

	return NRF_SUCCESS;
}


uint32_t uv_configs_update(){
	uv_printf("\r\nh UV Configurations Update\r\n");

	uint8_t configuration = (m_uv.m_amb)->uv_configuration;

	//Reset count
	m_uv.timer_count  = 0;

	//Sensors enable configs
	m_uv.IS_UV_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_uv.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_uv.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_uv.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_uv.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_uv.ticks = msec_to_ticks(READ_FREQ);     //0.5 Hz
			break;
	}

	uv_printf("uv_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}



uint32_t uv_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	uint16_t uv_buffer;
	//return NRF_SUCCESS;
	//SparkFunTSL2561_init();
	
	//err_code = SparkFunTSL2561_bring_the_light(&lum_buffer);
	err_code = SparkFunMS1_read(&uv_buffer);
	
	
	if (err_code != NRF_SUCCESS) {
		uv_printf("uv: UV_read failed.\r\n");
		return err_code;
	}
	
    char buf[12];
    sprintf(buf, ",%d", (int)uv_buffer);
	log2sd(buf, "READINGS.txt");

	uv_printf("UV: %d\r\n", (int)uv_buffer);

	err_code = ble_ambient_sensor_update(m_uv.m_amb, (uint8_t *) &uv_buffer,
			AMB_UV_MAX_PACKET_VALUE, BLE_AMBIENT_UV);
	check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}


uint32_t uv_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_uv.IS_UV_ENABLED) {

		//Increment timer
		m_uv.timer_count++;

		//Clear timer if finished
		if(m_uv.timer_count >= m_uv.ticks)
			m_uv.timer_count = 0;

		//Start reading
		if ((m_uv.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			err_code = uv_values_handler();

			if (err_code != NRF_SUCCESS) {
				uv_printf("uv_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_uv.timer_count == 1) && twi_busy) {
			m_uv.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t uv_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_uv.m_amb, UV_INITIAL_CONFIG, BLE_AMBIENT_UV);

	if(err_code != NRF_SUCCESS){
		uv_printf("ble_ambient_config_update() for UV failed.\r\n");
		return err_code;
	}

	err_code = uv_configs_update();

	if(err_code != NRF_SUCCESS){
		uv_printf("uv_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t uv_buffer[AMB_UV_MAX_PACKET_VALUE];
	for(uint8_t i = 0; i < AMB_UV_MAX_PACKET_VALUE; i++) uv_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(
			ble_ambient_sensor_update(m_uv.m_amb, uv_buffer, AMB_UV_MAX_PACKET_VALUE, BLE_AMBIENT_UV));

	return NRF_SUCCESS;
}

#endif /* UV_ENABLED */
