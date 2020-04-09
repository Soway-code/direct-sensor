#ifndef _COMMON_H
#define _COMMON_H

#include "stm32l0xx.h"
//#include "stm32l031xx.h"

void Unshort2Array(uint16_t SourceData, uint8_t* Array);
void Unlong2Array(uint32_t SourceData, uint8_t* Array);

void long32Array(uint32_t SourceData, uint8_t* Array);
#endif