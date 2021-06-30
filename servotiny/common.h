/*
 * common.h
 *
 * Created: 13.12.2019 14:51:29
 *  Author: Legkiy
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

/*
 *	Cpu defaults
 */
#define DEFAULT_OSC 9600000UL
#define CPU_FREQ	(DEFAULT_OSC/8)

/*
 *	Pin config
 */
#define INPIN	3
#define PWMPIN	1
#define OUTPIN1 1
#define OUTPIN2 4

/*
 *	Servo macro
 */
#define S_ANGLE_MAX	0x18
#define S_ANGLE_MIN	0x06
#define S_CLOSE 0x12	//equals 1.8 ms
#define S_OPEN	0x0a	//equals 1 ms

#define RC_ENABLE  true
#define RC_DISABLE false
#define COUNTDOWN true
#define WOKEN false

/* timer */
typedef struct {
	uint32_t glob_sec;       // uptime in secounds
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint16_t pwr_dn;
	uint16_t rc_en;		// rc enable counter
} daytime;


/*
 *	hw.c prototypes
 */
void init_port(void);
void init_tim(void);

void Configure_Interrupt(uint8_t INT_MODE);
void Enable_Interrupt(void);
void Disable_Interrupt(void);

void servo_pwm_select(void);
void hw_init(void);
void remote_en(void);

#endif /* COMMON_H_ */
