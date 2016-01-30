#ifndef ALERT_H__
#define ALERT_H__

#include "ambient_service_config.h"
#include "board_config.h"

#if ALERT_ENABLED == 1

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utilities includes */
#include "utils.h"

#include "bme280.h"              //BME280 device driver

/* Error codes includes */
#include "general_error_codes.h"

/* Service includes */
#include "ble_ambient.h"

#if ALERT_DEBUG
#define alert_printf RTT_PRINTF
#else
#define alert_printf RTT_NOP
#endif

#define SENSOR_ALERT_ID 1

typedef struct
{
	ble_ambient_t *m_amb;

	uint64_t ticks; //Ticks of base timer
	uint64_t timer_count;

	bool    IS_ALERT_ENABLED;
	bool    IS_READING;
} alert_t;


alert_t m_alert;

uint32_t alert_init(ble_ambient_t *m_amb_init);

uint32_t alert_configs_update();

uint32_t alert_timer_handler();

//uint32_t alert_values_handler();

uint32_t alert_reset_configs();

#endif /* ALERT_ENABLED */

#endif /* ALERT_H__ */
