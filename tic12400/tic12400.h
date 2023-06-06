/*
 * tic124000.h
 *
 *  Created on: 5 апр. 2022 г.
 *      Author: Ruslan
 */

#ifndef INC_TIC12400_H_
#define INC_TIC12400_H_

#include <stdbool.h>
#include "tic12400_reg.h"
#include "parity/parity.h"
#include "spi/spi.h"

//структура фрейма передачи
typedef struct PACKED TIC12400_TX_BIT {
	unsigned par :1;
	unsigned data :24;
	unsigned addr :6;
	unsigned rw :1;
} tic12400_tx_t;
static_assert(sizeof(tic12400_tx_t) == TIC12400_FRAME_SIZE, "Invalid size of tic12400_tx_t!");

typedef union PACKED _TIC12400_TX_FRAME {
	uint32_t all;
	uint8_t byte[4];
	struct TIC12400_TX_BIT bit;
} TIC12400_TX_FRAME;

//структура фрейма приема
typedef struct PACKED TIC12400_RX_BIT {
	unsigned par :1;
	unsigned data :24;
	unsigned oi :1;
	unsigned temp :1;
	unsigned vs_th :1;
	unsigned ssc :1;
	unsigned par_fail :1;
	unsigned spi_fail :1;
	unsigned por :1;
} tic12400_rx_t;
static_assert(sizeof(tic12400_rx_t) == TIC12400_FRAME_SIZE, "Invalid size of tic12400_rx_t!");

typedef union PACKED _TIC12400_RX_FRAME {
	uint32_t all;
	uint8_t byte[4];
	struct TIC12400_RX_BIT bit;
} TIC12400_RX_FRAME;

typedef void (*tic12400_callback)(void*);

//структура последовательного чтения
typedef struct {
	uint8_t count;
	uint8_t index;
	const uint8_t *addr;
	uint32_t *data;
} tic12400_sequential_t;

//структура управления TIC12400
typedef struct {
	SPI_BUS_TypeDef *spi_bus;
	const CFG_REG_SPI_TypeDef *spi_cfg;
	SPI_BUS_DATA_TypeDef spi_control;
	TIC12400_TX_FRAME tx_frame;
	TIC12400_RX_FRAME rx_frame;
	TIC12400_RX_FRAME status_mask;
	tic12400_callback status_callback;
	void *status_callback_argument;
	tic12400_sequential_t sequential;
	bool done;
} tic12400_t;

extern void tic12400_init(tic12400_t *tic, SPI_BUS_TypeDef *spi_bus, const CFG_REG_SPI_TypeDef *spi_cfg);

extern void tic12400_spi_bus_configure(tic12400_t *tic);

extern void tic12400_wait(tic12400_t *tic);

extern void tic12400_bus_free(tic12400_t *tic);

extern void tic12400_reg_read(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t count);

extern void tic12400_reg_read_from_callback(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t count);

extern void tic12400_reg_write(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t count);

extern void tic12400_reg_write_from_callback(tic12400_t *tic, uint32_t *data, const uint8_t *addr, uint8_t count);


#endif /* INC_TIC12400_H_ */
