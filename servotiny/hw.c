/*
 * hw.c
 *
 * Created: 13.12.2019 14:55:55
 *  Author: Legkiy
 */

#include <stdbool.h>
#include "common.h"

volatile daytime uptime;

//sleep flag shows if nothing happens until last pwr_dn increment
volatile bool f_sleep = true;

ISR(TIM0_OVF_vect)
{
	static uint8_t one_in_fifty = 0;
	one_in_fifty++;
	if (!f_sleep){//reset pwr_dn counter when some actions executed
		f_sleep = true;
		uptime.pwr_dn = 0;
	}
	if (one_in_fifty == 50){
		uptime.glob_sec++;
		uptime.sec++;
		uptime.pwr_dn++;
		one_in_fifty = 0;
	}
	if (uptime.sec == 60){
		uptime.sec = 0;
		uptime.min++;
	}
	if (uptime.min == 60){
		uptime.min = 0;
		uptime.hour++;
	}
	if (uptime.hour == 60){
		uptime.hour = 0;
		uptime.day++;
	}
	if (uptime.pwr_dn == 300){ //power down after ~5 mins
		uptime.pwr_dn = 0;
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	}
}

/*
 *	Pin change interrupt handler
 */
ISR(PCINT0_vect)
{
	/* wake up */
	sleep_disable();
	set_sleep_mode(SLEEP_MODE_IDLE);
	f_sleep = false;

	//timer value should change here
	servo_pwm_select();
}

void init_port()
{
	MCUCR |= (1<<PUD);
	DDRB = 0xff;PORTB = 0x00;
	/* Configure PWMPIN as output to generate pwm */
	DDRB |= _BV(PWMPIN);
	/* Turn on input pin */
	DDRB &= ~(_BV(INPIN));
}

/*
 * Timer set on phase correct dual-slope PWM. TOP value is 188.
 * Generated freq is 50 Hz with ~100us/tick resolution
 */
void init_tim()
{	
	/* TOP counter value 375/2=187.5 (150000/8/375=50Hz) */
	/* Timer clock = I/O clock / 8 = 18750 */
	TCCR0A = (1<<COM0B1)|(1<<WGM00);
	TCCR0B = (1<<CS01)|(1<<WGM02);
	/* Output compare register A is TOP value */
	OCR0A = 189; //value differs depending on each attiny13
	/*
	 *	178, 189
	 */
	/* Output compare register B is pulse width value */
	// 100us LSb
	servo_pwm_select();
	/* Clear overflow flag */
	TIFR0 = 1<<TOV0;
	/* Enable Overflow Interrupt */
	TIMSK0 = (1<<TOIE0);
}

void Configure_Interrupt(uint8_t INT_MODE)
{
	switch(INT_MODE)
	{
		case 0:MCUCR=(MCUCR&(~(1<<ISC01|1<<ISC00)))|(0<<ISC01|0<<ISC00);
		break;
		case 1:MCUCR=(MCUCR&(~(1<<ISC01|1<<ISC00)))|(0<<ISC01|1<<ISC00);
		break;
		case 2:MCUCR=(MCUCR&(~(1<<ISC01|1<<ISC00)))|(1<<ISC01|0<<ISC00);
		break;
		case 3:MCUCR=(MCUCR&(~(1<<ISC01|1<<ISC00)))|(1<<ISC01|1<<ISC00);
		break;
		default:break;
	}
}

void Enable_Interrupt()
{
	//GIMSK |= (1<<INT0);
	GIMSK |= (1<<PCIE);
	PCMSK |= (1<<PCINT3);
}

void Disable_Interrupt()
{
	//GIMSK = (GIMSK&(~(1<<INT0)));
	GIMSK = (GIMSK&(~(1<<PCIE)));
	PCMSK = (PCMSK&(~(1<<PCINT3)));
}

void servo_pwm_select(){
	if (PINB & (1<<INPIN)){
		OCR0B = S_OPEN;
	} else {
		OCR0B = S_CLOSE;
	}
}

void hw_init()
{
	/* Set CPU clk prescaler 64 */
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS2)|_BV(CLKPS1);
	init_port();
	Enable_Interrupt();
	init_tim();
	/* disable adc */
	ADCSRA &= ~(1<<ADEN);
	/* analog comparator disable */
	ACSR |= (1<<ACD);
	ACSR &= ~(1<<ACI);
	set_sleep_mode(SLEEP_MODE_IDLE);
	sei();
}