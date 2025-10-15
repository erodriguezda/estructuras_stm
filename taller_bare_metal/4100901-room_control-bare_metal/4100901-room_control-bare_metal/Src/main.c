#include "tim.h"
#include "gpio.h"
#include "nvic.h"
#include "systick.h"
#include "rcc.h"
#include "uart.h"  // Agregar esta línea


static char rx_buffer[256];
static uint8_t rx_index = 0;

// --- Programa principal ------------------------------------------------------
int main(void)
{
    rcc_init();
    tim3_ch1_pwm_init(1000); // Inicializa PWM en TIM3 CH1 (frecuencia 1 kHz)
    tim3_ch1_pwm_set_duty_cycle(0); // Comienza con 0% de brillo
    init_gpio(GPIOA, 5, 0x01, 0x00, 0x01, 0x00, 0x00);
    init_gpio(GPIOC, 13, 0x00, 0x00, 0x01, 0x01, 0x00);
    init_systick();
    init_gpio_uart();  // Agregar inicialización GPIO para UART
    init_uart();       // Agregar inicialización UART
    nvic_usart2_irq_enable();
    nvic_exti_pc13_button_enable();

    
    uart_send_string("Sistema Inicializado!\r\n");  // Enviar mensaje de inicio

    // Duty cycle al 10%
    tim3_ch1_pwm_set_duty_cycle(10);
    systick_delay_ms(3000);

    // Duty cycle al 25%
    tim3_ch1_pwm_set_duty_cycle(25);
    systick_delay_ms(3000);

    // Duty cycle al 75%
    tim3_ch1_pwm_set_duty_cycle(75);
    systick_delay_ms(3000);

    // Duty cycle al 0% (apagar)
    tim3_ch1_pwm_set_duty_cycle(0);
    
    while (1) {
        if (read_gpio(GPIOC, 13) != 0) { // Botón presionado
            systick_ms_count = 0;   // reiniciar contador global del SysTick
            set_gpio(GPIOA, 5);     // Encender LED
        }

        if (systick_ms_count >= 3000) { // Si han pasado 3 segundos o más
            clear_gpio(GPIOA, 5);       // Apagar LED
        }


        // --- Efecto de respiración del LED (PWM) ---
        for (uint8_t duty = 0; duty <= 100; duty++) {
            tim3_ch1_pwm_set_duty_cycle(duty);
            systick_delay_ms(10); // ajusta la velocidad del cambio
        }

        for (int8_t duty = 100; duty >= 0; duty--) {
            tim3_ch1_pwm_set_duty_cycle(duty);
            systick_delay_ms(10);
        }


        // Polling UART receive
        if (USART2->ISR & (1 << 5)) {  // RXNE
            char c = (char)(USART2->RDR & 0xFF);
            if (rx_index < sizeof(rx_buffer) - 1) {
                rx_buffer[rx_index++] = c;
                if (c == '\r' || c == '\n') {
                    rx_buffer[rx_index] = '\0';  // Fin de cadena
                    int brillo = atoi(rx_buffer); // Convierte a número

                    if (brillo >= 0 && brillo <= 100) {
                        tim3_ch1_pwm_set_duty_cycle(brillo);
                        uart_send_string("Brillo ajustado a ");
                        uart_send_string(rx_buffer);
                        uart_send_string("%\r\n");
                    } else {
                        uart_send_string("Valor fuera de rango (0-100)\r\n");
                    }

                    rx_index = 0;

                }  
            }
        }
    }
}




// --- Interrupción de USART2 ---
void USART2_IRQHandler(void)
{
    if (USART2->ISR & (1 << 5)) { // RXNE
        char c = (char)(USART2->RDR & 0xFF);
        uart_send_string("UART IRQ: ");
        uart_send(c);
        uart_send_string("\r\n");
    }
}

// --- Interrupción del botón (PC13) ---
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR1 & (1 << 13)) {
        EXTI->PR1 |= (1 << 13); // Limpiar flag de interrupción
        uart_send_string("Botón IRQ!\r\n");
    }
}

