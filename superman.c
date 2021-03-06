/*
 * superman.c
 *
 * Created: 05/05/2021 1:53:30 pm
 * Author : Sebastian Santos
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile char count,digit; /* set count as volatile  because the value will change in the ISR */
int delay[3] = {0x0a,0x02,0x0a}; /* delay for green, yellow, red */

int timeDelay(int x) /* set timer1 as 1 sec timer */
{
	int sec;
	for (sec = 0; sec < delay[x]; sec++)
	{
		TCNT1 = 49911;
		TCCR1B |= (1<<CS12) | (1<<CS10);
		while (((TIFR1 & (1 << TOV1)) == 0) && (count < 5));
		TIFR1 |= (1<<TOV1);
	}
	count = 0;
	return 0;
}
int light(int y) /* turn on LED */
{
	DDRB = 0xff;
	PORTB = ~digit;
	timeDelay(y);
	return 0;
}
ISR(INT0_vect) /* interrupt that handles counting of cars */
{
	if (digit == 0x04)
		count ++;
}
ISR(INT1_vect) /* interrupts the loop to turn on the green light */
{
	int digit_save;
	digit_save = digit;
	digit = 0x04;
	light(0);
	digit = digit_save;
}
int main(void)
{
	DDRD = 0x00;
	EICRA |= (1<<ISC11) | (1<<ISC01);
	EIMSK |= (1<<INT1) | (1<<INT0);
	sei();
	while (1)
	{
		int i;
		digit = 0x04;
		for (i = 0; i < 3; i++)
		{
			light(i);
			digit>>=1;
		}
	}
}
