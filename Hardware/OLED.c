#include "stm32f10x.h"                  // Device header
#include "OLED_Font.h"
#include "OLED.h"

// Define OLED I2C Pin out function;
#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)(x))
#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)(x))

void OLED_I2C_GPIOinit(void)
{
	
	/*Initial GIPO 
		GPIO_Pin_B12		SDA
		GPIO_Pin_B13		SCL
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_12;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_13;
	GPIO_Init(GPIOB,& GPIO_InitStruct);
	
}

void OLED_I2C_Start(void)
{
	OLED_W_SCL(1);
	OLED_W_SDA(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);	
}

void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);	
	OLED_W_SCL(1);	
	OLED_W_SDA(1);
}


void OLED_I2C_SendByte(uint8_t senddata)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		OLED_W_SDA(senddata & (0x80>>i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
		OLED_W_SCL(1); // no ack;
		OLED_W_SCL(0);
}

void OLED_SendCommand(uint8_t command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLEDI2CAddrWrite);
	OLED_I2C_SendByte(OLEDI2CModeCommand);		//写命令
	OLED_I2C_SendByte(command); 
	OLED_I2C_Stop();
}

void OLED_SendData(uint8_t data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(OLEDI2CAddrWrite);		//从机地址
	OLED_I2C_SendByte(OLEDI2CModeData);		//写数据
	OLED_I2C_SendByte(data);
	OLED_I2C_Stop();
}

void OLED_SetPosition(uint8_t y, uint8_t x)
{
	OLED_SendCommand(OLEDCommandPageAddrStartPage | y);	//设置Y位置,Page,0~7,
	OLED_SendCommand(OLEDCommandPageAddrStartAddrHigherh4bit | ((x & 0xF0) >> 4));	//设置X位置高4位,
	OLED_SendCommand(OLEDCommandPageAddrStartAddrLow4bit | (x & 0x0F));							//设置X位置低4位
}

void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetPosition(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_SendData(0x00);
		}
	}
}


/*用于显示16*16字符，
ROW 范围1~8；
Col 范围1~128；
*/

void OLED_ShowChar(uint8_t Row, uint8_t Col, char Char)
{      	
	uint8_t i;
	OLED_SetPosition((Row - 1) * 2, (Col - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_SendData(asc2_1608[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetPosition((Row - 1) * 2 + 1, (Col - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_SendData(asc2_1608[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

void OLED_ShowString(uint8_t Row, uint8_t	Col, char *String)
{
	uint8_t i;
	for(i=0;String[i] != '\0';i++)
	{
		OLED_ShowChar(Row,Col+i,String[i]);
	}
		
}


/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}
/**
  * @brief OLED显示一个5位的无符号数
  * @retval 
  */
void OLED_ShowUnsignNum(uint8_t Row, uint8_t Col, uint32_t Num, uint8_t Length)
{
	uint8_t i;
	for(i=0; i< Length; i++)
	{
		OLED_ShowChar(Row,Col+i, (Num / (OLED_Pow(10,Length-i-1)))%10+'0');
	}
} 

void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_GPIOinit();			//端口初始化
	
	OLED_SendCommand(0xAE);	//关闭显示
	
	OLED_SendCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_SendCommand(0x80);
	
	OLED_SendCommand(0xA8);	//设置多路复用率
	OLED_SendCommand(0x3F);
	
	OLED_SendCommand(0xD3);	//设置显示偏移
	OLED_SendCommand(0x00);
	
	OLED_SendCommand(0x40);	//设置显示开始行
	
	OLED_SendCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_SendCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_SendCommand(0xDA);	//设置COM引脚硬件配置
	OLED_SendCommand(0x12);
	
	OLED_SendCommand(0x81);	//设置对比度控制
	OLED_SendCommand(0xCF);

	OLED_SendCommand(0xD9);	//设置预充电周期
	OLED_SendCommand(0xF1);

	OLED_SendCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_SendCommand(0x30);

	OLED_SendCommand(0xA4);	//设置整个显示打开/关闭

	OLED_SendCommand(0xA6);	//设置正常/倒转显示

	OLED_SendCommand(0x8D);	//设置充电泵
	OLED_SendCommand(0x14);

	OLED_SendCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}

