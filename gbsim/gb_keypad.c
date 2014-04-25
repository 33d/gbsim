#include <stdlib.h>
#include "gb_keypad.h"

static const char* irq_names[GB_KEYPAD_COUNT] = {
        [GB_KEYPAD_UP] = "gc.keypad.up",
        [GB_KEYPAD_DOWN] = "gc.keypad.down",
        [GB_KEYPAD_LEFT] = "gc.keypad.left",
        [GB_KEYPAD_RIGHT] = "gc.keypad.right",
        [GB_KEYPAD_A] = "gc.keypad.a",
        [GB_KEYPAD_B] = "gc.keypad.b",
        [GB_KEYPAD_C] = "gc.keypad.c",
};

void gb_keypad_press(gb_keypad_t* keypad, int key, int state) {
    avr_raise_irq(keypad->irq + key, state);
}

void gb_keypad_init(avr_t* avr, gb_keypad_t* keypad) {
    keypad->avr = avr;
    keypad->irq = avr_alloc_irq(&avr->irq_pool, 0, GB_KEYPAD_COUNT, irq_names);

    // Start with keys high
    for (int i = 0; i < GB_KEYPAD_COUNT; i++)
        gb_keypad_press(keypad, i, 1);
}
