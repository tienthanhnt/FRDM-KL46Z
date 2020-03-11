#include <MKL46Z4.h>

#define PORTSHIFT(x)    (1 << x)
#define SW1  3
#define LED1 29

void init_sw()
{
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[SW1] &= ~PORT_PCR_MUX_MASK;
  PORTC->PCR[SW1] |= PORT_PCR_MUX(1);
  PORTC->PCR[SW1] &= ~PORT_PCR_IRQC_MASK;
  PORTC->PCR[SW1] |= PORT_PCR_IRQC(10);
  PORTC->PCR[SW1] |= PORT_PCR_PE_MASK;
  PORTC->PCR[SW1] |= PORT_PCR_PS_MASK;
  GPIOC->PDDR &= ~PORTSHIFT(SW1);
}

void init_led()
{
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[LED1] |= PORT_PCR_MUX(1);
  GPIOE->PDDR |= PORTSHIFT(LED1);
  GPIOE->PSOR |= PORTSHIFT(LED1);
}

void init_interrupt()
{
  NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
  NVIC_EnableIRQ(PORTC_PORTD_IRQn);
  NVIC_SetPriority(PORTC_PORTD_IRQn,1);
}
void PORTC_PORTD_IRQHandler()
{
  GPIOE->PTOR |= PORTSHIFT(LED1);
  PORTC->ISFR &= ~PORT_ISFR_ISF_MASK;
  PORTC->ISFR |= PORT_ISFR_ISF(0);
}

void main()
{
  init_led();
  init_interrupt();
  init_sw();
  while(1);
}