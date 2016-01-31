#include "install.h"

#if INST_ENABLED


uint32_t inst_init(ble_ambient_t *m_amb_init){
	inst_printf("inst_init() \r\n");
	//flag_lido=0;
	m_inst.timer_count           = 0;
	m_inst.m_amb                 = m_amb_init;
	m_inst.IS_INST_ENABLED       = true;

	return NRF_SUCCESS;
}

uint32_t inst_configs_update(){
	inst_printf("\r\nExternal Temperature Configurations Update\r\n");

	uint8_t configuration = (m_inst.m_amb)->inst_configuration;

	//Reset count
	m_inst.timer_count  = 0;

	//Sensors enable configs
	m_inst.IS_INST_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_inst.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_inst.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_inst.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_inst.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_inst.ticks = msec_to_ticks(2000);     //0.5 Hz
			break;
	}

	inst_printf("inst_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}


/*
uint32_t inst_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	//int32_t inst_buffer;

	//err_code = bme280_read_temperature(&inst_buffer);

	//if (err_code != NRF_SUCCESS) {
		//inst_printf("inst: bme280_read_temperature() failed.\r\n");
		//return err_code;
	//}
	
	//char buf[12];
    //sprintf(buf, "%d,%d,%d,%d,\n", DEVICE_ID,SENSOR_INST_ID,(int)inst_buffer,000);
    //log2sd(buf, "TEMP.txt");
    
	inst_printf("Leu cartao\n");
    
	//err_code = ble_ambient_sensor_update(m_inst.m_amb, (uint8_t *) &inst_buffer, AMB_INST_MAX_PACKET_VALUE, BLE_AMBIENT_INST);
	//check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}
int lerCartao2(){
	FIL file;       // File object
    char buf[4];
	unsigned int bytesread;
    if(sd_card.fs_type == 0) { //INST card not mounted
		//Mount the INST card
		if(f_mount(&sd_card, "", 1) == 0){
			printf("Mounted INST card!\n");
			if(f_open(&file, "TEMP.TXT", FA_READ) != FR_OK){ //Could be that the file already exists
				printf("ERROR Opening!\n");
				return 0;
			}
			//ble_ambient_config_update(m_amb, (uint8_t) 0, BLE_AMBIENT_FLAG);
			while(1){
				f_read(&file,buf,4,&bytesread);
				if(bytesread==0) break;
				//int y=0;
				flag_lido=0;	
				//while(1){
					//printf("%c",buf[y]);
					//y++;
					//if(y>bytesread) break;
				//}
				printf("HEY1");
				ble_ambient_sensor_update(&m_amb, (uint8_t *) buf, 4, BLE_AMBIENT_INST);
				while(flag_lido==0){
					//printf("HEY2");
				}
			}
			//ble_ambient_config_update(m_amb, (uint8_t) 1, BLE_AMBIENT_FLAG);
			f_close(&file);
			f_mount(NULL, "", 1);
			printf("Acabei de fazer tudo!\n");
		}
    }
    else { //INST card already mounted, someone is logging to it!
		printf("Fui parar aqui\n");
		//log_to_sd(filename, buffer, strlen(buffer));
    }
}

uint32_t inst_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	//int32_t inst_buffer;

	//err_code = bme280_read_temperature(&inst_buffer);

	//if (err_code != NRF_SUCCESS) {
		//inst_printf("inst: bme280_read_temperature() failed.\r\n");
		//return err_code;
	//}
	
	//char buf[12];
    //sprintf(buf, "%d,%d,%d,%d,\n", DEVICE_ID,SENSOR_INST_ID,(int)inst_buffer,000);
    //log2sd(buf, "TEMP.txt");
  
	inst_printf("Leu cartao\n");
    
	//err_code = ble_ambient_sensor_update(m_inst.m_amb, (uint8_t *) &inst_buffer, AMB_INST_MAX_PACKET_VALUE, BLE_AMBIENT_INST);
	//check_ble_service_err_code(err_code);

	return err_code;
}*/

uint32_t inst_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_inst.IS_INST_ENABLED) {

		//Increment timer
		m_inst.timer_count++;

		//Clear timer if finished
		if(m_inst.timer_count >= m_inst.ticks)
			m_inst.timer_count = 0;

		//Start reading
		if ((m_inst.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			//err_code = inst_values_handler();

			if (err_code != NRF_SUCCESS) {
				inst_printf("inst_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_inst.timer_count == 1) && twi_busy) {
			m_inst.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t inst_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_inst.m_amb, INST_INITIAL_CONFIG, BLE_AMBIENT_INST);

	if(err_code != NRF_SUCCESS){
		inst_printf("ble_ambient_config_update() for INST failed.\r\n");
		return err_code;
	}

	err_code = inst_configs_update();

	if(err_code != NRF_SUCCESS){
		inst_printf("inst_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t inst_buffer[INST_RX_PACKET_VALUE];
	for(uint8_t i = 0; i < INST_RX_PACKET_VALUE; i++) inst_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(ble_ambient_sensor_update(m_inst.m_amb, inst_buffer, INST_RX_PACKET_VALUE, BLE_AMBIENT_INST));

	return NRF_SUCCESS;
}
#endif
