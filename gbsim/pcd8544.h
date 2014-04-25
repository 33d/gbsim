#if !defined(__PCD8544_H__)
#define __PCD8544_H__

#include "sim_avr.h"
#include "sim_irq.h"
#include <stdint.h>

enum {
    IRQ_PCD8544_SPI_IN = 0,
    IRQ_PCD8544_DC,
    IRQ_PCD8544_CS,
    IRQ_PCD8544_RST,
};

enum {
    PCD8544_MODE_BLANK = 0,
    PCD8544_MODE_ALL_ON,
    PCD8544_MODE_NORMAL,
    PCD8544_MODE_INVERSE
};

typedef struct pcd8544_t {
    avr_irq_t* irq;
    uint8_t ram[84*6];
    int dc;
    int cs;
    int rst;
    int horizontal_addressing;
    int extended;
    int active;
    int x;
    int y;
    int init_reset_state;
    int vop;
    int mode;
    int updated;
    const char* error;
} pcd8544_t;

void pcd8544_init(struct avr_t* avr, pcd8544_t* p);

#endif
