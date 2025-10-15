#include "room_control.h"

#include "gpio.h"    // Para controlar LEDs
#include "systick.h" // Para obtener ticks y manejar tiempos
#include "uart.h"    // Para enviar mensajes
#include "tim.h"     // Para controlar el PWM

// Estados de la sala
typedef enum {
    ROOM_IDLE,
    ROOM_OCCUPIED
} room_state_t;

// Variable de estado global
room_state_t current_state = ROOM_IDLE;

static uint32_t last_button_press_time = 0;
static uint8_t current_duty_cycle = PWM_INITIAL_DUTY;


void room_control_app_init(void)
{
    // TODO: Implementar inicializaciones específicas de la aplicación
    tim3_ch1_pwm_init(PWM_FREQUENCY);
    tim3_ch1_pwm_set_duty_cycle(PWM_INITIAL_DUTY);
    uart_send_string("Room Control inicializado (PWM 50%)\r\n");

}

void room_control_on_button_press(void)
{
    // TODO: Implementar la lógica para manejar la pulsación del botón usando estados
    // Ejemplo: Si idle, cambiar a occupied; si occupied, cambiar a idle
    if (current_state == ROOM_IDLE) {
        current_state = ROOM_OCCUPIED;
        tim3_ch1_pwm_set_duty_cycle(current_duty_cycle);
        uart_send_string("Estado: ROOM_OCCUPIED\r\n");
    } else {
        current_state = ROOM_IDLE;
        tim3_ch1_pwm_set_duty_cycle(0);
        uart_send_string("Estado: ROOM_IDLE\r\n");
    }

    last_button_press_time = systick_get_tick();

}


void room_control_on_uart_receive(char received_char)
{
    switch (received_char) {
        case 'h': case 'H':
            current_duty_cycle = 100;
            tim3_ch1_pwm_set_duty_cycle(current_duty_cycle);
            uart_send_string("PWM: 100%\r\n");
            break;

        case 'l': case 'L':
            current_duty_cycle = 0;
            tim3_ch1_pwm_set_duty_cycle(current_duty_cycle);
            uart_send_string("PWM: 0%\r\n");
            break;

        case 'o': case 'O':
            current_state = ROOM_OCCUPIED;
            tim3_ch1_pwm_set_duty_cycle(current_duty_cycle);
            uart_send_string("Estado: ROOM_OCCUPIED\r\n");
            break;

        case 'i': case 'I':
            current_state = ROOM_IDLE;
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Estado: ROOM_IDLE\r\n");
            break;

        default:
            uart_send_string("Comando desconocido\r\n");
            break;
    }
}


void room_control_update(void)
{
    if (current_state == ROOM_OCCUPIED) {
        uint32_t now = systick_get_tick();
        if ((now - last_button_press_time) >= LED_TIMEOUT_MS) {
            current_state = ROOM_IDLE;
            tim3_ch1_pwm_set_duty_cycle(0);
            uart_send_string("Timeout: ROOM_IDLE\r\n");
        }
    }
}
