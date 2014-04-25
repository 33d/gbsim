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
