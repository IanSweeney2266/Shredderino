/*
 * stepper.h
 *
 *  Created on: Dec 1, 2018
 *      Author: Iansweeney
 */
#include "stm32f4xx_hal.h"
#define STEPPER_CW  1 /* CW */
#define STEPPER_CCW 0 /* CCW */

	/*---- STEPPER (5-PIN)  ----*/
	/*
	 * POWER - Red
	 * AIN1 - Orange
	 * AIN2 - Pink
	 * BIN1 - Yellow
	 * BIN2 - Blue
	 * Plug order:
	 *
	 */

	/*---- STEPPER (NEMA17) ----*/
	/*
	 * Ian's out of motor:
	 * 		Red Blue Green Black
	 * Ian's into mc:
	 * 		Green, Black, Blue, Red
	 * Jackson's out:
	 * 		Blue Yellow Green Red
	 * Jackson's in:
	 * 		Green Red Yellow Blue
	 *
	*/

struct _stepper {
	uint16_t A1;
	uint16_t A2;
	uint16_t B1;
	uint16_t B2;
	GPIO_TypeDef *Port_A1;
	GPIO_TypeDef *Port_A2;
	GPIO_TypeDef *Port_B1;
	GPIO_TypeDef *Port_B2;
	uint16_t step_in_seq;
	uint16_t seq_len;

	int16_t cur_pos;
	int16_t desired_pos;
	uint16_t direction;
};

void step(struct _stepper* s);

void init_stepper(struct _stepper* s);

void run_stepper(struct _stepper* s);

void init_picks(struct _stepper *p1);



