#include "stm32wl55xx.h"

void delay_ms(uint32_t ms);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void selectAlternateFunction (GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t AF);
void setup();


int main(void)
{
    // Optional: set push-pull output type (default)
    GPIOB->OTYPER &= ~(1 << 9);

    setup();

    // Optional: no pull-up/pull-down
    GPIOB->PUPDR &= ~(0x3 << (9 * 2));

while (1)
{
    GPIOB->ODR |= (1 << 9);      // Turn on PB9
    GPIOC->ODR &= ~(1 << 13);    // Turn off PC13
    delay_ms(100);
    
    GPIOB->ODR &= ~(1 << 9);     // Turn off PB9
    GPIOC->ODR |= (1 << 13);     // Turn on PC13
    delay_ms(100);
}
}
void setup()
{
    RCC->AHB2ENR |= (1 << 0) | (1 << 1) | (1 << 2); // Enable GPIOA, GPIOB, and GPIOC clock
    // select alternative function mode for the SPI pins
    pinMode(GPIOC,13,1);
    pinMode(GPIOB,9,1);  // Alt func
    // pinMode(GPIOB,9,2);
    // pinMode(GPIOA,11,2);
    // pinMode(GPIOA,1,2);
}
void delay_ms(uint32_t ms)
{
    // crude software delay — adjust factor for your clock
    for (volatile uint32_t i = 0; i < ms * 4000; ++i) {
        __NOP();
    }
}

void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
        Modes : 00 = input
                01 = output
                10 = special function
                11 = analog mode
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}

void selectAlternateFunction (GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t AF)
{
    // The alternative function control is spread across two 32 bit registers AFR[0] and AFR[1]
    // There are 4 bits for each port bit.
    if (BitNumber < 8)
    {
        Port->AFR[0] &= ~(0x0f << (4*BitNumber));
        Port->AFR[0] |= (AF << (4*BitNumber));
    }
    else
    {
        BitNumber = BitNumber - 8;
        Port->AFR[1] &= ~(0x0f << (4*BitNumber));
        Port->AFR[1] |= (AF << (4*BitNumber));
    }
}