#ifndef AMBIENT_SERVICE_CONFIG_H__
#define AMBIENT_SERVICE_CONFIG_H__

#include "board_config.h"

#if AMBIENT_SERVICE_ENABLED == 1

/***************** TEMP *********************/
#define TEMP_INITIAL_CONFIG         0b11110001										  //Initial configuration of the TEMP sensor. Enabled, 0.5Hz.
#define TEMP_DEBUG                  1												  //Enables TEMP RTT debug


/***************** PR *********************/
#define PR_INITIAL_CONFIG           0b11110001										  //Initial configuration of the PR sensor. Enabled, 0.5Hz.
#define PR_DEBUG                    1												  //Enables PR RTT debug


/***************** HUM *********************/
#define HUM_INITIAL_CONFIG          0b11110001										  //Initial configuration of the HUM sensor. Enabled, 0.5Hz.
#define HUM_DEBUG                   1												  //Enables HUM RTT debug


/***************** HUMSOLO *********************/
#define HUMSOLO_INITIAL_CONFIG          0b11110000										  //Initial configuration of the HUMSOLO sensor. Disabled, 0.5Hz.
#define HUMSOLO_DEBUG                   1												  //Enables HUM RTT debug


/***************** LUM *********************/
#define LUM_INITIAL_CONFIG          0b11110000										  //Initial configuration of the LUM sensor. Disabled, 0.5Hz.
#define LUM_DEBUG                   1												  //Enables LUM RTT debug


/***************** SD *********************/
#define SD_INITIAL_CONFIG          0b11110000										  //Initial configuration of the SD. Disabled, 0.5Hz.
#define SD_DEBUG                   1												  //Enables SD RTT debug


/***************** INST *********************/
#define INST_INITIAL_CONFIG          0b11110000										  //Initial configuration of the Install. Disabled, 0.5Hz.
#define INST_DEBUG                   1												  //Enables INST RTT debug


/***************** ALERT *********************/
#define ALERT_INITIAL_CONFIG          0b11110000										  //Initial configuration of the Alert. Disabled, 0.5Hz.
#define ALERT_DEBUG                   1												  //Enables ALERT RTT debug


#endif /*AMBIENT_SERVICE_ENABLED*/

#endif /* AMBIENT_SERVICE_CONFIG_H__ */
