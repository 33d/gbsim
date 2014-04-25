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

static pcd8544_t lcd;

/*
int run() {
    uint8_t fb[84*48];
    for (int i = 0; i < 84*48; ++i) {
        uint8_t val = ((i / (84)) & 1) == (i&1) ? 0xDE : 0;
        fb[i] = val;
    }
*/

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

    int state = cpu_Running ;
    int count = 16000000;
    while (( state != cpu_Done ) && ( state != cpu_Crashed ) && --count > 0 )
        state = avr_run(avr);

    if (display_init()) {
        display_update(lcd.ram);
        SDL_Delay(5000);
    } else {
        r = 1;
        fprintf(stderr, "%s\n", display_error_message());
    }

    display_destroy();

    return r;
}
