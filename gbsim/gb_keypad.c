#include <stdlib.h>
#include "gb_keypad.h"

void gb_keypad_press(gb_keypad_t* keypad, int key, int state) {
    avr_raise_irq(keypad->irq + key, state);
}

void gb_keypad_init(avr_t* avr, gb_keypad_t* keypad) {
    keypad->avr = avr;
    keypad->irq = avr_alloc_irq(&avr->irq_pool, 0, GB_KEYPAD_COUNT, NULL);

    // Start with keys high
    for (int i = 0; i < GB_KEYPAD_COUNT; i++)
        gb_keypad_press(keypad, i, 1);
}
