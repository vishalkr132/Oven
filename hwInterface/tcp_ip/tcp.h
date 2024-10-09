#ifndef _TCP_H
#define _TCP_H

#include <stdint.h>

#define PORT 80
#define SERVER "api.thingspeak.com"
#define API_KEY "LHZS8JKRXTL3UZRY"

int32_t TCP_fnInit();
int32_t TCP_fnSend(int32_t temp_val);
int32_t TCP_fnRecv();
int32_t TCP_fnClose();

#endif