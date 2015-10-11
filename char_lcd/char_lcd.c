#ifndef F_CPU  
  #define F_CPU   16000000    // 16 MHz - turn on external resonator (fuse bits)
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
//-------------------------------------
// main program
//-------------------------------------
int main(void)
{
char temp, i;
LCD_Initialize();
DDRB = 0b00000000;
PORTB = 0b00001111;

int value = 0;
int calculations = 0;

	char dzialanie = 0;
	do{

	char sw0 = PINB & 0b00000001;
	char sw1 = PINB & 0b00000010;
	char sw2 = PINB & 0b00000100;
	char sw3 = PINB & 0b00001000;

	if(sw0 != 0b00000001) value++;
	if(sw1 != 0b00000010) value--;

	if(sw2 != 0b00000100) {
		if(dzialanie == 0) calculations += value;
		if(dzialanie == 1) calculations -= value;
		if(dzialanie == 2) calculations /= value;
		if(dzialanie == 3) calculations *= value;
			value = 0;
	}


	if(sw3 != 0b00001000) {
		dzialanie++;
		dzialanie = dzialanie % 4;
	}




	  char str[15];


	  sprintf(str, "%15d", calculations);
	LCD_GoTo(1,0);
	LCD_WriteText(str);

	  sprintf(str, "%15d", value);
	LCD_GoTo(1,1);
	LCD_WriteText(str);
	_delay_ms(300);

	
	if(dzialanie == 0){
		LCD_GoTo(0,0);
		LCD_WriteText("+");
	}

	if(dzialanie == 1){
		LCD_GoTo(0,0);
		LCD_WriteText("-");
	}
	
	if(dzialanie == 2){
		LCD_GoTo(0,0);
		LCD_WriteText("/");
	}
	
	if(dzialanie == 3){
		LCD_GoTo(0,0);
		LCD_WriteText("*");
	}
	}while(1);

return 0;
}
//-------------------------------------
// End of file
//-------------------------------------
