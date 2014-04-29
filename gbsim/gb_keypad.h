/*
        gbsim is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        gbsim is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with gbsim.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __GB_KEYPAD_H_
#define __GB_KEYPAD_H_

#include "sim_irq.h"
#include "sim_avr.h"

enum {
    GB_KEYPAD_UP = 0,
    GB_KEYPAD_DOWN,
    GB_KEYPAD_LEFT,
    GB_KEYPAD_RIGHT,
    GB_KEYPAD_A,
    GB_KEYPAD_B,
    GB_KEYPAD_C,
    GB_KEYPAD_COUNT
};

typedef struct gb_keypad_t {
    struct avr_t* avr;
    avr_irq_t* irq;
} gb_keypad_t;

void gb_keypad_press(gb_keypad_t* keypad, int key, int state);

void gb_keypad_init(avr_t* avr, gb_keypad_t* keypad);

#endif /* GB_KEYPAD_H_ */
