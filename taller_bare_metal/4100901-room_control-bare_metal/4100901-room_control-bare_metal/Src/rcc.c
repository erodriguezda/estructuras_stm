#include "rcc.h"

void rcc_init(void)
{
    RCC->AHB2ENR |= (1 << 0);                      // Habilita reloj GPIOA
    RCC->AHB2ENR |= (1 << 2);                      // Habilita reloj GPIOC
}


void rcc_syscfg_clock_enable(void)
{
    // Habilitar el reloj del SYSCFG en el bus APB2
    RCC->APB2ENR |= (1 << 0);


}

void rcc_tim3_clock_enable(void)
{
    RCC->APB1ENR1 |= (1 << 1);  // TIM3 está en bit 1 del APB1ENR1
}
