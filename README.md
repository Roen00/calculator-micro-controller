# calculator-micro-controller
Diody sa podpiete do portu D

Porty:
A, B, C, D

Rejestry:
PORTx - dwojakie dzialanie zaleznie od DDR.
Jesli DDR wyjscie (1) i PORTX = 0 to masa 1 to plus


DDRx - ustawia wejscie lub wyjscie (0b00000000 - wejscie) (1 - wyjscie)
PINx - przedstawia aktualny stan napiec sygnalow na wejsciu

DDRB = 0b00010000
PORTB= 0b00000000

PINB = 4 bit bedzie pull up

1 - wycisniety przycisk
0 - wcisniety przycisk


LCD:
D7 - PA5
RS - PA7
RW - GND
E  - PA6
D5 - PA3
D6 - PA4
D4 - PA2

PA0 - PA2
PA1 - IR

Guziki:
SW0 - PB0
SW1 - PB1
SW2 - PB2
SW3 - PB3
