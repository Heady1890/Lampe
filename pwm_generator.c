
/* INCLUDES */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm_generator.h"
#include "general.h"


/* VARIABLES */
uint8_t compare_counter=0, compare_count=0, next_compare_count=0;

uint16_t pwm_value1[6];
uint8_t pwm_portB1[6];
uint8_t pwm_portD1[6];

uint16_t pwm_value2[6];
uint8_t pwm_portB2[6];
uint8_t pwm_portD2[6];

uint16_t pwm_value_error[2]={100,1948};
uint8_t pwm_portB_error[2]={0xFF,0xFE};
uint8_t pwm_portD_error[2]={0xFF,0x1F};

uint16_t *pwm_value=&pwm_value1;
uint8_t *pwm_portB=&pwm_portB1;
uint8_t *pwm_portD=&pwm_portD1;

uint8_t pwm_pointer=0;
uint16_t sum;

uint8_t new_PWM = FALSE;

/* INTERRUPTS */

/*
  Rechenzeit:
  - neuen PWM setzen		82Zyklen
  - letzter Interrupt		61Zyklen
  - normaler Interrupt		49Zyklen
  */
ISR(TIMER1_OVF_vect){
  //last interrupt
  if(compare_counter==compare_count){

    //if a new PWM should be displayed, then the pointer on the array will be switched
    if(new_PWM==TRUE){
	  if(pwm_pointer==0){  
        pwm_value=&pwm_value1;
		pwm_portB=&pwm_portB1;
		pwm_portD=&pwm_portD1;
	  }
	  else if(pwm_pointer==1){
	    pwm_value=&pwm_value2;
		pwm_portB=&pwm_portB2;
		pwm_portD=&pwm_portD2;
	  }
	  else{
        pwm_value=&pwm_value_error;
		pwm_portB=&pwm_portB_error;
		pwm_portD=&pwm_portD_error;
		next_compare_count=2;
	  }
	  compare_count=next_compare_count;
      
	  new_PWM=FALSE;
	}

    PORTD |= 0xE0&pwm_portD[0];
    PORTB |= 0x03&pwm_portB[0];

    compare_counter=0;
  }
  //normal interrupt
  else{
    PORTD &= pwm_portD[compare_counter];
    PORTB &= pwm_portB[compare_counter];
  }

  TCNT1=0xFFFF-pwm_value[compare_counter]+40;
  compare_counter++;  
}


/* FUNCTIONS */
void init_pwm(){
  DDRB |= 0x03;
  DDRD |= 0xE0;
 
  TCCR1B |= 0x01;
  TIMSK1 |= 0x01;
}

void setPWM(PWM* pwm){
  next_compare_count=pwm->compare_count;

  sum=0;

  if(pwm_pointer==0){
    pwm_pointer=1;
	for(int i=0;i<next_compare_count;i++){
      pwm_value2[i]=pwm->value[i];
	  pwm_portB2[i]=pwm->portB[i];
	  pwm_portD2[i]=pwm->portD[i];

	  sum+=pwm_value2[i];
	  if( pwm_value2[i]<50 || (pwm_portB2[i]|0x03)!=0xFF || (pwm_portD2[i]|0xE0)!=0xFF)
	    pwm_pointer=2;
	}
	if(sum!=TOP_VALUE)pwm_pointer=2;
    
  }
  else{
    pwm_pointer=0;
    for(int i=0;i<next_compare_count;i++){
      pwm_value1[i]=pwm->value[i];
	  pwm_portB1[i]=pwm->portB[i];
	  pwm_portD1[i]=pwm->portD[i];

	  sum+=pwm_value1[i];
	  if( pwm_value1[i]<50 || (pwm_portB1[i]|0x03)!=0xFF || (pwm_portD1[i]|0xE0)!=0xFF)
	    pwm_pointer=2;
	}
	if(sum!=TOP_VALUE)pwm_pointer=2;
  }
  new_PWM=TRUE;
}
