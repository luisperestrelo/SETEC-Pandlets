#ifndef UV_H_
#define UV_H_

#include "ambient_service_config.h"
#include "board_config.h"

#if UV_ENABLED == 1

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

#if UV_DEBUG
#define uv_printf RTT_PRINTF
#else
#define uv_printf RTT_NOP
#endif

#define SENSOR_UV_ID 7

typedef struct
{
	ble_ambient_t *m_amb;

	uint64_t ticks; //Ticks of base timer
	uint64_t timer_count;

	bool    IS_UV_ENABLED;
} uv_t;


uv_t m_uv;


uint32_t uv_init(ble_ambient_t *m_amb_init);

uint32_t uv_configs_update();

uint32_t uv_timer_handler();

uint32_t uv_values_handler();

uint32_t uv_reset_configs();

#endif /* UV_ENABLED */

#endif /* UV_H_ */
