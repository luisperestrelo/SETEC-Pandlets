#ifndef SD_H__
#define SD_H__

#include "ambient_service_config.h"
#include "board_config.h"

#if SD_ENABLED == 1

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

#if SD_DEBUG
#define sd2app_printf RTT_PRINTF
#else
#define sd2app_printf RTT_NOP
#endif

#define SENSOR_SD_ID 1

typedef struct
{
	ble_ambient_t *m_amb;

	uint64_t ticks; //Ticks of base timer
	uint64_t timer_count;

	bool    IS_SD_ENABLED;
	bool    IS_READING;
} sd2app_t;


sd2app_t m_sd2app;

uint32_t sd2app_init(ble_ambient_t *m_amb_init);

uint32_t sd2app_configs_update();

uint32_t sd2app_timer_handler();

uint32_t sd2app_values_handler();

uint32_t sd2app_reset_configs();

#endif /* SD_ENABLED */

#endif /* SD_H__ */
