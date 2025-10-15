# Manual de Usuario – Control de Brillo LED con PWM (Bare Metal)

**Proyecto:** Control de Brillo LED con PWM  
**Versión:** Final (14/10/2025)  
**Autor:** Edwin Rodriguez – Curso *Estructuras Computacionales*  
**Universidad Nacional de Colombia – Sede Manizales**

---

## 1. ¿Qué es este proyecto?

Un sistema de control de brillo para un LED utilizando **PWM (Modulación por Ancho de Pulso)** con la tarjeta **STM32L476RG**.  
Permite variar la intensidad luminosa del LED según el **duty cycle** configurado en el canal 1 del **TIM3**, simulando el comportamiento de una bombilla controlada electrónicamente.

---

## 2. Hardware Utilizado

- **Tarjeta:** Nucleo STM32L476RG  
- **LED PWM:** LED conectado al pin **PA6** (canal TIM3_CH1)  
- **Resistencia:** 220 Ω entre PA6 y GND  
- **Botón de usuario:** PC13 (activo bajo)  
- **UART:** PA2 = TX, PA3 = RX (115200 8N1)  
- **Alimentación:** Vía USB desde ST-LINK  
- **Indicador Heartbeat:** LED en PA5 (LD2 integrado en la placa)



---

## 3. Funcionalidades

### 3.1 Heartbeat
El LED **LD2/PA5** parpadea regularmente para indicar que el firmware está corriendo correctamente.

### 3.2 Control de Bombilla por Botón
Al presionar el botón de usuario (PC13), el sistema enciende el LED principal durante **3 segundos** y luego lo apaga automáticamente.  
El evento también se refleja mediante un mensaje UART.

### 3.3 Comunicación UART
Permite enviar comandos desde un terminal serial (por ejemplo, PuTTY o TeraTerm) para ajustar el brillo del LED PWM.

**Comandos soportados:**
- Ingresar un valor entre `0` y `100` → Ajusta el duty cycle en porcentaje.  
  Ejemplo:

  Resultado: brillo del LED al 75%.

### 3.4 Control PWM de Bombilla
El **PWM** se genera en el canal 1 del **TIM3**.  
El ciclo útil (duty cycle) se ajusta en tiempo real, variando el brillo del LED.  
Se prueban tres valores de referencia: **10%, 25% y 75%**.

---

## 4. Arquitectura del Sistema

### 4.1 Módulos de Software
- **gpio.c / gpio.h:** Configuración de pines y lectura del botón.  
- **tim.c / tim.h:** Inicialización y control de PWM.  
- **uart.c / uart.h:** Comunicación serial para comandos y mensajes.  
- **systick.c / systick.h:** Temporizador de sistema y retardos.  
- **nvic.c / nvic.h:** Configuración de interrupciones externas.  
- **main.c:** Lógica principal del programa.

### 4.2 Flujo de Ejecución
1. Inicializa los periféricos (GPIO, TIM3, UART, SysTick).  
2. Entra en un bucle principal (`while(1)`).  
3. Espera eventos:
 - Botón presionado → Enciende LED principal por 3 s.  
 - Comando UART recibido → Ajusta brillo PWM.  
4. LED Heartbeat indica ejecución activa.

---

## 5. Uso del Sistema

### 5.1 Conexión
1. Conectar la placa **Nucleo STM32L476RG** al PC mediante USB.  
2. Abrir un programa de terminal (baudrate: **115200**).  
3. Verificar el mensaje:  



### 5.2 Inicio
El LED heartbeat comienza a parpadear indicando que el sistema está corriendo.

### 5.3 Interacción
- Pulsar el botón → Enciende el LED principal por 3 segundos.  
- Enviar número 0–100 por UART → Ajusta el brillo del LED PWM.

---

## 6. Diagrama de Estados

**Estados principales del sistema:**
1. **IDLE:** Espera de eventos.  
2. **BOTÓN PRESIONADO:** Enciende LED principal.  
3. **TEMPORIZADOR ACTIVO:** Cuenta 3 s y apaga el LED.  
4. **UART COMANDO:** Actualiza brillo PWM.  
5. **HEARTBEAT:** Parpadeo continuo.
6. Estado INICIALIZACIÓN → ESPERA → BOTÓN_PRESIONADO → TEMPORIZACIÓN → ESPERA.

---

## 7. Ejemplo de Código

```c
// Ejemplo de variación de brillo
tim3_ch1_pwm_set_duty_cycle(10);  // Brillo bajo
systick_delay_ms(3000);

tim3_ch1_pwm_set_duty_cycle(25);  // Brillo medio
systick_delay_ms(3000);

tim3_ch1_pwm_set_duty_cycle(75);  // Brillo alto
systick_delay_ms(3000);

