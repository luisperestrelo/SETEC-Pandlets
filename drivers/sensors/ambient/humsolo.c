/*
 * humsolo.c
 *
 *  Created on: Apr 17, 2015
 *      Author: João Oliveira
 */


#include "humsolo.h"

#if HUMSOLO_ENABLED == 1

uint32_t humsolo_init(ble_ambient_t *m_amb_init){
	humsolo_printf("humsolo_init() \r\n");

	m_humsolo.timer_count           = 0;
	m_humsolo.m_amb                 = m_amb_init;
	m_humsolo.IS_HUMSOLO_ENABLED    = true;

	return NRF_SUCCESS;
}


uint32_t humsolo_configs_update(){
	humsolo_printf("\r\nhumidity soil Configurations Update\r\n");

	uint8_t configuration = (m_humsolo.m_amb)->humsolo_configuration;

	//Reset count
	m_humsolo.timer_count  = 0;

	//Sensors enable configs
	m_humsolo.IS_HUMSOLO_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_humsolo.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_humsolo.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_humsolo.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_humsolo.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_humsolo.ticks = msec_to_ticks(READ_FREQ);     //0.5 Hz
			break;
	}

	humsolo_printf("humsolo_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}



uint32_t humsolo_values_handler() {
	//uint32_t  err_code = NRF_SUCCESS;
	uint16_t humsolo_buffer;
	
	SparkFunMS1_read3(&humsolo_buffer);
	
	char val[20];
	add_zeroes((int)humsolo_buffer, val); //HANSOLO
    char buf[20];
	sprintf(buf, ",%s", val);
	log2sd(buf, "READINGS.txt");

//	humsolo_printf("Humidity Soil: %d\r\n", (int)humsolo_buffer);

	//err_code = ble_ambient_sensor_update(m_humsolo.m_amb, (uint8_t *) &humsolo_buffer,
			//AMB_HUMSOLO_MAX_PACKET_VALUE, BLE_AMBIENT_HUMSOLO);
	//check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}


uint32_t humsolo_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_humsolo.IS_HUMSOLO_ENABLED) {

		//Increment timer
		m_humsolo.timer_count++;

		//Clear timer if finished
		if(m_humsolo.timer_count >= m_humsolo.ticks)
			m_humsolo.timer_count = 0;

		//Start reading
		if ((m_humsolo.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			err_code = humsolo_values_handler();

			if (err_code != NRF_SUCCESS) {
				humsolo_printf("humsolo_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_humsolo.timer_count == 1) && twi_busy) {
			m_humsolo.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t humsolo_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_humsolo.m_amb, HUMSOLO_INITIAL_CONFIG, BLE_AMBIENT_HUMSOLO);

	if(err_code != NRF_SUCCESS){
		humsolo_printf("ble_ambient_config_update() for HUMSOLO failed.\r\n");
		return err_code;
	}

	err_code = humsolo_configs_update();

	if(err_code != NRF_SUCCESS){
		humsolo_printf("humsolo_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t humsolo_buffer[AMB_HUMSOLO_MAX_PACKET_VALUE];
	for(uint8_t i = 0; i < AMB_HUMSOLO_MAX_PACKET_VALUE; i++) humsolo_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(
			ble_ambient_sensor_update(m_humsolo.m_amb, humsolo_buffer, AMB_HUMSOLO_MAX_PACKET_VALUE, BLE_AMBIENT_HUMSOLO));

	return NRF_SUCCESS;
}

#endif /* HUMSOLO_ENABLED */
