#include <MKL46Z4.h>


#define COUNT_NUM 19999
#define RED_LED_PIN         (1 << 29)

uint32_t tick = 0;

void init_systick()
{
  SysTick->LOAD = (uint32_t)COUNT_NUM;
  SysTick->VAL = 0;
  SysTick->CTRL |= (uint32_t)(SysTick_CTRL_CLKSOURCE_Msk); 
  SysTick->CTRL |= (uint32_t)(SysTick_CTRL_TICKINT_Msk);
  SysTick->CTRL |= (uint32_t)(SysTick_CTRL_ENABLE_Msk);
}

void SysTick_Handler()
{
  tick++;
  SysTick->CTRL;
}

void delay_ms(uint32_t time)
{
  uint32_t current_tick;
  current_tick = tick;
  while(tick-current_tick < time);
}
void init_led()
{
  SIM->SCGC5 |= (uint32_t)SIM_SCGC5_PORTE_MASK;
  
  PORTE->PCR[29] &= ~(uint32_t)PORT_PCR_MUX_MASK;
  PORTE->PCR[29] |= (uint32_t)PORT_PCR_MUX(1);
  
  GPIOE->PDDR |= (uint32_t)RED_LED_PIN;
  GPIOE->PSOR |= (uint32_t)RED_LED_PIN;
}

int main()
{
  init_led();
  init_systick();
  //tick=0;
  //current_tick=0;
  while(1)
  {
 /*   while(SysTick->CTRL & (1<<16))
    {
      tick++;
    }
    if((tick-current_tick) == 500)
    {
      current_tick = tick;
      GPIOE->PTOR |= (uint32_t)RED_LED_PIN;
    }*/
    
    GPIOE->PTOR |= (uint32_t)RED_LED_PIN;
    delay_ms(500);
  }
}