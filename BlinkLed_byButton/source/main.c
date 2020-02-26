#include "MKL46Z4.h"

#define DELAY_CNT           (1000000)
#define RED_LED_PIN         (1 << 29)
#define SW1_PIN             (1 << 3)

void delay() // Ham delay nay chua chinh xac, nen dung timer de tao delay
{
  uint32_t i,j;
  for(i=0;i<1000;i++)
    for(j=0;j<1000;j++);
}

int main()
{
  SIM->SCGC5 |= (uint32_t)SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;// enable clock cho port C và port E
  
  PORTC->PCR[3] &= ~(uint32_t)PORT_PCR_MUX_MASK;// xoa truong Mux cua thanh ghi PCR[3]
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_MUX(1);// ghi gia tri 1 vao truong Mux de chan hoat dong la GPIO
  
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_PE_MASK;// set truong PE ve 1 khi set bang 1 thi nghia la dang enable dien tro keo len
  PORTC->PCR[3] |= (uint32_t)PORT_PCR_PS_MASK;// set truog PS ve 1 trong truong hop nay la keo len

  PORTE->PCR[29] &= ~(uint32_t)PORT_PCR_MUX_MASK;// // xoa truong Mux cua thanh ghi PCR[29]
  PORTE->PCR[29] |= (uint32_t)PORT_PCR_MUX(1);// ghi gia tri 1 vao truong Mux de chan hoat dong la GPIO
  
  GPIOC->PDDR &= ~(uint32_t)SW1_PIN; // config pin 3 as input
  GPIOE->PDDR |= (uint32_t)RED_LED_PIN; // chan 29 la output
  
  GPIOE->PSOR |= (uint32_t)RED_LED_PIN;// chan 29 muc ra ung voi logic 1
  
  while(1)
  {
    if(!(GPIOC->PDIR & SW1_PIN))
    {
      GPIOE->PTOR |= (uint32_t)RED_LED_PIN;// thanh ghi PTOR la thanh ghi toggle de thay doi trang thai hien tai
      delay();
    }
  }
}

