#include "systick.h"

#define HSI_FREQ    4000000U                                   // Reloj interno 4 MHz




void init_systick(void)
{
    SYSTICK->RVR = HSI_FREQ / 1000 - 1;                      // Recarga = 4000 - 1
    SYSTICK->CSR = (1 << 0) | (1 << 1) | (1 << 2);    // ENABLE|TICKINT|CLKSOURCE
}

volatile uint32_t systick_ms_count = 0; // Contador global de milisegundos

// --- Manejador de la interrupción SysTick ---
void SysTick_Handler(void)
{
    systick_ms_count++;
}

// --- Retorna el tiempo actual en milisegundos ---
uint32_t systick_get_tick(void)
{
    return systick_ms_count;
}

void systick_delay_ms(uint32_t ms)
{
    uint32_t start = systick_ms_count;
    while ((systick_ms_count - start) < ms) {
        // Esperar activamente hasta que pasen los milisegundos
    }
}