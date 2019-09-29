#ifndef _KBC_H_
#define _KBC_H_

#include <stdint.h>

void kbc_init(void);

struct Kbc {
    // Control register
    volatile uint8_t * control;
    // Interrupt control register
    volatile uint8_t * irq;
    // Status register
    volatile uint8_t * status;
    // Keyboard out register
    volatile uint8_t * keyboard_out;
    // Mouse out register
    volatile uint8_t * mouse_out;
    // Data in register
    volatile uint8_t * data_in;
};

extern __code struct Kbc KBC;

#define KBC_STS_OBF (1 << 0)
#define KBC_STS_IBF (1 << 1)
#define KBC_STS_CMD (1 << 3)

uint8_t kbc_status(struct Kbc * kbc);
uint8_t kbc_read(struct Kbc * kbc);
void kbc_keyboard(struct Kbc * kbc, uint8_t data);
void kbc_mouse(struct Kbc * kbc, uint8_t data);

void kbc_event(struct Kbc * kbc);

__xdata volatile uint8_t __at(0x1300) KBHICR;
__xdata volatile uint8_t __at(0x1302) KBIRQR;
__xdata volatile uint8_t __at(0x1304) KBHISR;
__xdata volatile uint8_t __at(0x1306) KBHIKDOR;
__xdata volatile uint8_t __at(0x1308) KBHIMDOR;
__xdata volatile uint8_t __at(0x130A) KBHIDIR;

#endif // _KBC_H_
