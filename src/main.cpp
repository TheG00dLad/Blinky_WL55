#include "stm32wl55xx.h"

void delay_ms(uint32_t ms);

int main(void)
{
    // Enable GPIOB clock (PB9 is on GPIOB)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // Set PB9 as output (MODER9[1:0] = 0b01)
    GPIOB->MODER &= ~(0x3 << (9 * 2)); // clear mode bits
    GPIOB->MODER |=  (0x1 << (9 * 2)); // set output mode

    // Optional: set push-pull output type (default)
    GPIOB->OTYPER &= ~(1 << 9);

    // Optional: no pull-up/pull-down
    GPIOB->PUPDR &= ~(0x3 << (9 * 2));

    while (1)
    {
        // Toggle PB9
        GPIOB->ODR ^= (1 << 9);
        delay_ms(500);
    }
}

void delay_ms(uint32_t ms)
{
    // crude software delay — adjust factor for your clock
    for (volatile uint32_t i = 0; i < ms * 4000; ++i) {
        __NOP();
    }
}
