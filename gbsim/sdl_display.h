#if !defined(__SDL_DISPLAY_H__)
#define __SDL_DISPLAY_H__

#include <stdint.h>

int display_init();
int display_update();
int display_destroy();
int display_update(const uint8_t ram[84*6]);

#endif
