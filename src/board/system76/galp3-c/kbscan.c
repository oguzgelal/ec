#include <arch/delay.h>
#include <board/kbc.h>
#include <board/kbscan.h>
#include <board/keymap.h>
#include <common/debug.h>

bool kbscan_enabled = false;

void kbscan_init(void) {
    KSOCTRL = 0x05;
    KSICTRLR = 0x04;

    // Set all outputs to GPIO mode, low, and inputs
    KSOL = 0;
    KSOH1 = 0;
    KSOLGCTRL = 0xFF;
    KSOLGOEN = 0;
    KSOHGCTRL = 0xFF;
    KSOHGOEN = 0;
}

void kbscan_event(void) {
    static uint8_t kbscan_layer = 0;
    static uint8_t kbscan_last[KM_OUT] = { 0 };

    int i;
    for (i = 0; i < KM_OUT; i++) {
        // Set current line as output
        if (i < 8) {
            KSOLGOEN = 1 << i;
            KSOHGOEN = 0;
        } else if (i < 16) {
            KSOLGOEN = 0;
            KSOHGOEN = 1 << (i - 8);
        }

        // TODO: figure out optimal delay
        delay_ticks(10);

        uint8_t new = ~KSI;
        uint8_t last = kbscan_last[i];
        if (new != last) {
            int j;
            for (j = 0; j < 8; j++) {
                bool new_b = new & (1 << j);
                bool last_b = last & (1 << j);
                if (new_b != last_b) {
                    uint16_t key = keymap(i, j, kbscan_layer);
                    DEBUG("KB %d, %d, %d = 0x%04X, %d\n", i, j, kbscan_layer, key, new_b);
                    if (key == K_FN) {
                        if (new_b) kbscan_layer = 1;
                        else kbscan_layer = 0;
                    } else if (kbscan_enabled && key) {
                        kbc_scancode(&KBC, key, new_b);
                    }
                }
            }

            kbscan_last[i] = new;
        }
    }

    // Reset all lines to inputs
    KSOLGOEN = 0;
    KSOHGOEN = 0;

    // TODO: figure out optimal delay
    delay_ticks(10);
}
