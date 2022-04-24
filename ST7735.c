#include "ST7735.h"

const unsigned short Tahoma[] = {
        0x7E, 0xFF, 0x81, 0x81, 0xFF, 0x7E, 0x00, 0x00,  // Code for char 0
        0x84, 0x82, 0xFF, 0xFF, 0x80, 0x80, 0x00, 0x00,  // Code for char 1
        0xC2, 0xE3, 0xB1, 0x99, 0x8F, 0x86, 0x00, 0x00,  // Code for char 2
        0x42, 0xC3, 0x89, 0x89, 0xFF, 0x76, 0x00, 0x00,  // Code for char 3
        0x30, 0x28, 0x24, 0xFE, 0xFF, 0x20, 0x00, 0x00,  // Code for char 4
        0x40, 0xCF, 0x8F, 0x89, 0xF9, 0x71, 0x00, 0x00,  // Code for char 5
        0x7E, 0xFF, 0x89, 0x89, 0xF9, 0x70, 0x00, 0x00,  // Code for char 6
        0x01, 0x01, 0xE1, 0xFD, 0x1F, 0x03, 0x00, 0x00,  // Code for char 7
        0x76, 0xFF, 0x89, 0x89, 0xFF, 0x76, 0x00, 0x00,  // Code for char 8
        0x0E, 0x9F, 0x91, 0x91, 0xFF, 0x7E, 0x00, 0x00,  // Code for char 9
        0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char :
        0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Code for char ;
        0x00, 0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0x82   // Code for char <
        };

uint8_t Send_SPI(uint8_t dt)
{
	(void) SPI1->DR;
  while(!LL_SPI_IsActiveFlag_TXE(SPI1)) {}
	LL_SPI_TransmitData8(SPI1, dt);
	while(!LL_SPI_IsActiveFlag_TXE(SPI1)) {}	
}

void SendCommand(unsigned char DATA)
{
	A0_res;
	Send_SPI(DATA);
}

void SendDATA(unsigned char DATA)
{
	A0_set;
	Send_SPI(DATA);
}

//инициализация дисплея
void DisplayInit()
{
	Reset_set;
	CS_res;
	LL_SPI_Enable(SPI1);

	SendCommand(SWRESET);	//программный ресет
	LL_mDelay(220);

	SendCommand(SLPOUT);		//будим после ресета
	LL_mDelay(220);

	SendCommand(MADCTL);		//меняем вывод цветов с GBR на RGB
	SendDATA(0x08);

	SendCommand(COLMOD);		//меняем кодировку цветопередачи на 2-байтную (64К цветов)
	SendDATA(5);
	
	SendCommand(0xB1);   //Частота обновления дисплея.
	SendDATA(0);
	SendDATA(0x0F);	
	SendDATA(0);
	SendDATA(0x0F); 
	SendDATA(0);
	SendDATA(0x0F); 
	
	SendCommand(DISPON);		//включаем экран
	CS_set;
	
}

void fon(unsigned int back_color, unsigned char W, unsigned char 	H)
{
	unsigned int k;
	CS_res;
	SendCommand(RAMWR);
	//заливка области экрана выбранным цветом
	for(k=0; k<W*H; k++)
	{
		SendDATA(back_color>>8);
		SendDATA(back_color&0xFF);
	}
	CS_set;
}

void Window (unsigned char x0, unsigned char y0, unsigned char W, unsigned char 	H)
	//x0, y0 - координаты левой верхней точки окна
//w - длина надписи в пикселях, h - высота надписи в строчках
{

	//задание периметра вывода изображения
	CS_res;
	SendCommand(CASET);
	SendDATA(0);
	SendDATA(x0);
	SendDATA(0);
	SendDATA(x0+W-1);

	SendCommand(RASET);
	SendDATA(0);
	SendDATA(y0);
	SendDATA(0);
	SendDATA(y0+H-1);
	CS_set;
}

//-----------------------------------------------------------

void Number(uint8_t Num, uint8_t x0, uint8_t y0, uint16_t color)
{
	//uint8_t x=x0, y=y0;
	
	Window (x0, y0, 8, 8);
	
	CS_res;
	SendCommand(RAMWR);
	
	for(uint8_t j=0; j<8; j++)
	{
		for(uint8_t k=Num*8; k<8+Num*8; k++)	
		{
			if ((Tahoma[k]>>j)&0x01) 	{SendDATA(color>>8); SendDATA(color&0xFF);}
			else {SendDATA(black>>8); SendDATA(black&0xFF);}
		}	
	}		
	CS_set;	
}

//------------------------------------------------------------------

#define DIG_BASE  10 
//#define MAX_SIZE 5
//unsigned char SYMBOLS[DIG_BASE] = {'0','1','2','3','4','5','6','7','8','9'};
void PrintInt(int32_t NUM, uint8_t x0, uint8_t y0, uint8_t MAX_SIZE)
{
	int i, m;
	
	//if (NUM<0) Number(0xff, x0, y0); // print minus
	//else Number(0xfe, x0, y0); 
	
	uint8_t x=x0+10*(MAX_SIZE-1)+5;
	for(i=MAX_SIZE-1; i>=0; --i)
	{	
		m = NUM % DIG_BASE; 
		NUM /= DIG_BASE;  
		if (m<0) m*=-1;
		Number(m, x, y0, white); 
		x-=10;
	}
//	if (MAX_SIZE<6) Ramka();
//	PrintBufer();
}
