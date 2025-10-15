// systick.h
#include <stdint.h>

typedef struct {
    volatile uint32_t CSR;   // Control and Status Register
    volatile uint32_t RVR;   // Reload Value Register
    volatile uint32_t CVR;   // Current Value Register
    volatile uint32_t CALIB; // Calibration Value Register
} SYSTICK_Typedef_t;

#define SYSTICK_BASE 0xE000E010U
#define SYSTICK      ((SYSTICK_Typedef_t *) SYSTICK_BASE)

void init_systick(void);
void systick_delay_ms(uint32_t ms);

extern volatile uint32_t systick_ms_count;

uint32_t systick_get_tick(void);


