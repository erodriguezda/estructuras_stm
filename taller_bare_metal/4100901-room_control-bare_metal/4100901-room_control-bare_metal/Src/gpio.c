#include "gpio.h"                                     // Pin PC13 (Button)


void init_gpio(GPIO_Typedef_t * GPIO, uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed, uint8_t pupd, uint8_t initial_value)
{
    GPIO->MODER &= ~(3 << (pin*2));           // Limpia bits
    GPIO->MODER |=  (mode << (pin*2));           // Configura modo

    GPIO->TYPER &= ~(1 << pin);           // Limpia bits
    GPIO->TYPER |=  (type << pin);

    GPIO->SPEEDR &= ~(3 << (pin*2));
    GPIO->SPEEDR |= (speed << (pin*2));
    
    GPIO->PUPDR &= ~(3 << (pin*2));
    GPIO->PUPDR |= (pupd << (pin*2));

    GPIO->ODR &= ~(1 << pin);
    GPIO->ODR |= (initial_value << pin);
}

void set_gpio(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    GPIO->ODR |= (1 << pin);
}

void clear_gpio(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    GPIO->ODR &= ~(1 << pin);
}


uint8_t read_gpio(GPIO_Typedef_t * GPIO, uint8_t pin)
{
    // Leer estado del botón PC13 (botón presionado = 0, no presionado = 1)
    if ((GPIO->IDR & (1 << pin)) == 0) { // pressed
        return 1;
    }
    return 0;
}

// --- Alias de compatibilidad con código que usa gpio_setup_pin ---
// --- Alias de compatibilidad con código que usa gpio_setup_pin ---
void gpio_setup_pin(GPIO_Typedef_t *GPIO, uint8_t pin, uint8_t mode, uint8_t af)
{
    // Configura el pin en modo alterno con los valores por defecto
    // type = 0 (push-pull), speed = 2 (medium), pupd = 0 (none)
    init_gpio(GPIO, pin, mode, 0, 2, 0, 0);

    // Configura la función alternativa (AF)
    if (pin < 8) {
         GPIO->AFRL &= ~(0xF << (pin * 4));
         GPIO->AFRL |= (af << (pin * 4));
    } else {
        GPIO->AFRH &= ~(0xF << ((pin - 8) * 4));
        GPIO->AFRH |= (af << ((pin - 8) * 4));
}

}



