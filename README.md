# Embedded lab work, Óbuda University (2023)

C firmware from my Erasmus semester at Óbuda University, Budapest (winter semester 2023/24, Embedded Systems I and Vehicle On-board Systems). There are two STM32 projects built with STM32CubeIDE and the HAL, and five AVR programs that drive the registers directly.

## Origin

These are lab exercises, not standalone projects. The CubeIDE projects started from material provided in the course. Files whose header names a different author (`test.c`/`test.h`, "BD") come from that material and were adapted by me. Code that CubeMX generates (HAL setup, startup, system files) is ST boilerplate and has its own license headers. The application logic lives in the `USER CODE` sections of `main.c` and in `test.c`.

## STM32F756ZG (Arm Cortex-M7)

| Project | What it does | Peripherals |
|---|---|---|
| [`stm32/01_uart_led_control`](stm32/01_uart_led_control) | Single-character commands over a serial terminal switch an LED between timer-driven blinking, off, and 20 % or 70 % PWM brightness | USART3 (9600 baud), TIM3 update interrupt, TIM12 PWM, GPIO |
| [`stm32/02_status_led_menu`](stm32/02_status_led_menu) | A text-command menu over UART that drives status LEDs: blink patterns, three PWM brightness levels, a fading "danger" mode and a random number between 20 and 55 | USART3, TIM2/TIM4 interrupts, TIM12 PWM, hardware RNG, GPIO |

**Build:** open the `.ioc` file in STM32CubeIDE and run *Generate Code*, which restores the `Drivers/` folder (HAL and CMSIS, not committed). Then build and flash.

## AVR (register-level C, Atmel Studio)

| Program | What it does |
|---|---|
| [`avr/01_led_patterns`](avr/01_led_patterns/main.c) | Running-light patterns on 8 LEDs split over two ports, selected with push buttons |
| [`avr/02_running_light_buttons`](avr/02_running_light_buttons/main.c) | Back-and-forth running light with start and stop buttons |
| [`avr/03_keypad_sevenseg`](avr/03_keypad_sevenseg/main.c) | Countdown from 8 to 2 on a seven-segment digit with start and stop buttons, plus the scan routine for a 3x4 matrix keypad |
| [`avr/04_countdown_keypad`](avr/04_countdown_keypad/main.c) | Four-digit multiplexed countdown from 1000 with start, stop and keypad entry of the start value |
| [`avr/05_countdown_timer_interrupts`](avr/05_countdown_timer_interrupts/main.c) | The same countdown moved into a 1 ms Timer0 CTC interrupt, plus a Timer2 overflow interrupt for software PWM. **Unfinished:** this is the last saved state and does not compile as is |

Clock: 8 or 16 MHz depending on the board (see `F_CPU` in each file).
