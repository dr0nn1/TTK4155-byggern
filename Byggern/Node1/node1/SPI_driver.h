/*
 * SPI_driver.h
 *
 * Created: 01.10.2020 10:16:08
 *  Author: jarlenes
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_



void SPI_send(char cData);
void SPI_init(void);
uint8_t SPI_read(void);
void SPI_select(void);
void SPI_deselect(void);
void SPI_test(void);

#endif /* SPI_DRIVER_H_ */