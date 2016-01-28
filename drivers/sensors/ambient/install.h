#ifndef INST_H__
#define INST_H__

#include "ambient_service_config.h"
#include "board_config.h"

#if INST_ENABLED == 1

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

#if INST_DEBUG
#define inst_printf RTT_PRINTF
#else
#define inst_printf RTT_NOP
#endif

#define SENSOR_INST_ID 6

typedef struct
{
	ble_ambient_t *m_amb;

	uint64_t ticks; //Ticks of base timer
	uint64_t timer_count;

	bool    IS_INST_ENABLED;
	bool    IS_READING;
} inst_t;


inst_t m_inst;

uint32_t inst_init(ble_ambient_t *m_amb_init);

uint32_t inst_configs_update();

uint32_t inst_timer_handler();

//uint32_t inst_values_handler();

uint32_t inst_reset_configs();

#endif /* INST_ENABLED */

#endif /* INST_H__ */
