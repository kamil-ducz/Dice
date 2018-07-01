#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t seg[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
volatile uint8_t i = 0; // Zwyk3a zmienna ale nieoptymalizowana przez kompilator
uint8_t choice[] = {~8, ~4, ~2, ~1};
volatile uint8_t data[] = {0, 0};
uint8_t przycisk = 0;

//przerwania wyzwalane sa przy przepelnieniu

ISR(TIMER0_OVF_vect) //przerwanie 1
{
	if(i++== 1) {
		i = 0;
	}
}

ISR(TIMER1_OVF_vect)  // przerwanie 2
{
	TCNT1 = 1000;		
}

int main(void)
{
	
	DDRA = 0xff;
	DDRB = 0xff;

	DDRC = 0; // przyciski
	PORTC = 0xff;
	
	TCCR1B = (1<<CS12);
	TCCR0 =  (1<<CS01);
	TIMSK =  (1<<TOIE1)|(1<<TOIE0); // przepelnienia licznik 1 i 2

	sei(); //globalne wlacznie przerwan 1 i 2

	while(1) {

		PORTA = seg[data[i]];
		PORTB = choice[i];

		if (!(PINC & (1<<PC0))) // 
		{				
			for (int i = 0; i < 25; i++)
			{	
				data[0] = rand()%6+1;
				data[1] = rand()%6+1;	
			}		
		}	
		if (!(PINC & (1<<PC1)))
		{
			data[0] = rand()%6+1;
			_delay_ms(20);
			data[1] = 0b00000000;
		}
	}

	return 0;
}
