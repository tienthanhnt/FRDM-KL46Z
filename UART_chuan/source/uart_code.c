#include "MKL46Z4.h"

#define RED_LED_PIN            (1<<29)
#define GREEN_LED_PIN          (1<<5)

/*FUNCTION**********************************************************************
 *
 * Function Name : init_led
 * Description   : Initialize the GREEN & RED LED.
 *
 *END**************************************************************************/
void init_led()
{
    /* Enable clock for PORTE & PORTD*/
    SIM->SCGC5 |= 0x000003000;
    /* 
    * Initialize the RED LED (PTE29)
    */
    PORTE->PCR[29] = PORT_PCR_MUX(1);

    /* Set the pins direction to output */
    GPIOE->PDDR |= RED_LED_PIN;

    /* Set the initial output state to high */
    GPIOE->PSOR |= RED_LED_PIN;

    /* 
    * Initialize the Green LED (PTD5)
    */

    /* Set the PTD5 pin multiplexer to GPIO mode */
    PORTD->PCR[5]= PORT_PCR_MUX(1);

    /* Set the pins direction to output */
    GPIOD->PDDR |= GREEN_LED_PIN;

    /* Set the initial output state to high */
    GPIOD->PSOR |= GREEN_LED_PIN;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UpdateClockTo8Mhz
 * Description   : Blink Led .
 *
 *END**************************************************************************/
void UpdateClockTo8Mhz(void)
{
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  MCG->C1 |= MCG_C1_IREFS_MASK;
  MCG->C6 &= ~MCG_C6_PLLS_MASK;
  
  MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
  MCG->C4 |= MCG_C4_DRST_DRS(1);
  MCG->C4 |= MCG_C4_DMX32_MASK;
  
  SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
  SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(2);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UpdateClockTo8Mhz In mode FEES
 * Description   : Blink Led .
 *
 *END**************************************************************************/
void UpdateClockTo8Mhz_FEE(void)
{
  MCG->C1 &= ~MCG_C1_CLKS_MASK;
  MCG->C1 &= ~MCG_C1_IREFS_MASK;
  MCG->C1 &= ~MCG_C1_FRDIV_MASK;
  MCG->C1 |= MCG_C1_FRDIV(3);
  MCG->C6 &= ~MCG_C6_PLLS_MASK;
  
  MCG->C7 &= ~MCG_C7_OSCSEL_MASK;
  MCG->C2 &= ~MCG_C2_RANGE0_MASK;
  MCG->C2 |= MCG_C2_RANGE0(1);
  
  MCG->C4 &= ~MCG_C4_DRST_DRS_MASK;
  MCG->C4 |= MCG_C4_DRST_DRS(1);
  MCG->C4 &= ~MCG_C4_DMX32_MASK;
  
  SIM->CLKDIV1 &= ~SIM_CLKDIV1_OUTDIV1_MASK;
  SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(4);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : UART0 Init
 * Description   : init UART0 module.
 *
 *END**************************************************************************/
void uart_init(void)
{
  SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
  // Set chan 1 va 2 cua PORTA làm chân gui va nhan UART
  PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK;
  PORTA->PCR[1] = PORT_PCR_MUX(2);
  PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;
  PORTA->PCR[2] = PORT_PCR_MUX(2);
  
  UART0->C1 &= ~UART0_C1_M_MASK;
  UART0->C4 &= ~UART0_C4_M10_MASK;
  
  UART0->S2 |= UART0_S2_MSBF_MASK;
  SIM->SOPT2 &=~SIM_SOPT2_UART0SRC_MASK;
  SIM->SOPT2 |=SIM_SOPT2_UART0SRC(1);
  
  UART0->C4 &= ~UART0_C4_OSR_MASK;
  UART0->C4 |= UART0_C4_OSR(5);
  
  UART0->BDH &= ~UART0_BDH_SBR_MASK;
  UART0->BDL &= ~UART0_BDL_SBR_MASK;
  UART0->BDL |= UART0_BDL_SBR(60);
  
  UART0->C2 |=UART0_C2_TE_MASK;
  UART0->C2 |=UART0_C2_RE_MASK;
  
}
char uart0_getchar () // ham nhan ky tu 
{
      /* Wait until character has been received */
      while (!(UART0->S1 & UART0_S1_RDRF_MASK));
    
      /* Return the 8-bit data from the receiver */
      return UART0->D;
}
void uart0_putchar ( char ch) // ham gui ky tu
{
      /* Wait until space is available in the FIFO */
      while((UART0->S1 & UART0_S1_TC_MASK)==0);
    
      /* Send the character */
      UART0->D = (uint8_t)ch;
      
//      while(((UART0_S1 & UART0_S1_TC_MASK) >> UART0_S1_TC_SHIFT)==0);
   
 }
/*FUNCTION**********************************************************************
 *
 * Function Name : main
 * Description   : Blink Led .
 *
 *END**************************************************************************/
int main(void)
{
    init_led();
    UpdateClockTo8Mhz_FEE();
    uart_init();
    while (1)
    {
      uart0_putchar('b');
    }
}

