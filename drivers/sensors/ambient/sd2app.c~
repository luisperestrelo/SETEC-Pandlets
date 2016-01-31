#include "sd2app.h"

#if SD_ENABLED


uint32_t sd_init(ble_ambient_t *m_amb_init){
	sd2app_printf("sd2app_init() \r\n");
	//flag_lido=0;
	m_sd2app.timer_count           = 0;
	m_sd2app.m_amb                 = m_amb_init;
	m_sd2app.IS_SD_ENABLED         = false;

	return NRF_SUCCESS;
}

uint32_t sd2app_configs_update(){
	sd2app_printf("\r\nExternal Temperature Configurations Update\r\n");

	uint8_t configuration = (m_sd2app.m_amb)->sd_configuration;

	//Reset count
	m_sd2app.timer_count  = 0;

	//Sensors enable configs
	m_sd2app.IS_SD_ENABLED = (configuration & AMB_ENABLE_BIT);

	//Rate configs
	switch(((configuration & AMB_RATE_BITS) >> 5)){
		case 0b000:
			m_sd2app.ticks = msec_to_ticks(10000000); //0.0001 Hz
			break;
		case 0b001:
			m_sd2app.ticks = msec_to_ticks(1000000);  //0.001 Hz
			break;
		case 0b010:
			m_sd2app.ticks = msec_to_ticks(100000);   //0.01 Hz
			break;
		case 0b011:
			m_sd2app.ticks = msec_to_ticks(10000);    //0.1 Hz
			break;
		case 0b111:
		default: //If not recognized set max rate
			m_sd2app.ticks = msec_to_ticks(2000);     //0.5 Hz
			break;
	}

	sd2app_printf("sd2app_configs_update() Ok!\r\n\r\n");
	return NRF_SUCCESS;
}


/*
uint32_t sd2app_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	//int32_t sd2app_buffer;

	//err_code = bme280_read_temperature(&sd2app_buffer);

	//if (err_code != NRF_SUCCESS) {
		//sd2app_printf("sd2app: bme280_read_temperature() failed.\r\n");
		//return err_code;
	//}
	
	//char buf[12];
    //sprintf(buf, "%d,%d,%d,%d,\n", DEVICE_ID,SENSOR_SD_ID,(int)sd2app_buffer,000);
    //log2sd(buf, "TEMP.txt");
    
	sd2app_printf("Leu cartao\n");
    
	//err_code = ble_ambient_sensor_update(m_sd2app.m_amb, (uint8_t *) &sd2app_buffer, AMB_SD_MAX_PACKET_VALUE, BLE_AMBIENT_SD);
	//check_ble_service_err_code(err_code);

	return NRF_SUCCESS;
}
int lerCartao2(){
	FIL file;       // File object
    char buf[4];
	unsigned int bytesread;
    if(sd_card.fs_type == 0) { //SD card not mounted
		//Mount the SD card
		if(f_mount(&sd_card, "", 1) == 0){
			printf("Mounted SD card!\n");
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
				ble_ambient_sensor_update(&m_amb, (uint8_t *) buf, 4, BLE_AMBIENT_SD);
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
    else { //SD card already mounted, someone is logging to it!
		printf("Fui parar aqui\n");
		//log_to_sd(filename, buffer, strlen(buffer));
    }
}

uint32_t sd2app_values_handler() {
	uint32_t  err_code = NRF_SUCCESS;
	//int32_t sd2app_buffer;

	//err_code = bme280_read_temperature(&sd2app_buffer);

	//if (err_code != NRF_SUCCESS) {
		//sd2app_printf("sd2app: bme280_read_temperature() failed.\r\n");
		//return err_code;
	//}
	
	//char buf[12];
    //sprintf(buf, "%d,%d,%d,%d,\n", DEVICE_ID,SENSOR_SD_ID,(int)sd2app_buffer,000);
    //log2sd(buf, "TEMP.txt");
  
	sd2app_printf("Leu cartao\n");
    
	//err_code = ble_ambient_sensor_update(m_sd2app.m_amb, (uint8_t *) &sd2app_buffer, AMB_SD_MAX_PACKET_VALUE, BLE_AMBIENT_SD);
	//check_ble_service_err_code(err_code);

	return err_code;
}*/

uint32_t sd2app_timer_handler() {
	uint32_t err_code = NRF_SUCCESS;

	if (m_sd2app.IS_SD_ENABLED) {

		//Increment timer
		m_sd2app.timer_count++;

		//Clear timer if finished
		if(m_sd2app.timer_count >= m_sd2app.ticks)
			m_sd2app.timer_count = 0;

		//Start reading
		if ((m_sd2app.timer_count == 1) && !twi_busy) {

			//Get values, parse values and send them through BLE
			//err_code = sd2app_values_handler();

			if (err_code != NRF_SUCCESS) {
				sd2app_printf("sd2app_values_handler() failed.\r\n");
				return err_code;
			}
		}
		else if ((m_sd2app.timer_count == 1) && twi_busy) {
			m_sd2app.timer_count--;
		}
	}

	return NRF_SUCCESS;
}


uint32_t sd2app_reset_configs(){
	uint32_t   err_code = NRF_SUCCESS;

	//Set service to default configurations
	err_code = ble_ambient_config_update(m_sd2app.m_amb, SD_INITIAL_CONFIG, BLE_AMBIENT_SD);

	if(err_code != NRF_SUCCESS){
		sd2app_printf("ble_ambient_config_update() for SD failed.\r\n");
		return err_code;
	}

	err_code = sd2app_configs_update();

	if(err_code != NRF_SUCCESS){
		sd2app_printf("sd2app_configs_update() failed.\r\n");
		return err_code;
	}

	//Reset sensor values
	uint8_t sd2app_buffer[AMB_SD_MAX_PACKET_VALUE];
	for(uint8_t i = 0; i < AMB_SD_MAX_PACKET_VALUE; i++) sd2app_buffer[i] = INVALID_SENSOR_VALUE;

	check_ble_service_err_code(ble_ambient_sensor_update(m_sd2app.m_amb, sd2app_buffer, AMB_SD_MAX_PACKET_VALUE, BLE_AMBIENT_SD));

	return NRF_SUCCESS;
}
#endif
