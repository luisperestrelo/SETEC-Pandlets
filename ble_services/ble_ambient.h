/** @file
 * 
 * @brief Ambient Service module.
 *
 * @details This module implements the Ambient Service with the X, Y and Z values characteristic.
 *          During initialization it adds the Ambient Service and the axis values characteristic
 *          to the BLE stack database. 
 *
 */

#ifndef BLE_AMBIENT_H__
#define BLE_AMBIENT_H__

#include "ambient_service_config.h"

#if AMBIENT_SERVICE_ENABLED == 1

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_types.h"
#include "nrf_delay.h"
#include "nordic_common.h"
#include "app_util.h"
#include "app_timer.h"
#include "utils.h"
#include "nosso.h"

app_timer_id_t				            m_rtc_timer_id;		        		    		// RTC timer for counting time
//app_timer_id_t				            m_sd_timer_id;		        		    		// SD timer for counting time

static const ble_uuid128_t AMBIENT_UUID_BASE = {{0xDD, 0xA3, 0x44, 0xA5, 0xFA, 0x22, 0xAD, 0x1A, 0x11, 0x21, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00}};

//static int flag_lido;

//Arbitrary values. Need to have 2 bytes and be sequential
#define AMBIENT_UUID_SERVICE                    0x1120  //Ambient Service UUID

#define AMBIENT_UUID_TEMP_CHAR                  0x1133  //Temperature Values
#define AMBIENT_UUID_TEMP_CONFIG_CHAR           0x1134  //T Configuration

#define AMBIENT_UUID_PR_CHAR                    0x1135  //Pressure Values
#define AMBIENT_UUID_PR_CONFIG_CHAR             0x1136  //PR Configuration

#define AMBIENT_UUID_HUM_CHAR                   0x1137  //Humidity Values
#define AMBIENT_UUID_HUM_CONFIG_CHAR            0x1138  //HUM Configuration

#define AMBIENT_UUID_LUM_CHAR                   0x1139  //Luminosity Values
#define AMBIENT_UUID_LUM_CONFIG_CHAR            0x1140  //LUM Configuration

#define AMBIENT_UUID_HUMSOLO_CHAR               0x1141  //Humidity solo Values
#define AMBIENT_UUID_HUMSOLO_CONFIG_CHAR        0x1142  //HUMSOLO Configuration

#define AMBIENT_UUID_SD_CHAR                   	0x1143  //SD Values
#define AMBIENT_UUID_SD_CONFIG_CHAR            	0x1144  //SD Configuration

#define AMBIENT_UUID_INST_CHAR                  0x1145  //Install Values
#define AMBIENT_UUID_INST_CONFIG_CHAR           0x1146  //Install Configuration

#define AMBIENT_UUID_ALERT_CHAR                 0x1147  //Alert Values
#define AMBIENT_UUID_ALERT_CONFIG_CHAR          0x1148  //Alert Configuration

#define AMBIENT_UUID_RAIN_CHAR                  0x1149  //Rain Values
#define AMBIENT_UUID_RAIN_CONFIG_CHAR           0x1150  //Rain Configuration

#define AMB_TEMP_MAX_PACKET_VALUE               0x04   //4 byte per packet
#define AMB_PR_MAX_PACKET_VALUE                 0x04   //4 byte per packet
#define AMB_HUM_MAX_PACKET_VALUE                0x04   //4 byte per packet   
#define AMB_HUMSOLO_MAX_PACKET_VALUE            0x04   //4 byte per packet
#define AMB_LUM_MAX_PACKET_VALUE                0x04   //4 byte per packet  
#define AMB_RAIN_MAX_PACKET_VALUE            	0x04   //4 byte per packet
#define AMB_SD_MAX_PACKET_VALUE                 0x14   //20 byte per packet
#define AMB_ALERT_MAX_PACKET_VALUE              0x01   //1 byte per packet
#define INST_RX_PACKET_VALUE               		0x0C   //12 byte per packet
#define INST_TX_PACKET_VALUE               		0x08   //8 byte per packet
#define INVALID_SENSOR_VALUE                    0xFF   //Default value for the sensor values
 
//Bit masks for the configuration packet.
#define AMB_ENABLE_BIT                0b00000001   
#define AMB_RATE_BITS                 0b11100000
#define AMB_SLEEP_BIT                 0b00010000

#define AMB_NUMBER_OF_SENSORS         10


/**@brief Ambient sensor type. */
typedef enum
{
    BLE_AMBIENT_TEMP,
    BLE_AMBIENT_PR,
    BLE_AMBIENT_HUM,
    BLE_AMBIENT_LUM,
    BLE_AMBIENT_HUMSOLO,
    BLE_AMBIENT_SD,
    BLE_AMBIENT_INST,
    BLE_AMBIENT_ALERT,
    BLE_AMBIENT_RAIN
    
} ble_ambient_sensor_type;

/**@brief Ambient Service event type. */
typedef enum
{
    BLE_AMBIENT_EVT_TEMP_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_PR_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_HUM_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_LUM_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_HUMSOLO_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_SD_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_INST_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_ALERT_CONFIG_CHANGED,
    BLE_AMBIENT_EVT_RAIN_CONFIG_CHANGED
} ble_ambient_evt_type_t;

/**@brief Maps all update types. Really useful for a compact update function.*/
typedef struct
{
    ble_ambient_sensor_type         type;
    ble_gatts_char_handles_t *      handle;                 
    ble_gatts_char_handles_t *      config_handle;
    uint8_t	*				        value;
} ble_ambient_sensor_to_handle;  


/**@brief Ambient Service event. */
typedef struct
{
    ble_ambient_evt_type_t evt_type;                                  
    ble_evt_t * p_ble_evt;
} ble_ambient_evt_t;


// Forward declaration of the ble_ambient_t type. 
typedef struct ble_ambient_s ble_ambient_t;


/**@brief Ambient Service event handler type. */
typedef void (*ble_ambient_evt_handler_t) (ble_ambient_t * p_amb, ble_ambient_evt_t * p_evt);


/**@brief Ambient Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_ambient_evt_handler_t     evt_handler;                                 // Event handler to be called for handling events in the Ambient Service.
    bool                          support_notification;                        // TRUE if notification of Ambient values is supported.

	#if TEMP_ENABLED
    uint8_t                       temp_init_configuration;                     // Sensor configuration value to init the struct.
	#endif

	#if PR_ENABLED
	uint8_t                       pr_init_configuration;                       // Sensor configuration value to init the struct.
	#endif

	#if HUM_ENABLED
    uint8_t                       hum_init_configuration;                      // Sensor configuration value to init the struct.
	#endif

	#if LUM_ENABLED
    uint8_t                       lum_init_configuration;                      // Sensor configuration value to init the struct.
	#endif

	#if HUMSOLO_ENABLED
    uint8_t                       humsolo_init_configuration;                      // Sensor configuration value to init the struct.
	#endif
	
	#if RAIN_ENABLED
    uint8_t                       rain_init_configuration;                      // Sensor configuration value to init the struct.
	#endif

	#if SD_ENABLED
    uint8_t                       sd_init_configuration;                      // Sensor configuration value to init the struct.
	#endif

	#if INST_ENABLED
    uint8_t                       inst_init_configuration;                      // Sensor configuration value to init the struct.
	#endif

	#if ALERT_ENABLED
    uint8_t                       alert_init_configuration;                      // Sensor configuration value to init the struct.
	#endif
    
} ble_ambient_init_t;


/**@brief Ambient Service structure. This contains various status information for the service. */
typedef struct ble_ambient_s
{
    ble_ambient_evt_handler_t     evt_handler;                    			   // Event handler to be called for handling events in the Ambient Service.
    uint16_t                      service_handle;                 			   // Handle of Ambient Service (as provided by the BLE stack).

    uint8_t                       uuid_type;					  			   // Save uuid type here (as provided by the BLE stack)..
    
    uint16_t                      conn_handle;                                 // Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).
    bool                          is_notification_supported;                   // TRUE if notifications are supported.

	#if TEMP_ENABLED == 1
    ble_gatts_char_handles_t      temp_handles;                                // Handles related to the Temperature value characteristic.
    ble_gatts_char_handles_t      temp_configuration_handles;                  // Handles related to the Temperature sensor configuration characteristic.

    uint8_t                       temp_value[AMB_TEMP_MAX_PACKET_VALUE];       // Temperature value placeholder.
    uint8_t                       temp_configuration;      					   // Temperature sensor configuration value placeholder.
	#endif

	#if PR_ENABLED == 1
    ble_gatts_char_handles_t      pr_handles;                                 // Handles related to the Pressure value characteristic.
    ble_gatts_char_handles_t      pr_configuration_handles;                   // Handles related to the Pressure sensor configuration characteristic.

    uint8_t                       pr_value[AMB_PR_MAX_PACKET_VALUE];           // Pressure value placeholder.
    uint8_t                       pr_configuration;      					   // Pressure sensor configuration value placeholder.
	#endif

	#if HUM_ENABLED == 1
	ble_gatts_char_handles_t      hum_handles;                                 // Handles related to the Humidity value characteristic.
	ble_gatts_char_handles_t      hum_configuration_handles;                   // Handles related to the Humidity sensor configuration characteristic.

	uint8_t                       hum_value[AMB_HUM_MAX_PACKET_VALUE];         // Humidity value placeholder.
	uint8_t                       hum_configuration;      					   // Humidity sensor configuration value placeholder.
	#endif

	#if HUMSOLO_ENABLED == 1
	ble_gatts_char_handles_t      humsolo_handles;                                 // Handles related to the Humidity value characteristic.
	ble_gatts_char_handles_t      humsolo_configuration_handles;                   // Handles related to the Humidity sensor configuration characteristic.

	uint8_t                       humsolo_value[AMB_HUMSOLO_MAX_PACKET_VALUE];         // Humidity value placeholder.
	uint8_t                       humsolo_configuration;      					   // Humidity sensor configuration value placeholder.
	#endif


	#if LUM_ENABLED == 1
	ble_gatts_char_handles_t      lum_handles;                                 // Handles related to the Humidity value characteristic.
	ble_gatts_char_handles_t      lum_configuration_handles;                   // Handles related to the Humidity sensor configuration characteristic.

	uint8_t                       lum_value[AMB_LUM_MAX_PACKET_VALUE];         // Humidity value placeholder.
	uint8_t                       lum_configuration;      					   // Humidity sensor configuration value placeholder.
	#endif
	
	#if RAIN_ENABLED == 1
	ble_gatts_char_handles_t      rain_handles;                                 // Handles related to the Rain value characteristic.
	ble_gatts_char_handles_t      rain_configuration_handles;                   // Handles related to the Rain sensor configuration characteristic.

	uint8_t                       rain_value[AMB_RAIN_MAX_PACKET_VALUE];         // Rain value placeholder.
	uint8_t                       rain_configuration;      					   // Rain sensor configuration value placeholder.
	#endif

	#if SD_ENABLED == 1
	ble_gatts_char_handles_t      sd_handles;                                 // Handles related to the SD value characteristic.
	ble_gatts_char_handles_t      sd_configuration_handles;                   // Handles related to the SD configuration characteristic.

	uint8_t                       sd_value[AMB_SD_MAX_PACKET_VALUE];         // SD value placeholder.
	uint8_t                       sd_configuration;      					   // SD sensor configuration value placeholder.
	#endif

	#if INST_ENABLED == 1
	ble_gatts_char_handles_t      inst_handles;                                 // Handles related to the Install value characteristic.
	ble_gatts_char_handles_t      inst_configuration_handles;                   // Handles related to the Install configuration characteristic.

	uint8_t                       inst_value[INST_RX_PACKET_VALUE];         	// Install value placeholder.
	uint8_t                       inst_configuration;      					   // Install sensor configuration value placeholder.
	#endif

	#if ALERT_ENABLED == 1
	ble_gatts_char_handles_t      alert_handles;                                 // Handles related to the Alert value characteristic.
	ble_gatts_char_handles_t      alert_configuration_handles;                   // Handles related to the Alert configuration characteristic.

	uint8_t                       alert_value[AMB_ALERT_MAX_PACKET_VALUE];         // Alert value placeholder.
	uint8_t                       alert_configuration;      					   // Alert sensor configuration value placeholder.
	#endif
    
} ble_ambient_t;


/**@brief Function for initializing the Ambient Service.
 *
 * @param[out]  p_amb       Ambient Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_amb_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_ambient_init(ble_ambient_t * p_amb, const ble_ambient_init_t * p_amb_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Ambient Service.
 *
 *
 * @param[in]   p_amb      Ambient Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_ambient_on_ble_evt(ble_ambient_t * p_amb, ble_evt_t * p_ble_evt);


/**@brief Function for updating the Ambient service values.
 *
 * @param[in]   p_amb        Ambient Service structure.
 * @param[in]   axis_value   New value.
 * @param[in]   type         Type of the value.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 * @return  	BLE_ERROR_GATTS_SYS_ATTR_MISSING if notifications disabled
 */
uint32_t ble_ambient_sensor_update(ble_ambient_t * p_amb, uint8_t * values, 
					uint8_t number_of_bytes, ble_ambient_sensor_type type);


/**@brief Function for updating the Ambient config values.
 * 
 * @param[in]   p_amb                  Ambient Service structure.
 * @param[in]   sensor_configuration   New sensor configuration.
 * @param[in]   type                   Sensor type.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_config_update(ble_ambient_t * p_amb, uint8_t sensor_configuration, 
														 ble_ambient_sensor_type type);


void rtc_timer_handler(void * p_context);

//void sd_timer_handler(void * p_context);
void sd_handler();

/**@brief Function for starting the application timers.
 */
void application_work_start(void *data, uint16_t size);


/**@brief Function for updating the Install config values.
 * 
 * @param[in]   p_amb                  Ambient Service structure.
 * @param[in]   sensor_configuration   New configuration.
 * @param[in]   type                   Type.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_install_config_update(ble_ambient_t * p_amb, uint8_t * sensor_configuration, 
														 ble_ambient_sensor_type type);


/**@brief Function for updating status code value.
 * 
 * @param[in]   p_amb   Ambient Service structure.
 * @param[in]   code    New status code.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_status_code_update(ble_ambient_t * p_amb, uint32_t code);  


/**@brief Function for updating the Ambient Service sensors calibration stage.
 *
 * @param[in]   p_amb                  Ambient Service Service structure.
 * @param[in]   sensor_calibration     Sensor calibration stage in byte 0 and value in byte 1.
 * @param[in]   type                   Sensor type (only PH and EC allow calibration yet).
 * @param[in]   length                 Number of bytes in buffer.
 * @param[in]   client                 If true, was sent by smartphone, else sent by the firmware (needs to notify the smartphone).
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ambient_calib_update(ble_ambient_t * p_amb, uint8_t * sensor_calibration, uint16_t length, ble_ambient_sensor_type type, bool client);
#endif
#endif /* BLE_AMBIENT_H__ */
