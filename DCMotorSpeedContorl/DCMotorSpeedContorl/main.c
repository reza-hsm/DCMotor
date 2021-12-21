/*
 * DCMotorSpeedControl.c
 *
 * Created: 12/21/2021 10:14:31 PM
 * Author : Reza Hesami
 */ 

#include <avr/io.h>
#define F_CPU  1000000		//1 MHz
#include <util/delay.h>

#define increaseButtonPressed		!(PIND & (1<<3))
#define increaseButtonNotPressed	(PIND & (1<<3))
#define decreaseButtonPressed		!(PIND & (1<<4))
#define decreaseButtonNotPressed	(PIND & (1<<4))
#define directionButtonPressed		!(PIND & (1<<5))
#define directionButtonNotPressed   (PIND & (1<<5))

void setPWM();
void PWMWrite(uint8_t value);
void setPins();
uint8_t pwmValue = 70;
int main(void)
{
	setPins();
	setPWM();
	PWMWrite(pwmValue);
	
	while (1) {
		if(increaseButtonPressed && decreaseButtonNotPressed && directionButtonNotPressed){
			_delay_ms(50);
			while(increaseButtonPressed);
			_delay_ms(50);
			if(pwmValue <= 249)
				pwmValue += 5;
			else 
				pwmValue = 250;
			PWMWrite(pwmValue);
		}
		if(increaseButtonNotPressed && decreaseButtonPressed && directionButtonNotPressed){
			_delay_ms(50);
			while(decreaseButtonPressed);
			_delay_ms(50);
			if(pwmValue >= 70)
				pwmValue -= 5;
			else
				pwmValue = 70;
			PWMWrite(pwmValue);
		}
		if(increaseButtonNotPressed && decreaseButtonNotPressed && directionButtonPressed){
			_delay_ms(50);
			while(directionButtonPressed);
			_delay_ms(50);
			PORTD ^= 1<<1; //toggle PD1
			PORTD ^= 1<<2; //toggle PD2
			//PORTD ^= 0b11<<1;
		}
    }
}

void setPWM(){
	ASSR = 0x00;  // set external clock off(for timer2)
	TCCR2 = 0x69; // Prescaler : 1 , FastMode PWM , Non-Inverting Mode ,pwm freq : 62.5 KHz
	TCNT2 = 0x00; // Initial value of timer/counter2 
}
void PWMWrite(uint8_t value){
	OCR2 = value;
}
void setPins(){
	DDRD |= 1<<1;		//set PD1 as output (DDRD1 : 1)
	DDRD |= 1<<2;		//set PD2 as output (DDRD2 : 1)
	DDRD &= ~(1<<3);	//set PD3 as input (DDRD3 : 0)
	DDRD &= ~(1<<4);	//set PD4 as input (DDRD4 : 0)
	DDRD &= ~(1<<5);	//set PD5 as input (DDRD5 : 0)
	DDRD |= 1<<7;		//set PD7(OC2) as output (DDRD7 : 1)
	PORTD |= 1<<3;		//pull up PD3
	PORTD |= 1<<4;		//pull up PD4 
	PORTD |= 1<<5;		//pull up PD5 
	PORTD |= 1<<2;		//set PD2 1 
	PORTD &= ~(1<<1);	//set PD1 0
	
	//DDRD = 0xC4;
	//PORTD = 0x3C;	
}