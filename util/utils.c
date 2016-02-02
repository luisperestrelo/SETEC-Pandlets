#include "utils.h"

#define AMB_NUMBER_OF_SENSORS 10
#define BYTES_PER_LINE 8 * (AMB_NUMBER_OF_SENSORS-6) + 1 + 2 + 6 + 10
//#define BYTES_PER_LINE 53 //
//#define BYTES_START_OVERWRITE 2*7*24*BYTES_PER_LINE // 2 semanas, escritas de hora em hora, 55 bytes por linha.
#define BYTES_START_OVERWRITE 644 // For testing.


uint64_t msec_to_ticks(uint32_t msec){
	return msec/BASE_TIMER_FREQ;
}


/**@brief Function for checking ble_acl error codes.
 *
 * @param[in]   err_code   Error code containing information about what went wrong.
 */
void check_ble_service_err_code(uint32_t err_code){
	if(err_code == BLE_ERROR_NO_TX_BUFFERS)
		printf("BLE_ERROR_NO_TX_BUFFERS\r\n");

	if ((err_code != NRF_SUCCESS) &&  						  //value saved and notification sent
			(err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING) && //value saved and notification not enabled
				(err_code != BLE_ERROR_NO_TX_BUFFERS) &&      //still need to handle this...
					(err_code != NRF_ERROR_INVALID_STATE)){   //no-one is connected!
        APP_ERROR_HANDLER(err_code);
    }
}


void add_zeroes_twenty(int val, char buf[]){ 
	if (val < 10)
		sprintf(buf, "0000000000000000000%d", val);
	else if (val < 100)
		sprintf(buf, "000000000000000000%d", val);
	else if (val < 1000)
		sprintf(buf, "00000000000000000%d", val);
	else if (val < 10000)
		sprintf(buf, "0000000000000000%d", val);
	else if (val < 100000)
		sprintf(buf, "000000000000000%d", val);
	else if (val < 1000000)
		sprintf(buf, "00000000000000%d", val);
	else if (val < 10000000)
		sprintf(buf, "0000000000000%d", val);
	else if (val < 100000000)
		sprintf(buf, "000000000000%d", val);
	else if (val < 1000000000)
		sprintf(buf, "00000000000%d", val);
	else if (val < 10000000000)
		sprintf(buf, "0000000000%d", val);
	else if (val < 100000000000)
		sprintf(buf, "000000000%d", val);
	else
		sprintf(buf, "%d", val);
}

void add_zeroes(int val, char buf[]){ // vao ter todos 8
	if (val < 10)
		sprintf(buf, "0000000%d", val);
	else if (val < 100)
		sprintf(buf, "000000%d", val);
	else if (val < 1000)
		sprintf(buf, "00000%d", val);
	else if (val < 10000)
		sprintf(buf, "0000%d", val);
	else if (val < 100000)
		sprintf(buf, "000%d", val);
	else if (val < 1000000)
		sprintf(buf, "00%d", val);
	else if (val < 10000000)
		sprintf(buf, "0%d", val);
	else
		sprintf(buf, "%d", val);
}

#if SD_LOG
//Should only be used for error logging
//It will take around 40 ms to complete!
uint32_t log_to_sd(const char * filename, const char * data, uint32_t buffer_size){
	FIL file;       // File object
	static int file_position = 20;
	static int file_position_prev = 0;
	//uint32_t buffer_file_size = 20;
//	printf("POS: %d\n", file_position);
//	printf("WRITING : %d BYTES\n", (int)buffer_size);
	
	//Open file
	if(f_open(&file, filename, FA_OPEN_ALWAYS | FA_WRITE ) != FR_OK){ //Could be that the file already exists
		if(f_open(&file, filename, FA_WRITE) != FR_OK){
//		printf("RETURN 1");
		return SD_LOG_FAILED;
				}
		}
//	printf("FILE SIZE : %d\n", (int)f_size(&file));

	UINT aux = 0;	

	if (file_position >= BYTES_START_OVERWRITE)
	{
		file_position = 20;
	}
	
	
		if(f_lseek(&file, 0) != FR_OK){ // WRITE SIZE AT THE FIRST LINE
//			printf("RETURN 2");
			return SD_LOG_FAILED;
		}
				
		char buf[25];
		add_zeroes_twenty((int)f_size(&file), buf);
//		printf("f_buf size: %d, f_buf: %s\n", strlen(buf), buf);
		//sprintf(buf, "%s\r\n", buf); // 8 caracteres + 2 ? . Se calhar nao quero o \n?
		
		if((f_write(&file, buf, (uint32_t) strlen(buf), &aux) != FR_OK)){
//			printf("RETURN 3\n");
			return SD_LOG_FAILED;
		}

	//Move to the end of the file and append file_name_size
	/*if(f_lseek(&file, f_size(&file) + buffer_size) != FR_OK)
		return SD_LOG_FAILED;*/
		
	//Move to the end of the file and append file_name_size
	if(f_lseek(&file, file_position) != FR_OK){
//		printf("RETURN 2");
		return SD_LOG_FAILED;
	}

	//Move pointer to the beginning of the new data
	/*if(f_lseek(&file, f_size(&file) - buffer_size) != FR_OK)
		return SD_LOG_FAILED;*/





	//printf("BUFFER_SIZE: %d\n", (int)buffer_size);

	//printf("DIFERENÇA: %d\n", file_position - file_position_prev);
	
	file_position_prev = file_position;
	
//	printf("data: %s\n", data);

	if (!strcmp(filename, "READINGS.txt") && (int) buffer_size < 25 && data != NULL && strlen(data) < 25 && strlen(data) > 0){
		if((f_write(&file, data, buffer_size, &aux) != FR_OK)){
			//printf("RETURN 3");
			return SD_LOG_FAILED;
		}
		file_position += (int) buffer_size;
	}
	

	if (file_position - file_position_prev > 40)
		file_position = file_position_prev;
		
	//	printf("**************FILE_POSITION (AFTER SUMMATION): %d\n", file_position);

	f_close(&file);
}

int log2sd(char* message, char *filename){ // returns -1 if SD_LOG is disabled
 
#if SD_LOG == 1
        char buffer[128];
        sprintf(buffer, message);
 
        if(sd_card.fs_type == 0) { //SD card not mounted
                //Mount the SD card
                if(f_mount(&sd_card, "", 1) == 0){
            //    printf("Mounted SD card!\n");
                        log_to_sd(filename, buffer, strlen(buffer));
                        f_mount(NULL, "", 1);
                }
        }
        else { //SD card already mounted, someone is logging to it!
              //  printf("Fui parar aqui!\n");
                log_to_sd(filename, buffer, strlen(buffer));
                        f_mount(NULL, "", 1);
        }
       
        return 0;
#endif
 
        return -1;
}
#endif /* SD_LOG */

#if ACCELEROMETER_SD_CARD_LOG
//TODO add append buffer!
uint32_t open_file(FIL *file, const char * filename){
	//Open file
	if(f_open(file, filename, FA_CREATE_NEW | FA_WRITE) != FR_OK){ //Could be that the file already exists
		if(f_open(file, filename, FA_WRITE) != FR_OK) //Try to open file
			return SD_OPEN_FILE_FAILED;
	}

	return SD_SUCCESS;
}

uint32_t extend_file(FIL *file, uint32_t size){
	//Move to the end of the file and append file_name_size
	if(f_lseek(file, f_size(file) + size) != FR_OK)
		return SD_WRITE_FILE_FAILED;

	//Move pointer to the beginning of the new data
	if(f_lseek(file, f_size(file) - size) != FR_OK)
		return SD_WRITE_FILE_FAILED;

	return SD_SUCCESS;
}

uint32_t write_to_sd(FIL *file, const char * data, uint32_t buffer_size){
	UINT aux = 0;

	if(f_write(file, data, buffer_size, &aux) != FR_OK)
		return SD_WRITE_FILE_FAILED;

	return SD_SUCCESS;
}

uint32_t close_file(FIL *file){
	if(f_close(file) != FR_OK)
		return SD_CLOSE_FILE_FAILED;

	return SD_SUCCESS;
}

#endif /* ACCELEROMETER_SD_CARD_LOG */

int lerCartao(){
	FIL file;       // File object
    char buf[12];
	unsigned int bytesread;
    if(sd_card.fs_type == 0) { //SD card not mounted
		//Mount the SD card
		if(f_mount(&sd_card, "", 1) == 0){
			printf("Mounted SD card!\n");
			if(f_open(&file, "TEMP.TXT", FA_READ) != FR_OK){ //Could be that the file already exists
				printf("ERROR Opening!\n");
				return 0;
			}
			while(1){
				f_read(&file,buf,12,&bytesread);
				if(bytesread==0) break;
				int y=0;
				while(1){
					printf("%c",buf[y]);
					y++;
					if(y>bytesread) break;
				}
			}
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


char * itoa_embedded (int32_t value, char *result, int base, const int value_type){
	switch(value_type){
		case I_16:
			if(value >> 15)
				value = (0xFFFF0000 | value);
		break;
	}

    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if ((tmp_value < 0) && !(value_type == U_32))  *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
