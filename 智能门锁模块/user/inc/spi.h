#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"
#include "io_bit.h"

#define SPI_SCK PBout(13)
#define SPI_MOSI PBout(15)

void SPIPinInit(void);
void SPISendByte(u8 data);


#endif

