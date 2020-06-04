#include <MKL46Z4.h>

#define MODVAL (20000)

/*Config mode EPWM cho channel 2 TPM0
Clock in  = 20Mhz
PS =1
*/
void TpmInit() // Khoi tao cau hinh cho TPM
{
  SIM->SCGC5 |= (uint32_t)SIM_SCGC5_PORTE_MASK;
    
  PORTE->PCR[29] &= ~(uint32_t)PORT_PCR_MUX_MASK;
  PORTE->PCR[29] |= (uint32_t)PORT_PCR_MUX(3);
  /* Chon clock la MCGFLLOUT*/
  SIM->SOPT2 &= (uint32_t)~(uint32_t)SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
  
  SIM->SCGC6 |= (uint32_t)SIM_SCGC6_TPM0_MASK;
  /* Setup prescale = 1 */
  TPM0->SC &=(uint32_t)~(uint32_t)TPM_SC_PS_MASK;
  TPM0->SC &=(uint32_t)~(uint32_t)TPM_SC_CPWMS_MASK;
  
  TPM0->MOD = (uint16_t)MODVAL;
  TPM0->CONTROLS[2].CnSC |= (uint32_t)(TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK);
  TPM0->CONTROLS[2].CnSC &= (uint32_t)~(uint32_t)(TPM_CnSC_MSA_MASK);
  
  TPM0->CONTROLS[2].CnV = 10000;
  
  TPM0->SC &= (uint32_t)~(TPM_SC_CMOD_MASK);
  TPM0->SC |= TPM_SC_CMOD(1);
  
}

int main()
{
  uint32_t i,j;
  
  TpmInit();
  while(1)
  {
    for(i=0;i<20000;i++)
    {
      for(j=0;j<1000;j++)
        TPM0->CONTROLS[2].CnV = i;
    }
    for(i=20000;i>0;i--)
    {
      for(j=0;j<1000;j++)
        TPM0->CONTROLS[2].CnV = i;
    }
  }
}

