/*
 * test.h
 *
 *  Created on: Nov 17, 2023
 *      Author: BD
 */

#ifndef INC_TEST_H_
#define INC_TEST_H_

#include "stm32f7xx_hal.h"
#include "main.h"

void f_menu_transmit(void);
void f_off_1(uint8_t * connecting, uint8_t * normal, uint8_t * danger, uint8_t * on);
void f_on_8(uint8_t * connecting, uint8_t * normal, uint8_t * on);
void f_connecting_2(uint8_t * connecting, uint8_t * normal);
void f_normal_3(uint8_t * connecting, uint8_t * normal, uint8_t * danger);
void f_pwm_4(uint8_t pwm, uint8_t * on);
void f_pwm_5(uint8_t pwm, uint8_t * on);
void f_pwm_6(uint8_t pwm, uint8_t * on);
void f_danger_7(uint8_t * danger, uint8_t * normal, uint8_t * on);
void f_rng_9(uint8_t * connecting, uint8_t * normal, uint8_t * danger, uint8_t * on, uint8_t rng_min, uint8_t rng_max);
void f_status_off(uint8_t on);
void f_status_test(uint8_t connecting, uint8_t normal, uint8_t danger, uint8_t on);
uint8_t menu_command(void);

#endif /* INC_TEST_H_ */
