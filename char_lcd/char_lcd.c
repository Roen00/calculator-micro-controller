#ifndef F_CPU  
  #define F_CPU   16000000    // 16 MHz - turn on external resonator (fuse bits)
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "HD44780.h"
//-------------------------------------
// main program
//-------------------------------------

int getADC(char channel)
{
unsigned int W = 0;
ADMUX |= channel;
ADCSRA |= (1 << ADSC);
while(ADCSRA & (1 << ADIF));
ADCSRA |= (1 << ADIF);
W = ADCL;
W |= (ADCH << 8);
return W;
}
//-------------------------------------
// ADC Initialization
//-------------------------------------
void ADC_Init(void)
{
ADMUX = (1 << REFS0);
ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

int digitMap[] = {1,2,3,4,7,9,14,30,100,850};

int signMap[] = {14,15};

int expressionMap[] = {3,9,30,850};

int mappingLogToLinear(int logValue, int* map, int length) {
	int i;
	for(i = 0; i < length; i++) {
		//std::cout<<logValue<<">"<<map[i]<<std::endl;
		if(map[i] >= logValue) {
			return i;
		}
	}
	return length - 1;
}




int state = 0;
int power = 0;
int lastValue = 0;
int newValue =0;
int sign = 1;


void setNewValue(int newDigit){
	int i;
	for(i = 0; i < power; i++){
		newDigit*=10;
	}

	newValue+=newDigit;
	power++;
}
void increaseState(){
	if(state < 3) {
		state++;
	}else{
		state = 0;
	}
	power = 0;
}
void doCalculations(int expression){
	switch(expression){
		case 0:
			lastValue+=newValue;
			break;
		case 1:
			lastValue-=newValue;
			break;
		case 2:
			lastValue*=newValue;
			break;
		case 3:
			if(newValue == 0) lastValue = 0;
			else lastValue/=newValue;
			break;
	}
}
void setSign(int sign){
	if(sign==0){
		newValue= newValue*(-1);
	}
}





int main(void)
{
char temp, i;
LCD_Initialize();
DDRB = 0b00000000;
PORTB = 0b00001111;

DDRA = 0xFF;
ADC_Init();
int value = 0;
int calculations = 0;

	char dzialanie = 0;

	
	int digit = 0;




	do{

	int digit  = getADC(0);

	char sw0 = PINB & 0b00000001;
	char sw1 = PINB & 0b00000010;
	if(sw0 != 0b00000001) {
		state++;
		_delay_ms(300);
	}
	

	  char str[15];


	  sprintf(str, "%15d", lastValue);
	LCD_GoTo(1,0);
	LCD_WriteText(str);


	switch(state){
		case 0:		
			if(sw1 != 0b00000010){
				setNewValue(mappingLogToLinear(digit, digitMap, 10));			
				_delay_ms(300);
			}
				sprintf(str, "%15d", mappingLogToLinear(digit, digitMap,10));
				LCD_GoTo(1,1);
				LCD_WriteText(str);	
			break;
		case 1:
			if(sw1 != 0b00000010){
				setSign(mappingLogToLinear(digit, signMap, 2));
				power = 0;				
				_delay_ms(300);
			}
				switch(mappingLogToLinear(digit, signMap,2)){
					case 0:
					LCD_GoTo(1,1);
					LCD_WriteText("-");
					break;
					case 1:
					LCD_GoTo(1,1);
					LCD_WriteText("+");
					break;
				}
			break;
		case 2:
			if(sw1 != 0b00000010){
				doCalculations(mappingLogToLinear(digit, expressionMap, 4));
				power = 0;
				newValue=0;
				state = 0;			
				_delay_ms(300);
			}
				
				switch(mappingLogToLinear(digit, expressionMap,4)){
					case 0:
					LCD_GoTo(1,1);
					LCD_WriteText("+");
					break;
					case 1:
					LCD_GoTo(1,1);
					LCD_WriteText("-");
					break;
					case 2:
					LCD_GoTo(1,1);
					LCD_WriteText("*");
					break;
					case 3:
					LCD_GoTo(1,1);
					LCD_WriteText("/");
					break;
				}
			break;
		}


/*
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
	}*/
	}while(1);

return 0;
}
//-------------------------------------
// End of file
//-------------------------------------
