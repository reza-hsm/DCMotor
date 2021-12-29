#include <io.h>
#define F_CPU  1000000		//1 MHz
#include <delay.h>

#define increaseButtonPressed       !PIND.3
#define increaseButtonNotPressed    PIND.3
#define decreaseButtonPressed       !PIND.4
#define decreaseButtonNotPressed    PIND.4 
#define directionButtonPressed      !PIND.5
#define directionButtonNotPressed   PIND.5

void setPWM();
void PWMWrite(char value);
void setPins();

char pwmValue = 70;

void main(void)
{
    setPins();
    setPWM();
    PWMWrite(pwmValue);
    
    while (1) {
        if(increaseButtonPressed && decreaseButtonNotPressed && directionButtonNotPressed){
            delay_ms(50);
            while(increaseButtonPressed);
            delay_ms(50);
            if(pwmValue < 250)
                pwmValue += 5;
            else 
                pwmValue = 250;
            PWMWrite(pwmValue);
        }
        if(increaseButtonNotPressed && decreaseButtonPressed && directionButtonNotPressed){
            delay_ms(50);
            while(decreaseButtonPressed);
            delay_ms(50);
            if(pwmValue > 70)
                pwmValue -= 5;
            else
                pwmValue = 70;
            PWMWrite(pwmValue);
        }
        if(increaseButtonNotPressed && decreaseButtonNotPressed && directionButtonPressed){
            delay_ms(50);
            while(directionButtonPressed);
            delay_ms(50);
            PORTD.1 = !PORTD.1; //Toggle PD1
            PORTD.2 = !PORTD.2 ; //Toggle PD2
            //PORTD ^= 0b11<<1;
        }
    }
}

void setPWM(){
    ASSR = 0x00;  // Set external clock off(for timer2)
    TCCR2 = 0x69; // Prescaler : 1 , FastMode PWM , Non-Inverting Mode ,pwm freq : 62.5 KHz
    TCNT2 = 0x00; // Initial value of timer/counter2 
}

void PWMWrite(char value){
    OCR2 = value;
}

void setPins(){
    DDRD.1 = 1;        //Set PD1 as output (DDRD1 : 1)
    DDRD.2 = 1;        //Set PD2 as output (DDRD2 : 1)
    DDRD.3 = 0;    //Set PD3 as input (DDRD3 : 0)
    DDRD.4 = 0;    //Set PD4 as input (DDRD4 : 0)
    DDRD.5 = 0;    //Set PD5 as input (DDRD5 : 0)
    DDRD.7 = 1;        //Set PD7(OC2) as output (DDRD7 : 1)
    PORTD.3 = 1;        //Pull up PD3
    PORTD.4 = 1;        //Pull up PD4 
    PORTD.5 = 1;        //Pull up PD5 
    PORTD.2 = 1;        //Set PD2 1 
    PORTD.1 = 0;    //Set PD1 0
    
    //DDRD = 0xC4;
    //PORTD = 0x3C;    
}