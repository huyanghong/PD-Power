#ifndef __OLED_INIT_H
#define __OLED_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "u8g2.h"
#include "main.h"
#include "i2c.h"

#define OLED_ADDRESS 0x78

#define OLED_I2C_Handle hi2c1
#define OLED_I2C_Init  MX_I2C1_Init

#define OLED_I2C_SCL(n)	 (n? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET))
#define OLED_I2C_SDA(n)	 (n? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET))

void u8g2Init(u8g2_t *u8g2);


#ifdef __cplusplus
}
#endif

#endif // __OLED_INIT_H

