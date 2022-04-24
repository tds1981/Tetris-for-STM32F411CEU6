#include "main.h"

//дефайны команд управления дисплеем
#define		SWRESET		0x01    // Программный сброс
#define		SLPOUT		0x11    // Выход из режима сна
#define		DISPON		0x29    // включение дисплея
#define		CASET		  0x2A	  //x0=int,x1=int
#define		RASET		  0x2B	  //y0=int,y1=int
#define		RAMWR		  0x2C
#define		MADCTL		0x36	  //RGB.3=1, остальные=0
#define		COLMOD		0x3A	  //64k=5

#define maxX 128
#define maxY 160

//дефайны выводов порта В для управления дисплеем
#define		A0_set		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12);
#define		A0_res		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12);

#define		Reset_set		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
#define		Reset_res		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);

#define		CS_set		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14);
#define		CS_res		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14);

//коды цветов для одноцветного "экономного" вывода (1 байт = 8 пикселей на экране)
#define		blue		0xF800
#define		green		0x07E0
#define		red			0x001F
#define		black		0x0000
#define		white		0xFFFF

void DisplayInit();
void Window (unsigned char x0, unsigned char y0, unsigned char W, unsigned char 	H);
void fon(unsigned int back_color, unsigned char W, unsigned char 	H);
void SendCommand(unsigned char DATA);
void SendDATA(unsigned char DATA);
void Number(uint8_t Num, uint8_t x0, uint8_t y0, uint16_t color);
void PrintInt(int32_t NUM, uint8_t x0, uint8_t y0, uint8_t MAX_SIZE);