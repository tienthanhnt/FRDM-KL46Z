#include "MKL46Z4.h"

#define DELAY_CNT           (1000000)
#define RED_LED_PIN         (1 << 29)
#define SW1_PIN             (1 << 3)
#define GREEN_LED_PIN       (1 << 5)
#define SW2_PIN             (1 << 12)

void delay()
{
  uint32_t i,j;
  for(i=0;i<10000;i++)
    for(j=0;j<1000;j++);
}

int main()
{
  SIM->SCGC5 |= (uint32_t)SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK;// enable clock cho port C và port E. Tai sao thanh ghi 32 bit ma definition chi la 16 bit
  
  PORTC->PCR[3] &= ~(uint32_t)PORT_PCR_MUX_MASK;// xoa truong Mux cua thanh ghi PCR[3]
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_MUX(1);// ghi gia tri 1 vao truong Mux de chan hoat dong la GPIO
  
  PORTC->PCR[12] &= ~(uint32_t)PORT_PCR_MUX_MASK;// xoa truong Mux cua thanh ghi PCR[3]
  PORTC->PCR[12] |= (uint32_t)PORT_PCR_MUX(1);// ghi gia tri 1 vao truong Mux de chan hoat dong la GPIO
  
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_PE_MASK;// set truong PE ve 1 chan nay chi co the doc voi cac chan khong ho tro dien tro keo len, khi set bang 1 thi nghia la dang enable dien tro keo len
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_PS_MASK;// set truog PS ve 1 chan nay de set dien tro keo len hoac keo xuong, trong truong hop nay la keo len => trang thai mac dinh la high
  PORTC->PCR[12] |= (uint32_t)PORT_PCR_PE_MASK;// set truong PE ve 1 chan nay chi co the doc voi cac chan khong ho tro dien tro keo len, khi set bang 1 thi nghia la dang enable dien tro keo len
  PORTC->PCR[12] |= (uint32_t)PORT_PCR_PS_MASK;
  
  
  PORTE->PCR[29] &= ~(uint32_t)PORT_PCR_MUX_MASK;
  PORTE->PCR[29] |= (uint32_t)PORT_PCR_MUX(1);
  PORTD->PCR[5] &= ~(uint32_t)PORT_PCR_MUX_MASK;
  PORTD->PCR[5] |= (uint32_t)PORT_PCR_MUX(1);
  
  GPIOC->PDDR &= ~(uint32_t)SW1_PIN; // config pin 3 as input
  GPIOE->PDDR |= (uint32_t)RED_LED_PIN; // chan 29 la output
  
  GPIOC->PDDR &= ~(uint32_t)SW2_PIN;
  GPIOD->PDDR |= (uint32_t)GREEN_LED_PIN;
  
  GPIOE->PSOR |= (uint32_t)RED_LED_PIN;// chan 29 muc ra ung voi logic 1
  GPIOD->PDDR |= (uint32_t)GREEN_LED_PIN;
  
  while(1)
  {
    if(!(GPIOC->PDIR & SW1_PIN))
    {
      GPIOE->PTOR |= (uint32_t)RED_LED_PIN;// thanh ghi PTOR la thanh ghi toggle de thay doi trang thai hien tai
      delay();
    }
    if(!(GPIOC->PDIR & SW2_PIN)) {
      GPIOD->PTOR |= (uint32_t)GREEN_LED_PIN;
      delay();
    }
  }
}

