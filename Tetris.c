#include "Tetris.h"

#define XmaxTetr 12
#define YmaxTetr 15
#define cell 10
uint8_t Tetr[XmaxTetr][YmaxTetr];
int8_t F[13][8]      = { {0,0,0,1,1,0,1,1},     // 0  квадрат
												  {0,0,1,0,2,0,3,0},	  // 1 линия
												  {0,0,0,1,0,2,0,3},	  // 2 линия
												  {0,0,1,0,1,1,2,1},	  // 3 Z
												  {0,0,-1,1,0,1,-1,2},	// 4 Z
												  {0,0,1,0,-1,1,0,1},		// 5
												  {0,0,0,-1,1,0,1,1},		// 6
												  {0,0,-1,1,0,1,1,1}, 	// 7 T
												  {0,0,1,2,0,1,1,1},  	// 8 T
												  {0,2,-1,1,0,1,1,1}, 	// 9 T
												  {0,0,-1,1,0,1,0,2}, 	// 10 T
												  {0,0,1,0,0,1,0,2},  	// 11 Г
													{0,0,-1,0,0,1,0,2}		// 12
											 };
uint8_t Transform[13] = {0, 2, 1, 4, 3, 6, 5, 8, 9, 10, 7, 12, 11} ;
uint8_t DelStr=0;

void ShowTetris()
{
	uint8_t W = 120;
	uint8_t H = 150;
	uint16_t color;
	
	Window (1, 0, W, H);
	CS_res;
	SendCommand(RAMWR);
	//заливка области экрана выбранным цветом
	for(uint8_t i=0; i<YmaxTetr*cell; i++)
	{
		for(uint8_t j=0; j<XmaxTetr*cell; j++)
		{
				if (Tetr[j/cell][i/cell] == 0) color = black;
				if (Tetr[j/cell][i/cell] == 1) color = green;
				if (Tetr[j/cell][i/cell] == 2) color = red;
				if (Tetr[j/cell][i/cell] == 3) color = 0x0110;
				SendDATA(color>>8);
				SendDATA(color&0xFF);
		}
	}	
	CS_set;
	
}

uint8_t Element(int8_t dx, int8_t dy, uint8_t Transf)
{
	static int8_t x=6;
	static int8_t y=-1;
	static uint8_t N=0;
	uint8_t State=0;
	
	uint8_t old_N=N;
	int8_t old_x=x;
	int8_t old_y=y;
	
	y+=dy; 
	if (y>0) x+=dx; 
	if (x<0) x=0;
  if (x>=XmaxTetr)	x=XmaxTetr-1;
	if (Transf==1) N = Transform[N];
	
	for(int8_t i=YmaxTetr-1; i>=0; i--) // удаление заполненных строк
		if (Tetr[0][i]==3)
		{
			for(uint8_t j=0; j<XmaxTetr; j++)
				for(int8_t k=i; k>=0; k--)
					if (k>0) Tetr[j][k]=Tetr[j][k-1];
						else Tetr[j][k]=0;
		}			
		
	for(uint8_t i=0; i<4; i++) // удаление фигуры на старом месте
			if (Tetr[old_x+F[old_N][2*i]][old_y+F[old_N][2*i+1]]==1)
						Tetr[old_x+F[old_N][2*i]][old_y+F[old_N][2*i+1]]=0;		
	
	for(uint8_t i=0; i<4; i++) //проверка возможности перестановки фигуры по x
			if ((Tetr[x+F[N][2*i]][y+F[N][2*i+1]]==2)||(x+F[N][2*i]<0)||(x+F[N][2*i]>XmaxTetr-1)) {x=old_x; break;}
			
	for(uint8_t i=0; i<4; i++) //проверка возможности перестановки и необходимости фиксации фигуры
			if ((y+F[N][2*i+1]>=YmaxTetr)||(Tetr[x+F[N][2*i]][y+F[N][2*i+1]]==2)) 
				{
					//State=2; 
					for(uint8_t i=0; i<4; i++) // фиксация фигуры 			if ((old_x+F[old_N][2*i]>=0)&(old_y+F[old_N][2*i+1]>=0)) 
							Tetr[old_x+F[old_N][2*i]][old_y+F[old_N][2*i+1]]=2;	
					
					N=(TIM2->CNT>>(N/2)); N&=0x0f; if (N>12) N=N%2; 
					y=-1; x=6; 
					return 1;
				}
	
	for(uint8_t i=0; i<4; i++) //перестановка фигуры на новое место
			if ((x+F[N][2*i]>=0)&(y+F[N][2*i+1]>=0)) Tetr[x+F[N][2*i]][y+F[N][2*i+1]]=1; 
				
	for(uint8_t i=0; i<YmaxTetr; i++) //выделение заполненных строк
  {
		uint8_t flag=0;
		for(uint8_t j=0; j<XmaxTetr; j++)
			if (Tetr[j][i]==2) flag++;
		
		if (flag==XmaxTetr) {DelStr++; for(uint8_t j=0; j<XmaxTetr; j++) Tetr[j][i]=3;}
	}	
	return 0;
}	

