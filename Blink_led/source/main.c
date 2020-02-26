#include "MKL46Z4.h"

#define DELAY_CNT           (1000000)
#define RED_LED_PIN         (1 << 29)
#define GREEN_LED_PIN       (1 << 5)

void initLed();
void delay();


int main(void)
{
    initLed();

    while (1)
    {
        delay();
        /* Toggle Red Led */
        FPTE->PTOR |= RED_LED_PIN;
        /* Toggle Green Led */
        FPTD->PTOR |= GREEN_LED_PIN;
    }
}
void initLed()
{
    /* Enable clock for PORTE & PORTD */
    SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK );

    /* Initialize the RED LED (PTE29)*/
    /* Set the PTE29 pin multiplexer to GPIO mode */
    PORTE->PCR[29] = PORT_PCR_MUX(1);
    /* Set the pin's direction to output */ 
    FPTE->PDDR |= RED_LED_PIN;
    /* Set the initial output state to high */
    FPTE->PSOR |= RED_LED_PIN;

    /* Initialize the Green LED (PTD5) */
    /* Set the PTD5 pin multiplexer to GPIO mode */
    PORTD->PCR[5]= PORT_PCR_MUX(1);
    /* Set the pin's direction to output */
    FPTD->PDDR |= GREEN_LED_PIN;
    /* Set the initial output state to high */
    FPTD->PSOR |= GREEN_LED_PIN;
}

void delay()
{
    uint32_t i;
    for (i = 0; i < DELAY_CNT; i++)
    {
        __asm("nop"); // asm code do nothing
    }
}

