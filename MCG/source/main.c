#include "MKL46Z4.h"
#include <stdbool.h>

#define RED_LED_PIN            (1<<29)
#define GREEN_LED_PIN          (1<<5)
#define CORECLOCK8MHZ          (7995392)        
unsigned int tick = 0;
/*FUNCTION**********************************************************************
 *
 * Function Name : init_led
 * Description   : Initialize the GREEN & RED LED.
 *
 *END**************************************************************************/
void init_led()
{
    /* Enable clock for PORTE & PORTD*/
    SIM->SCGC5 |= ( SIM_SCGC5_PORTD_MASK
                    | SIM_SCGC5_PORTE_MASK );
    /* 
    * Initialize the RED LED (PTE5)
    */
    PORTE->PCR[29] = PORT_PCR_MUX(1);

    /* Set the pins direction to output */
    FPTE->PDDR |= RED_LED_PIN;

    /* Set the initial output state to high */
    FPTE->PSOR |= RED_LED_PIN;

    /* 
    * Initialize the Green LED (PTE5)
    */

    /* Set the PTE29 pin multiplexer to GPIO mode */
    PORTD->PCR[5]= PORT_PCR_MUX(1);

    /* Set the pins direction to output */
    FPTD->PDDR |= GREEN_LED_PIN;

    /* Set the initial output state to high */
    FPTD->PSOR |= GREEN_LED_PIN;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : my_delay
 * Description   : Use SysTick Timer to delay.
 *
 *END**************************************************************************/
void my_delay(uint32_t TICK)
{
    uint32_t current_tick = tick;
    while ((tick - current_tick) < TICK)
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : init_SysTick
 * Description   : Initialize the SysTick Timer.
 * This function has configured the SysTick to count in 1ms
 *END**************************************************************************/
void init_SysTick()// Dung Systick de tao timer dem thoi gian
{
    SysTick->LOAD = CORECLOCK8MHZ / 1000;
    /* Select Core Clock & Enable SysTick & Disable Interrupt */
    SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SystemCoreClockUpdate_8MHz
 * Description   : Update core clock.
 * This function has configured the core clock to 8MHz
 *END**************************************************************************/
void SystemCoreClockUpdate_8MHz()// tao clock cho he thong
{
    /* CLKS = 00:Selects the MCGOUTCLK is FLL or PLL output (depends on PLLS control bit)*/
    MCG->C1 &= (uint8_t)~(MCG_C1_CLKS_MASK);
    /* PLLS =0: Selects the MCGOUTCLK is FLL output*/
    MCG->C6 &= (uint8_t)~(MCG_C6_PLLS_MASK);
    /* IREFS = 1: Selects the input FLL is slow internal reference clock */
    MCG->C1 |= MCG_C1_IREFS(1);
    /* DRST_DRS = 00: Selects DCO range is 20-25Mhz or 24MHz (depends on DMX32 bit)*/
    MCG->C4 &= (uint8_t)~(MCG_C4_DRST_DRS_MASK);
    /* DMX32 = 1: Selects DCO range is 24MHz*/
    MCG->C4 |= MCG_C4_DMX32_MASK;
    /* OUTDIV1 = 2: Selects the Clock 1 Output Divider value is 3*/    
    SIM->CLKDIV1 = (uint32_t)SIM_CLKDIV1_OUTDIV1(2);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : fei_fee
 * Description   : Update core clock.
 * This function has configured the core clock to 8MHz used FEE mode
 *END**************************************************************************/
void fei_fee()
{ 
    MCG->C1 &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK);
    /* CLKS = 00:Selects the MCGOUTCLK is FLL or PLL output (depends on PLLS control bit)*/
    MCG->C1 &= (uint8_t)~(MCG_C1_CLKS_MASK);
    
    MCG->C1 |= MCG_C1_IREFS(0);
    
    MCG->C1 |= MCG_C1_FRDIV(3);
    
    MCG->C2 &= ~(MCG_C2_RANGE0_MASK | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK);
    MCG->C2 |= (MCG_C2_RANGE0(1) | (MCG_C2_HGO0(1)) | (MCG_C2_EREFS0(0)));
    
    /* PLLS =0: Selects the MCGOUTCLK is FLL output*/
    MCG->C6 &= (uint8_t)~(MCG_C6_PLLS_MASK);
    /* DRST_DRS = 00: Selects DCO range is 20-25Mhz or 24MHz (depends on DMX32 bit)*/
    MCG->C4 &= (uint8_t)~(MCG_C4_DRST_DRS_MASK);
    /* DMX32 = 1: Selects DCO range is 24MHz*/
    MCG->C4 |= MCG_C4_DMX32_MASK;
    /* OUTDIV1 = 2: Selects the Clock 1 Output Divider value is 3*/    
    SIM->CLKDIV1 = (uint32_t)SIM_CLKDIV1_OUTDIV1(0);  
} // fei_fee
/*FUNCTION**********************************************************************
 *
 * Function Name : main
 * Description   : Blink Led .
 *
 *END**************************************************************************/
int main(void)
{
//    SystemCoreClockUpdate_8MHz();
    fei_fee();
    init_led();
    init_SysTick();
    while (1)
    {
        my_delay(500); /* Wait for 50 TICKs*/
        FPTE->PTOR |= RED_LED_PIN;
        FPTD->PTOR |= GREEN_LED_PIN;
    }
}
void SysTick_Handler(void)
{
    tick++;
    (uint32_t)SysTick->CTRL;
}
