#include <MKL46Z4.h>
#include <stdio.h>

#define COUNT_NUM 199999; // Tinh count_num nhu the nao
#define RED_LED_PIN         (1 << 29)
void init_sysTick () // Mac di khi bat con chip len thi clock o khoang 20 MHz. Sau khi hoc xong bai clock thi phai set lai clock
{
  SysTick->LOAD = (uint32_t) COUNT_NUM;
  SysTick->VAL = 0;
  SysTick->CTRL |= (uint32_t) (SysTick_CTRL_CLKSOURCE_Msk |  SysTick_CTRL_ENABLE_Msk);
}

void initLed ()
{
  SIM->SCGC5 |= (uint32_t)SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;// enable clock cho port C và port E. Tai sao thanh ghi 32 bit ma definition chi la 16 bit
  
  PORTE->PCR[29] &= ~(uint32_t)PORT_PCR_MUX_MASK;// // xoa truong Mux cua thanh ghi PCR[29]
  PORTE->PCR[29] |= (uint32_t)PORT_PCR_MUX(1);// ghi gia tri 1 vao truong Mux de chan hoat dong la GPIO
  GPIOE->PDDR |= (uint32_t)RED_LED_PIN; // chan 29 la output
  GPIOE->PSOR |= (uint32_t)RED_LED_PIN;// chan 29 muc ra ung voi logic 1
}

int main()
{
  uint32_t tick, cur_tick;
  tick = 0;
  cur_tick = 0;
  initLed();
  init_sysTick ();
  printf("tick = %d", tick);
  while (1) {
    while(SysTick->CTRL & (1<<16))
    {
      tick++;
    }
    
    if (tick - cur_tick == 500) {
      cur_tick = tick;
      GPIOE->PTOR |= (uint32_t)RED_LED_PIN;
    }
  }
}
