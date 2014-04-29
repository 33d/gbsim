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
#include "pcd8544.h"
#include "stdio.h"

#define PCD8544_IRQ_COUNT 4
#define PCD8544_COMMAND 0
#define PCD8544_DATA 1
#define PCD8544_ROWS 6
#define PCD8544_COLS 84
#define STRINGIFY(x) #x
#define PCD8544_ERROR(p, e) { (p)->error = (STRINGIFY(__LINE__) ": "  e) ; return; }

static const char* irq_names[PCD8544_IRQ_COUNT] = {
        [IRQ_PCD8544_SPI_IN] = "pcd8544.in",
        [IRQ_PCD8544_DC] = "pcd8544.dc",
        [IRQ_PCD8544_CS] = "pcd8544.cs",
        [IRQ_PCD8544_RST] = "pcd8544.rst"

};

static void pcd8544_handle_command(pcd8544_t* const p, uint8_t data) {
    if (p->init_reset_state != 0) {
        PCD8544_ERROR(p, "Boom! Not reset properly");
    }
    if ((data & 0xF8) == 0x20) { // Set function
        p->active = (data & 0x04) == 0;
        p->horizontal_addressing = (data & 0x02) == 0;
        p->extended = (data & 0x01) != 0;
    } else if (p->extended) {
        if (data == 1 || (data & 0xFE) == 2
                || (data & 0xF8) == 0x08 || (data & 0xFC) == 0x40) {
            PCD8544_ERROR(p, "Got 'Do not use' instruction");
        } else if ((data & 0x80) != 0)
            p->vop = (data & 0x7F);
        // ignore the rest
    } else {
        if ((data & 0xF8) == 0x04 || (data & 0xF0) == 0x10) {
            PCD8544_ERROR(p, "Got 'Do not use' instruction");
        } else if ((data & 0x0A) == 0x08) {
            switch (data & 0x05) {
            case 0: p->mode = (PCD8544_MODE_BLANK); break;
            case 1: p->mode = (PCD8544_MODE_ALL_ON); break;
            case 4: p->mode = (PCD8544_MODE_NORMAL); break;
            case 5: p->mode = (PCD8544_MODE_INVERSE); break;
            default: PCD8544_ERROR(p, "Internal error");
            }
        } else if ((data & 0xF8) == 0x40) {
            p->y = (data & 0x07);
        } else if ((data & 0x80) == 0x80) {
            p->x = (data & 0x7F);
        } else
            PCD8544_ERROR(p, "Unhandled data");
    }

    p->updated = 1;
}

static void pcd8544_handle_data(pcd8544_t* const p, uint8_t data) {
    int index = p->y*PCD8544_COLS + p->x;
    p->ram[index] = data;
    if (p->horizontal_addressing) {
        ++p->x;
        if (p->x >= PCD8544_COLS) {
            p->x = 0;
            p->y = (p->y+1) % PCD8544_ROWS;
        }
    } else {
        ++p->y;
        if (p->y >= PCD8544_ROWS) {
            p->y = 0;
            p->x = (p->x+1) % PCD8544_COLS;
        }
    }

    p->updated = 1;
}

static void pcd8544_reset(pcd8544_t* p) {
    p->active = 0;
    p->horizontal_addressing = 1;
    p->x = p->y = 0;
    p->vop = 0;
}

static void pcd8544_spi_in_hook(struct avr_irq_t * irq, uint32_t value, void * param) {
    pcd8544_t* const p = (pcd8544_t*) param;
    if (!p->cs) {
        if (p->dc == PCD8544_DATA)
            pcd8544_handle_data(p, (uint8_t) value);
        else
            pcd8544_handle_command(p, (uint8_t) value);
    }
}

static void pcd8544_dc_hook(struct avr_irq_t * irq, uint32_t value, void * param) {
    pcd8544_t* const p = (pcd8544_t*) param;
    p->dc = value;
}

static void pcd8544_cs_hook(struct avr_irq_t * irq, uint32_t value, void * param) {
    pcd8544_t* const p = (pcd8544_t*) param;
    p->cs = value;
}

static void pcd8544_rst_hook(struct avr_irq_t * irq, uint32_t level, void * param) {
    pcd8544_t* const p = (pcd8544_t*) param;
    if (p->init_reset_state == 2 && !level)
        --p->init_reset_state;
    if (p->init_reset_state == 1 && level)
        --p->init_reset_state;
    if (!level)
        pcd8544_reset(p);
}

void pcd8544_init(struct avr_t* avr, struct pcd8544_t* p) {
    p->init_reset_state = 2;

    p->irq = avr_alloc_irq(&avr->irq_pool, 0, PCD8544_IRQ_COUNT, irq_names);
    avr_irq_register_notify(p->irq + IRQ_PCD8544_SPI_IN, pcd8544_spi_in_hook, p);
    avr_irq_register_notify(p->irq + IRQ_PCD8544_DC, pcd8544_dc_hook, p);
    avr_irq_register_notify(p->irq + IRQ_PCD8544_CS, pcd8544_cs_hook, p);
    avr_irq_register_notify(p->irq + IRQ_PCD8544_RST, pcd8544_rst_hook, p);

    /* Write some garbage to RAM, so the user knows it's wrong */
    for (int i = 0; i < 84*6; i++)
        p->ram[i] = i;
}
