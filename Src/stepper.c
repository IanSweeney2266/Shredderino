/*
 * stepper.c
 *
 *  Created on: Dec 1, 2018
 *      Author: Iansweeney
 */
#include "stepper.h"

void init_stepper(struct _stepper* s) {
	s->step_in_seq = 0;
	s->seq_len = 8;

	s->cur_pos = 0;
	s->desired_pos = 0;
	s->direction = STEPPER_CW;

}

void step(struct _stepper* s) {
	static uint16_t HALF_STEP_SEQ[8][4] = {
				  {1, 0, 0, 0},
				  {1, 1, 0, 0},
				  {0, 1, 0, 0},
				  {0, 1, 1, 0},
				  {0, 0, 1, 0},
				  {0, 0, 1, 1},
				  {0, 0, 0, 1},
				  {1, 0, 0, 1}
				};

	static uint16_t FULL_STEP_SEQ[4][4] = {
					  {1, 0, 0, 1},
					  {1, 1, 0, 0},
					  {0, 1, 1, 0},
					  {0, 0, 1, 1}
					};

	if (s->direction == STEPPER_CW) {
		if (s->step_in_seq == s->seq_len) {
			s->step_in_seq = 0;
		}
		else {
			s->step_in_seq++;
		}
		s->cur_pos++;
	}
	else {
		if (s->step_in_seq == 0) {
			s->step_in_seq = s->seq_len;
		}
		else {
			s->step_in_seq--;
		}
		s->cur_pos--;
	}


	uint16_t *pin_vals = HALF_STEP_SEQ[s->step_in_seq];
	HAL_GPIO_WritePin(s->Port_A1, s->A1, pin_vals[0]);
	HAL_GPIO_WritePin(s->Port_B1, s->B1, pin_vals[1]);
	HAL_GPIO_WritePin(s->Port_A2, s->A2, pin_vals[2]);
	HAL_GPIO_WritePin(s->Port_B2, s->B2, pin_vals[3]);
}

void run_stepper(struct _stepper* s) {
	if (s->cur_pos < s->desired_pos) {
		s->direction = STEPPER_CW;
		step(s);
	}
	else if (s->cur_pos > s->desired_pos) {
		s->direction = STEPPER_CCW;
		step(s);
	}
}

void init_picks(struct _stepper *p1) {
  /* Init pick 1*/
  p1->Port_A1 = GPIOE;
  p1->A1 = GPIO_PIN_13;
  p1->Port_A2 = GPIOE;
  p1->A2 = GPIO_PIN_12;
  p1->Port_B1 = GPIOE;
  p1->B1 = GPIO_PIN_11;
  p1->Port_B2 = GPIOE;
  p1->B2 = GPIO_PIN_10;
  init_stepper(p1);
}

