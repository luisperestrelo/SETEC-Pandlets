#include "ble_ambient.h"
#include "nosso.h"

//static int flag_lido;

FIL file_sens;       // File object
ble_ambient_t * m_amb_sd;
int bytes_sent = 0;

#if AMBIENT_SERVICE_ENABLED


static ble_ambient_sensor_to_handle type_to_handle[AMB_NUMBER_OF_SENSORS];

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_amb       Ambient Service Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_ambient_t * p_amb, ble_evt_t * p_ble_evt){
    p_amb->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_amb       Ambient Service Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_ambient_t * p_amb, ble_evt_t * p_ble_evt){
    UNUSED_PARAMETER(p_ble_evt);
    p_amb->conn_handle = BLE_CONN_HANDLE_INVALID;
    if(getflag_sd()==1){
		bytes_sent = 0;
		setflag_sd(0);
		f_close(&file_sens);
		f_mount(NULL, "", 1);
	}
}


/**@brief Function for handling the Write event.
 *
 * @param[in]   p_amb       Ambient Service Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_ambient_t * p_amb, ble_evt_t * p_ble_evt){
#if TEMP_ENABLED || PR_ENABLED || HUM_ENABLED || LUM_ENABLED || SD_ENABLED || INST_ENABLED || ALERT_ENABLED || HUMSOLO_ENABLED || RAIN_ENABLED
	ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
#endif
	
    //***************** TEMP ***********************/
	#if TEMP_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->temp_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) { 
				
		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;
			
			amb_evt.evt_type = BLE_AMBIENT_EVT_TEMP_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;
			
			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_TEMP);
			
			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif

    //***************** PR ***********************/
	#if PR_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->pr_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_PR_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_PR);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif

	//***************** HUM ***********************/
	#if HUM_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->hum_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_HUM_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_HUM);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif

	//***************** HUMSOLO ***********************/
	#if HUMSOLO_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->humsolo_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_HUMSOLO_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_HUMSOLO);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif
		
    //***************** LUM ***********************/
	#if LUM_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->lum_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_LUM_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_LUM);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif
	
	//***************** RAIN ***********************/
	#if RAIN_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->rain_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_RAIN_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_RAIN);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif

    //***************** SD ***********************/
	#if SD_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->sd_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_SD_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_SD);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif
		
    //***************** INST ***********************/
	#if INST_ENABLED
	//Sensor configuration written and with right size
	//printf("p_evt_write->len: %d\n", (int)p_evt_write->len);
	//printf("INST_RX_PACKET_VALUE: %d\n", (int)INST_RX_PACKET_VALUE);
	//int i=0;
	//while (i<p_evt_write->len) {
		//printf("Valor %d: %d\n", i+1, (int)p_evt_write->data[i]);
		//i++;
	//}
	if((p_evt_write->handle == p_amb->inst_configuration_handles.value_handle) &&
			(p_evt_write->len == INST_RX_PACKET_VALUE)) {
		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_INST_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_install_config_update(p_amb, p_evt_write->data, BLE_AMBIENT_INST);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif
		
    //***************** ALERT ***********************/
	#if ALERT_ENABLED
	//Sensor configuration written and with right size
	if((p_evt_write->handle == p_amb->alert_configuration_handles.value_handle) &&
			(p_evt_write->len == 1)) {

		if (p_amb->evt_handler != NULL){
			ble_ambient_evt_t amb_evt;

			amb_evt.evt_type = BLE_AMBIENT_EVT_ALERT_CONFIG_CHANGED;
			amb_evt.p_ble_evt = p_ble_evt;

			//Save new configuration value
			ble_ambient_config_update(p_amb, p_evt_write->data[0], BLE_AMBIENT_ALERT);

			//Signal the app that the configuration has changed
			p_amb->evt_handler(p_amb, &amb_evt);
		}
	}
	#endif
}


/**@brief Function for handling the BLE stack events. Called in main() in 
 * ble_evt_dispatch().
 *
 * @param[in]   p_amb       Ambient Service Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_ambient_on_ble_evt(ble_ambient_t * p_amb, ble_evt_t * p_ble_evt){
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_amb, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_amb, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_amb, p_ble_evt);
            break;
            
        case BLE_EVT_TX_COMPLETE:
            sd_handler();
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding the Ambient Service values characteristic.
 *
 * @param[in]   p_amb        Ambient Service Service structure.
 * @param[in]   p_amb_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t sensors_char_add(ble_ambient_t * p_amb, const ble_ambient_init_t * p_amb_init){
	uint32_t   err_code = NRF_SUCCESS;
	
    ble_gatts_attr_md_t cccd_md;

    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char;
    
    ble_gatts_attr_md_t attr_md_no_write;
    ble_gatts_attr_md_t attr_md_read_write;

#if TEMP_ENABLED || PR_ENABLED || HUM_ENABLED || LUM_ENABLED || HUMSOLO_ENABLED || RAIN_ENABLED || UV_ENABLED || SD_ENABLED || INST_ENABLED || ALERT_ENABLED
    ble_uuid_t          ble_char_uuid;
#endif

    // Add Ambient Service values characteristics
    if (p_amb->is_notification_supported)
    {
        memset(&cccd_md, 0, sizeof(cccd_md));

        // According to BAS_SPEC_V10, the read operation on cccd should be possible without
        // authentication. Always set cccd to OPEN!
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
        BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
        cccd_md.vloc       = BLE_GATTS_VLOC_STACK;
    }

    memset(&char_md, 0, sizeof(char_md));
	
	//Set characteristic metadata, use same for similar characteristics
    char_md.char_props.read   = 1;
    char_md.char_props.notify = (p_amb->is_notification_supported) ? 1 : 0;
    char_md.char_props.write  = 0;
    char_md.char_props.write_wo_resp = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = (p_amb->is_notification_supported) ? &cccd_md : NULL;
    char_md.p_sccd_md         = NULL;
	char_md.char_ext_props.wr_aux = 0;

	
	//Set atribute metadata for sensor values
    memset(&attr_md_no_write, 0, sizeof(attr_md_no_write));
	
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md_no_write.read_perm); //can read
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md_no_write.write_perm); //can't write
    attr_md_no_write.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md_no_write.rd_auth    = 0;
    attr_md_no_write.wr_auth    = 0;
    attr_md_no_write.vlen       = 1;
	
	//Set atribute metadata for configuration 
	memset(&attr_md_read_write, 0, sizeof(attr_md_read_write));
	
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md_read_write.read_perm); //can read
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md_read_write.write_perm); //can write
    attr_md_read_write.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md_read_write.rd_auth    = 0;
    attr_md_read_write.wr_auth    = 0;
    attr_md_read_write.vlen       = 0;	
	
    memset(&attr_char, 0, sizeof(attr_char));
	

    ///***************** TEMP ***********************/
	#if TEMP_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_TEMP_CHAR;
	
    attr_char.p_uuid    = &ble_char_uuid;
    attr_char.p_attr_md = &attr_md_no_write;
    attr_char.init_len  = AMB_TEMP_MAX_PACKET_VALUE;
    attr_char.init_offs = 0;
    attr_char.max_len   = AMB_TEMP_MAX_PACKET_VALUE;
    attr_char.p_value   = p_amb->temp_value;

    char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

    //Add temp characteristic 
    err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
                                    &attr_char, &p_amb->temp_handles);

    if(err_code != NRF_SUCCESS)
		return err_code;

    //Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_TEMP_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->temp_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->temp_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** PR ***********************/
	#if PR_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_PR_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_PR_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_PR_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->pr_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add pressure characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->pr_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_PR_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->pr_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->pr_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** HUM ***********************/
	#if HUM_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_HUM_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_HUM_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_HUM_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->hum_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add pressure characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->hum_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_HUM_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->hum_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->hum_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** HUMSOLO ***********************/
	#if HUMSOLO_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_HUMSOLO_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_HUMSOLO_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_HUMSOLO_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->humsolo_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add pressure characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->humsolo_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_HUMSOLO_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->humsolo_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->humsolo_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** LUM ***********************/
	#if LUM_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_LUM_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_LUM_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_LUM_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->lum_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add luminosity characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->lum_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_LUM_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->lum_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->lum_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** RAIN ***********************/
	#if RAIN_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_RAIN_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_RAIN_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_RAIN_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->rain_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add luminosity characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->rain_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_RAIN_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->rain_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add temp configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->rain_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif
	
	///***************** SD ***********************/
	#if SD_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_SD_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_SD_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_SD_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->sd_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add SD characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->sd_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_SD_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->sd_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add sd configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->sd_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** INST ***********************/
	#if INST_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_INST_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = INST_TX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = INST_TX_PACKET_VALUE;
	attr_char.p_value   = p_amb->inst_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add install characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->inst_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_INST_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = INST_RX_PACKET_VALUE*sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = INST_RX_PACKET_VALUE*sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->inst_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add sd configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->inst_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif

	///***************** ALERT ***********************/
	#if ALERT_ENABLED
	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_ALERT_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_no_write;
	attr_char.init_len  = AMB_ALERT_MAX_PACKET_VALUE;
	attr_char.init_offs = 0;
	attr_char.max_len   = AMB_ALERT_MAX_PACKET_VALUE;
	attr_char.p_value   = p_amb->alert_value;

	char_md.char_props.write  = 0;
	char_md.char_props.write_wo_resp = 0;

	//Add alert characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->alert_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;

	//Set atributes struct
	ble_char_uuid.type = p_amb->uuid_type;
	ble_char_uuid.uuid = AMBIENT_UUID_ALERT_CONFIG_CHAR;

	attr_char.p_uuid    = &ble_char_uuid;
	attr_char.p_attr_md = &attr_md_read_write;
	attr_char.init_len  = sizeof(uint8_t);
	attr_char.init_offs = 0;
	attr_char.max_len   = sizeof(uint8_t);
	attr_char.p_value   = &(p_amb->alert_configuration);

	char_md.char_props.write  = 1;
	char_md.char_props.write_wo_resp = 1;

	//Add sd configuration characteristic
	err_code = sd_ble_gatts_characteristic_add(p_amb->service_handle, &char_md,
									&attr_char, &p_amb->alert_configuration_handles);

	if(err_code != NRF_SUCCESS)
		return err_code;
	#endif
	
	
	//A map for handles, maybe a bit rudimentary...	
	type_to_handle[0].type = BLE_AMBIENT_TEMP;
	#if TEMP_ENABLED
	type_to_handle[0].handle = &(p_amb->temp_handles);
	type_to_handle[0].config_handle = &(p_amb->temp_configuration_handles);
	type_to_handle[0].value = (p_amb->temp_value);
	#endif
	
	type_to_handle[1].type = BLE_AMBIENT_PR;
	#if PR_ENABLED
	type_to_handle[1].handle = &(p_amb->pr_handles);
	type_to_handle[1].config_handle = &(p_amb->pr_configuration_handles);
	type_to_handle[1].value = (p_amb->pr_value);
	#endif

	type_to_handle[2].type = BLE_AMBIENT_HUM;
	#if HUM_ENABLED
	type_to_handle[2].handle = &(p_amb->hum_handles);
	type_to_handle[2].config_handle = &(p_amb->hum_configuration_handles);
	type_to_handle[2].value = (p_amb->hum_value);
	#endif	
	
	type_to_handle[3].type = BLE_AMBIENT_LUM;
	#if LUM_ENABLED
	type_to_handle[3].handle = &(p_amb->lum_handles);
	type_to_handle[3].config_handle = &(p_amb->lum_configuration_handles);
	type_to_handle[3].value = (p_amb->lum_value);
	#endif

	type_to_handle[4].type = BLE_AMBIENT_HUMSOLO;
	#if HUMSOLO_ENABLED
	type_to_handle[4].handle = &(p_amb->humsolo_handles);
	type_to_handle[4].config_handle = &(p_amb->humsolo_configuration_handles);
	type_to_handle[4].value = (p_amb->humsolo_value);
	#endif

	type_to_handle[5].type = BLE_AMBIENT_SD;
	#if SD_ENABLED
	type_to_handle[5].handle = &(p_amb->sd_handles);
	type_to_handle[5].config_handle = &(p_amb->sd_configuration_handles);
	type_to_handle[5].value = (p_amb->sd_value);
	#endif

	type_to_handle[6].type = BLE_AMBIENT_INST;
	#if INST_ENABLED
	type_to_handle[6].handle = &(p_amb->inst_handles);
	type_to_handle[6].config_handle = &(p_amb->inst_configuration_handles);
	type_to_handle[6].value = (p_amb->inst_value);
	#endif

	type_to_handle[7].type = BLE_AMBIENT_ALERT;
	#if ALERT_ENABLED
	type_to_handle[7].handle = &(p_amb->alert_handles);
	type_to_handle[7].config_handle = &(p_amb->alert_configuration_handles);
	type_to_handle[7].value = (p_amb->alert_value);
	#endif	
	
	type_to_handle[8].type = BLE_AMBIENT_RAIN;
	#if RAIN_ENABLED
	type_to_handle[8].handle = &(p_amb->rain_handles);
	type_to_handle[8].config_handle = &(p_amb->rain_configuration_handles);
	type_to_handle[8].value = (p_amb->rain_value);
	#endif	
	
	return err_code;	
}

/**@brief Function for adding the Ambient Service service.
 *
 * @param[in]   p_amb        Ambient Service Service structure.
 * @param[in]   p_amb_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_init(ble_ambient_t * p_amb, const ble_ambient_init_t * p_amb_init){
    uint32_t   err_code;
    ble_uuid_t ble_uuid;
    
	ble_uuid128_t base_uuid;
	
    // Initialize service structure
    p_amb->evt_handler                 = p_amb_init->evt_handler;
    p_amb->conn_handle                 = BLE_CONN_HANDLE_INVALID;
    p_amb->is_notification_supported   = p_amb_init->support_notification;
    
	#if TEMP_ENABLED
	for(uint8_t i = 0; i < AMB_TEMP_MAX_PACKET_VALUE; i++) p_amb->temp_value[i]          = INVALID_SENSOR_VALUE;
	p_amb->temp_configuration          = p_amb_init->temp_init_configuration;
	#endif

	#if PR_ENABLED
	for(uint8_t i = 0; i < AMB_PR_MAX_PACKET_VALUE; i++) p_amb->pr_value[i]              = INVALID_SENSOR_VALUE;
	p_amb->pr_configuration           = p_amb_init->pr_init_configuration;
	#endif

	#if HUM_ENABLED
	for(uint8_t i = 0; i < AMB_HUM_MAX_PACKET_VALUE; i++) p_amb->hum_value[i]            = INVALID_SENSOR_VALUE;
	p_amb->hum_configuration           = p_amb_init->hum_init_configuration;
	#endif

	#if HUMSOLO_ENABLED
	for(uint8_t i = 0; i < AMB_HUMSOLO_MAX_PACKET_VALUE; i++) p_amb->humsolo_value[i]    = INVALID_SENSOR_VALUE;
	p_amb->humsolo_configuration           = p_amb_init->humsolo_init_configuration;
	#endif

	#if LUM_ENABLED
	for(uint8_t i = 0; i < AMB_LUM_MAX_PACKET_VALUE; i++) p_amb->lum_value[i]            = INVALID_SENSOR_VALUE;
	p_amb->lum_configuration           = p_amb_init->lum_init_configuration;
	#endif	
	
	#if RAIN_ENABLED
	for(uint8_t i = 0; i < AMB_RAIN_MAX_PACKET_VALUE; i++) p_amb->rain_value[i]          = INVALID_SENSOR_VALUE;
	p_amb->rain_configuration           = p_amb_init->rain_init_configuration;
	#endif

	#if SD_ENABLED
	for(uint8_t i = 0; i < AMB_SD_MAX_PACKET_VALUE; i++) p_amb->sd_value[i]            = INVALID_SENSOR_VALUE;
	p_amb->sd_configuration           = p_amb_init->sd_init_configuration;
	#endif

	#if INST_ENABLED
	for(uint8_t i = 0; i < INST_RX_PACKET_VALUE; i++) p_amb->inst_value[i]            = INVALID_SENSOR_VALUE;
	p_amb->sd_configuration           = p_amb_init->inst_init_configuration;
	#endif

	#if ALERT_ENABLED
	for(uint8_t i = 0; i < AMB_ALERT_MAX_PACKET_VALUE; i++) p_amb->alert_value[i]            = INVALID_SENSOR_VALUE;
	p_amb->sd_configuration           = p_amb_init->alert_init_configuration;
	#endif
	
	
    // Add base UUID to softdevice's internal list. 
    base_uuid = AMBIENT_UUID_BASE;
	
	//Add the new UUID to the BLE stack
	err_code = sd_ble_uuid_vs_add(&base_uuid, &p_amb->uuid_type);
	if (err_code != NRF_SUCCESS){
		return err_code;
	}
	
	ble_uuid.type = p_amb->uuid_type;
    ble_uuid.uuid = AMBIENT_UUID_SERVICE;

	//Add new service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_amb->service_handle);
    if (err_code != NRF_SUCCESS){
        return err_code;
    }
	
    // Add service characteristics
    err_code = sensors_char_add(p_amb, p_amb_init);
    if (err_code != NRF_SUCCESS){
        return err_code;
    }
    
    return NRF_SUCCESS;
}


/**@brief Function for updating the Ambient Service service values.
 *
 * @param[in]   p_amb             Ambient Service Service structure.
 * @param[in]   values            Buffer with new values.
 * @param[in]   number_of_bytes   Size of the buffer.
 * @param[in]   type              Type of the value.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 * @return 		BLE_ERROR_GATTS_SYS_ATTR_MISSING if notifications disabled
 */
uint32_t ble_ambient_sensor_update(ble_ambient_t * p_amb, uint8_t * values, uint8_t number_of_bytes,
				ble_ambient_sensor_type type){
	
    uint32_t err_code = NRF_SUCCESS;
    
	int i, t;
	
	for(i = 0; i < AMB_NUMBER_OF_SENSORS; i++){
		if(type_to_handle[i].type == type){
			uint16_t len = (uint16_t)number_of_bytes;

			// Save new axis value
			for(t = 0; t < number_of_bytes; t++) type_to_handle[i].value[t] = values[t];

			// Update database
			err_code = sd_ble_gatts_value_set((*(type_to_handle[i].handle)).value_handle,
											  0,
											  &len,
											  values);
			if (err_code != NRF_SUCCESS)
				return err_code;

			// Send value if connected and notifying
			if ((p_amb->conn_handle != BLE_CONN_HANDLE_INVALID) && p_amb->is_notification_supported){
				ble_gatts_hvx_params_t hvx_params;

				memset(&hvx_params, 0, sizeof(hvx_params));

				hvx_params.handle = (*(type_to_handle[i].handle)).value_handle;
				hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
				hvx_params.offset = 0;
				hvx_params.p_len  = &len;
				hvx_params.p_data = values;

				err_code = sd_ble_gatts_hvx(p_amb->conn_handle, &hvx_params);
			}
			else
				err_code = NRF_ERROR_INVALID_STATE; //no-one is connected!

			return err_code;
		}	
	}
	
	//type doesn't exist
	err_code = NRF_ERROR_NOT_FOUND;
	return err_code;
}

/**@brief Handles the SD reload.
 */
/*void sd_timer_handler(void * p_context){
    
    char buf[AMB_SD_MAX_PACKET_VALUE];
	unsigned int bytesread;
	
	f_read(&file_sens, buf, AMB_SD_MAX_PACKET_VALUE, &bytesread);
	int x=0;
	printf("Data: ");
	while(bytesread>x){
			printf("%c",buf[x]);
			x++;
		}
	printf("\n");
	printf("Li %u bytes\n", bytesread);
	ble_ambient_sensor_update(m_amb_sd, (uint8_t *) buf, AMB_SD_MAX_PACKET_VALUE, BLE_AMBIENT_SD);
	if (bytesread < AMB_SD_MAX_PACKET_VALUE){
		app_timer_stop(m_sd_timer_id);
		f_close(&file_sens);
		f_mount(NULL, "", 1);
	}
}*/

void sd_handler() {
	
	if (getflag_sd() == 1) {
	
		if (bytes_sent >= f_size(&file_sens)) {
			printf("bytes_sent >= f_size(&file_sens): %d\n", bytes_sent);
			bytes_sent = 0;
			setflag_sd(0);
			f_close(&file_sens);
			f_mount(NULL, "", 1);
			return;
		}
		
		char buf[AMB_SD_MAX_PACKET_VALUE];
		unsigned int bytesread = 0;
		
		f_read(&file_sens, buf, AMB_SD_MAX_PACKET_VALUE, &bytesread);
		int x=0;
		printf("Data: ");
		while(bytesread>x){
				printf("%c",buf[x]);
				x++;
			}
		printf("\n");
		printf("Li %u bytes\n", bytesread);
		//while (1) {
			ble_ambient_sensor_update(m_amb_sd, (uint8_t *) buf, AMB_SD_MAX_PACKET_VALUE, BLE_AMBIENT_SD);/* == BLE_ERROR_NO_TX_BUFFERS)*/
				//break;
		//}
		
		bytes_sent += bytesread;
		printf("bytes_sent: %d\n", bytes_sent);
	}
}


int lerCartao2(ble_ambient_t * m_amb){
	
	//static DWORD pos=0;
	uint32_t  err_code = NRF_SUCCESS;
	
    if(sd_card.fs_type == 0) { //SD card not mounted
		//Mount the SD card
		if(f_mount(&sd_card, "", 1) == 0){
			printf("Mounted SD card!\n");
			if(f_open(&file_sens, "READINGS.txt", FA_READ) != FR_OK){ //Could be that the file already exists
				printf("ERROR Opening!\n");
				return 0;
			}
			m_amb_sd = m_amb;
			nrf_delay_ms(100);
			setflag_sd(1);
			sd_handler();
			//app_timer_start(m_sd_timer_id, APP_TIMER_TICKS(200, APP_TIMER_PRESCALER), NULL);
		} else {
			printf("Unable to mount\n");
		}
    }
    else { //SD card already mounted, someone is logging to it!
		printf("Fui parar aqui\n");
    }
    
	printf("Leu cartao\n");
	
	return err_code;
}

/**@brief Function for updating the Ambient Service config values.
 * 
 * @param[in]   p_amb                  Ambient Service Service structure.
 * @param[in]   sensor_configuration   New sensor configuration.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_config_update(ble_ambient_t * p_amb, uint8_t sensor_configuration, ble_ambient_sensor_type type){
	//new data!
#if TEMP_ENABLED || PR_ENABLED || HUM_ENABLED || LUM_ENABLED || HUMSOLO_ENABLED || RAIN_ENABLED || SD_ENABLED || INST_ENABLED || ALERT_ENABLED
	uint16_t len = 1;
#endif
	uint32_t err_code = NRF_SUCCESS;
	
	switch(type){
		#if TEMP_ENABLED
		case BLE_AMBIENT_TEMP:
		// Save new configuration value
		p_amb->temp_configuration = sensor_configuration;
		
		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->temp_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif
		
		#if PR_ENABLED
		case BLE_AMBIENT_PR:
		// Save new configuration value
		p_amb->pr_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->pr_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif

		#if HUM_ENABLED
		case BLE_AMBIENT_HUM:
		// Save new configuration value
		p_amb->hum_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->hum_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif

		#if HUMSOLO_ENABLED
		case BLE_AMBIENT_HUMSOLO:
		// Save new configuration value
		p_amb->humsolo_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->humsolo_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif


		#if LUM_ENABLED
		case BLE_AMBIENT_LUM:
		// Save new configuration value
		p_amb->lum_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->lum_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif
		
		#if RAIN_ENABLED
		case BLE_AMBIENT_RAIN:
		// Save new configuration value
		p_amb->rain_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->rain_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		break;
		#endif
		
		#if SD_ENABLED
		case BLE_AMBIENT_SD:
		// Save new configuration value
		p_amb->sd_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->sd_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		
		lerCartao2(p_amb);
		break;
		#endif
		
		#if ALERT_ENABLED
		case BLE_AMBIENT_ALERT:
		// Save new configuration value
		p_amb->alert_configuration = sensor_configuration;

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->alert_configuration_handles.value_handle,
										  0,
										  &len,
										  &sensor_configuration);
		
		uint8_t buf=0;
		//buf=(uint8_t) ((((uint8_t) getflagACC()) << 4) && ((uint8_t) getflagBAT()));
		buf=((uint8_t) getflagBAT());
	//	printf("buf antes: %d\n",buf);
		if((uint8_t) getflagACC()!=0) buf=buf+128;
	//	printf("buf: %d, bat: %d, acc: %d\n",buf,(int)getflagBAT(),(int)getflagACC());
		ble_ambient_sensor_update(p_amb, &buf, AMB_ALERT_MAX_PACKET_VALUE, BLE_AMBIENT_ALERT);
		setflagACC(0);
		
		break;
		#endif
		
		
		default:
		break;
	}
	
	return err_code;
}


/**@brief Handles the RTC reload.
 */
void rtc_timer_handler(void * p_context){
	increTimeStamp();
	//timeStamp2++; //Increment timestamp
	//printf("TIMESTAMPA NA CONA: %d\n", (int)getTimeStamp());
}


uint32_t ble_install_config_update(ble_ambient_t * p_amb, uint8_t * sensor_configuration, ble_ambient_sensor_type type){
	//new data!
#if INST_ENABLED
	uint16_t len = INST_TX_PACKET_VALUE;
#endif
	uint32_t err_code = NRF_SUCCESS;
	uint8_t gpsArray[8];
	uint8_t timeArray[4];
	//uint8_t configArray[4];
	unsigned long timestampArray;
	int i=0;
	
	switch(type){
		#if INST_ENABLED
		case BLE_AMBIENT_INST:
		// Save new configuration value
		p_amb->inst_configuration = sensor_configuration[0];
		
		while (i<INST_TX_PACKET_VALUE) {
			gpsArray[i] = sensor_configuration[i];
			i++;
		}
		while (i<INST_RX_PACKET_VALUE/*-4*/) {
			timeArray[i-INST_TX_PACKET_VALUE] = sensor_configuration[i];
			i++;
		}
		//while (i<INST_RX_PACKET_VALUE) {
			//configArray[i-INST_TX_PACKET_VALUE-4] = sensor_configuration[i];
			//i++;
		//}

		// Update database
		err_code = sd_ble_gatts_value_set(p_amb->inst_configuration_handles.value_handle,
										  0,
										  &len,
										  gpsArray);
		
		timestampArray = (timeArray[0] << 24) + (timeArray[1] << 16) + (timeArray[2] << 8) + timeArray[3];
		
		/*changeFreq(configArray[0],configArray[3]);*/
		
		deleteFile();
		
		
		printf("TIMESTAMP RECEBIDO: %d\n", (int)timestampArray);
		setTimeStamp(timestampArray);
		app_timer_stop(m_rtc_timer_id);
		app_timer_start(m_rtc_timer_id, APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER), NULL);
		
		app_sched_event_put(NULL, 0, application_work_start); //Start base timer.
		
		ble_ambient_sensor_update(p_amb, sensor_configuration, INST_TX_PACKET_VALUE, BLE_AMBIENT_INST);
		
		break;
		#endif
		
		
		default:
		break;
	}
	
	return err_code;
}


/**@brief Function for updating the Ambient Service sensors calibration stage.
 *
 * @param[in]   p_amb                  Ambient Service Service structure.
 * @param[in]   sensor_calibration     Sensor calibration stage in byte 0 and value in byte 1.
 * @param[in]   type                   Sensor type (only PH and EC allow calibration yet).
 * @param[in]   client                 If true, was sent by smartphone, else sent by the firmware (needs to notify the smartphone).
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_calib_update(ble_ambient_t * p_amb, uint8_t * sensor_calibration, uint16_t length,
		ble_ambient_sensor_type type, bool client){

	uint32_t err_code = NRF_SUCCESS;

	switch(type) {
		default:
			break;

	}

	return err_code;
}

#endif
