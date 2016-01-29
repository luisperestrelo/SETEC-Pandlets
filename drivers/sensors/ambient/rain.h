#ifndef RAIN_H_
#define RAIN_H_

#include "ambient_service_config.h"
#include "board_config.h"

#if RAIN_ENABLED == 1

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "nosso.h"

/* Utilities includes */
#include "utils.h"

#include "bme280.h"              //BME280 device driver
#include <SparkFunMS1.h>
#include <SparkFunTSL2561.h>

/* Error codes includes */
#include "general_error_codes.h"

/* Service includes */
#include "ble_ambient.h"

#if RAIN_DEBUG
#define rain_printf RTT_PRINTF
#else
#define rain_printf RTT_NOP
#endif

#define SENSOR_RAIN_ID 6

typedef struct
{
	ble_ambient_t *m_amb;

	uint64_t ticks; //Ticks of base timer
	uint64_t timer_count;

	bool    IS_RAIN_ENABLED;
	bool    IS_READING;
} rain_t;


rain_t m_rain;

uint32_t rain_init(ble_ambient_t *m_amb_init);

uint32_t rain_configs_update();

uint32_t rain_timer_handler();

uint32_t rain_values_handler();

uint32_t rain_reset_configs();

#endif /* RAIN_ENABLED */

#endif /* RAIN_H_ */
