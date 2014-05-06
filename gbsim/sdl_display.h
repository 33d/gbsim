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

#if !defined(__SDL_DISPLAY_H__)
#define __SDL_DISPLAY_H__

#include <stdint.h>

int display_init();
int display_update();
int display_destroy();
int display_update(const uint8_t ram[84*6]);
int display_render();
void display_rate(int rate);
const char* display_error_message();

#endif
