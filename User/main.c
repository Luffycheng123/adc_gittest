#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "ADC.h"


int main(void)
{
	OLED_Init();
	OLED_ShowString(1,1,"ADC Value is");
	AD_Init();
	
  while (1)
  {
		OLED_ShowUnsignNum(2,1,AD_GetValue(),5);
  }
}



