#include <SDL2/SDL.h>
#include <SDL2/SDL_assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "sim_avr.h"
#include "sim_elf.h"
#include "avr_ioport.h"
#include "avr_spi.h"

#include "sdl_display.h"
#include "pcd8544.h"
#include "gb_keypad.h"

typedef struct {
    SDL_Scancode key;
    int keypad_key;
    char port;
    int pin;
} keydef_t;

static const keydef_t keydefs[] = {
        { SDL_SCANCODE_W, GB_KEYPAD_UP,    'D', 1 },
        { SDL_SCANCODE_A, GB_KEYPAD_LEFT,  'D', 0 },
        { SDL_SCANCODE_S, GB_KEYPAD_DOWN,  'D', 3 },
        { SDL_SCANCODE_D, GB_KEYPAD_RIGHT, 'D', 2 },
        { SDL_SCANCODE_R, GB_KEYPAD_C,     'C', 3 },
        { SDL_SCANCODE_K, GB_KEYPAD_A,     'D', 4 },
        { SDL_SCANCODE_L, GB_KEYPAD_B,     'D', 2 },
};

#define keydefs_length (sizeof(keydefs) / sizeof(keydef_t))

static pcd8544_t lcd;
static gb_keypad_t keypad;
static uint8_t lcd_ram[84*6];
static int lcd_updated;
static SDL_mutex* lcd_ram_mutex;
static int quit_flag;
static int keys;

int avr_run_thread(void *ptr) {
    avr_t* avr = (avr_t*) ptr;
    int state = cpu_Running;
    do {
        int count = 10000;
        while (( state != cpu_Done ) && ( state != cpu_Crashed ) && --count > 0 )
            state = avr_run(avr);
        if (lcd.updated) {
            SDL_LockMutex(lcd_ram_mutex);
            memcpy(lcd_ram, lcd.ram, sizeof(lcd_ram));
            lcd_updated = 1;
            lcd.updated = 0;
            if (quit_flag) {
                SDL_UnlockMutex(lcd_ram_mutex);
                break;
            }

            int key_values = keys;
            for (int i = 0; i < keydefs_length; i++) {
                gb_keypad_press(&keypad, keydefs[i].keypad_key, (key_values & 1) == 0);
                key_values >>= 1;
            }

            SDL_UnlockMutex(lcd_ram_mutex);
        }
    } while (state != cpu_Done && state != cpu_Crashed);

    return 0;
}

void main_loop() {
    int quit = 0;
    SDL_Event e;

    while (!quit) {
        SDL_LockMutex(lcd_ram_mutex);

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN: {
                SDL_Scancode key = e.key.keysym.scancode;
                for (int i = 0; i < keydefs_length; i++) {
                    if (key == keydefs[i].key)
                        keys |= (1<<i);
                }
                break;
            }
            case SDL_KEYUP: {
                SDL_Scancode key = e.key.keysym.scancode;
                for (int i = 0; i < keydefs_length; i++) {
                    if (key == keydefs[i].key)
                        keys &= ~(1<<i);
                }
                break;
            }
            } /* switch */
        }

        int my_lcd_updated = lcd_updated;
        if (my_lcd_updated) {
            display_update(lcd_ram);
            lcd_updated = 0;
        }

        SDL_UnlockMutex(lcd_ram_mutex);

        if (my_lcd_updated)
            display_render();

        SDL_Delay(10);
    }
}

int main(int argc, const char* argv[]) {
    int r = 0;

    elf_firmware_t f;

    if (argc < 2) {
        fprintf(stderr, "Give me a .elf file to load\n");
        return 1;
    }
    elf_read_firmware(argv[1], &f);
    avr_t* avr = avr_make_mcu_by_name("atmega328p");
    if (!avr) {
        fprintf(stderr, "Unsupported cpu atmega328p\n");
        return 1;
    }

    avr_init(avr);
    avr->frequency = 16000000;
    avr_load_firmware(avr, &f);

    pcd8544_init(avr, &lcd);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 2),
            lcd.irq + IRQ_PCD8544_DC);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 1),
            lcd.irq + IRQ_PCD8544_CS);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('C'), 0),
            lcd.irq + IRQ_PCD8544_RST);
    avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_SPI_GETIRQ(0), SPI_IRQ_OUTPUT),
            lcd.irq + IRQ_PCD8544_SPI_IN);

    gb_keypad_init(avr, &keypad);
    for (int i = 0; i < keydefs_length; i++) {
        const keydef_t *k = keydefs + i;
        avr_connect_irq(keypad.irq + k->keypad_key,
                avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ(k->port), k->pin));
    }

    if (display_init()) {
        lcd_ram_mutex = SDL_CreateMutex();
        SDL_Thread* avr_thread = SDL_CreateThread(avr_run_thread, "avr-thread", avr);
        main_loop();
        SDL_LockMutex(lcd_ram_mutex);
        quit_flag = 1;
        SDL_UnlockMutex(lcd_ram_mutex);
        int avr_thread_return;
        SDL_WaitThread(avr_thread, &avr_thread_return);
        SDL_DestroyMutex(lcd_ram_mutex);
    } else {
        r = 1;
        fprintf(stderr, "%s\n", display_error_message());
    }

    display_destroy();

    return r;
}
