
#include <avr/io.h>
#include <avr/interrupt.h>

#include "general.h"
#include "pwm_generator.h"
#include "color_calculator.h"


/* VARIABLES */
PWM *current_PWM;
RGBW temp;

//used to calculate calcPWM
PWM pwm1;
uint16_t smallestNum=0,lastsmallestNum=0, step_counter=0;
uint8_t port_counter=0, PWM_port_counter=0;

//used in HSVtoRGBW
uint16_t r, g, b, w, t, f;
long p,q,test;
uint8_t i2;



/* PROTOTYPES */
RGBW HSVtoRGBW(uint16_t h, uint16_t s, uint16_t v);
PWM* calcPWM(uint16_t* channels);


/* FUNCTIONS */
void init_color(){
  
}

void setHSV(uint16_t h, uint16_t s, uint16_t v){
  temp = HSVtoRGBW(h,s,v);
  uint16_t new_channels[5]={temp.R,temp.G,temp.B,temp.W, 16384 };
  //uint16_t new_channels[5]={0,0,0,1000, 16384 };

  //transforming a 12Bit to a 14Bit variable
  if(TOP_VALUE==16384){
    for(uint8_t i=0;i<5;i++)
      new_channels[i]=((uint32_t)new_channels[i]*4);
  }

  //scaling duty circle to the typical
  new_channels[0]=((long)new_channels[0]*TYP_DUTY_R)/100;
  new_channels[1]=((long)new_channels[1]*TYP_DUTY_G)/100;
  new_channels[2]=((long)new_channels[2]*TYP_DUTY_B)/100;
  new_channels[3]=((long)new_channels[3]*TYP_DUTY_W)/100;
  new_channels[4]=TOP_VALUE;

  /*new_channels[0]=0;
  new_channels[1]=0;
  new_channels[2]=0;
  new_channels[3]=((long)4095*TYP_DUTY_W)/100;*/

  current_PWM=(PWM*)calcPWM(new_channels);
  setPWM(current_PWM);
}

/*
	0,255,255 = Rot			  0 Grad
	43,255,255 = Gelb		~60 Grad
	64,255,255 = Gelbgrün	 90 Grad
	128,255,255 = Hellblau	180 Grad

	1 Grad = 0,7111 Bit

	Berechnung dauert 168Zyklen@O2 und genau 21us@8Mhz
*/
RGBW HSVtoRGBW(uint16_t h, uint16_t s, uint16_t v) {
  //float h_f = h/0xFFF,s_f=s/0xFFF,v_f=;
  r=g=b=v;

  //TODO: checken ob notwendig
  if(h>3959) 
    h=h%3960;

  i2 = h / 660;
  f = h % 660;
  p = w = (long)v*(4095-s)/4096;
  q = (v*((2698605L-(long)s*f)/659))/4096;
  t = ((long)v*((((long)2698605-(long)s*(659-f)))/659))/4096;
  //t    = ((long)v*(((long)2698605-(long)s*(659-f)))/659)/4096;

  switch (i2){
    case 0:        g = t; b = p;  break;
    case 1: r = q;        b = p;  break;
    case 2: r = p;        b = t;  break;
    case 3: r = p; g = q;         break;      
    case 4: r = t; g = p;         break;        
    case 5:        g = p; b = q;  break;
  }


  RGBW newRGBW = {r,g,b,w};
  return newRGBW;
}


//TODO: dauert zu lange
// 1543 Zyklen, 192,88us@8Mhz
PWM* calcPWM(uint16_t calc_channels[5]){
  /*uint16_t smallestNum=0,lastsmallestNum=0, step_counter=0;
  uint8_t port_counter=0, PWM_port_counter=0; */

  uint16_t calc_pwm_value[6];
  uint8_t calc_pwm_portB[6];
  uint8_t calc_pwm_portD[6];

  uint8_t first_round=TRUE;
  
  smallestNum=0;
  lastsmallestNum=0;
  step_counter=0;
  port_counter=0;
  PWM_port_counter=0;

  calc_pwm_value[0]=0;

  //first searching for the smallest number
  while(port_counter < 5){
    if(first_round==FALSE){
      for(int i=0;i<5;i++){
        if(smallestNum>calc_channels[i]&&lastsmallestNum<calc_channels[i]) smallestNum=calc_channels[i];
      }
    }

    //then calculating the ports of an compare interrupt      
    if(port_counter==0 && smallestNum!=0)PWM_port_counter++;
    if(smallestNum!=0){
      calc_pwm_value[PWM_port_counter-1]=smallestNum-step_counter;
      step_counter+=calc_pwm_value[PWM_port_counter-1];
    }
    calc_pwm_portB[PWM_port_counter]=0xFF;
	calc_pwm_portD[PWM_port_counter]=0xFF;
    for(int i=0;i<5;i++){
      if(calc_channels[i]>=smallestNum && calc_channels[i] <= smallestNum+50){
        port_counter++;
        switch(i){
          case 0: calc_pwm_portD[PWM_port_counter]&=~PORT_R; break;
		  case 1: calc_pwm_portD[PWM_port_counter]&=~PORT_G; break;
		  case 2: calc_pwm_portD[PWM_port_counter]&=~PORT_B; break;
		  case 3: calc_pwm_portB[PWM_port_counter]&=~PORT_W; break;
		  case 4: calc_pwm_portB[PWM_port_counter]&=~PORT_L; break;
        }
      }
    }
    PWM_port_counter++;
    lastsmallestNum=smallestNum+50;
    smallestNum=TOP_VALUE;

	if(first_round==TRUE){
      first_round=FALSE;
      if(port_counter==0)
        PWM_port_counter--;
    }
  }
    
  if(step_counter<TOP_VALUE-50){
    calc_pwm_value[PWM_port_counter-1]=TOP_VALUE-step_counter;
  }
  else{
    calc_pwm_value[PWM_port_counter-2]+=TOP_VALUE-step_counter;
  }

  /*pwm1.value=&pwm_value[0];
  pwm1.portB=&pwm_portB[0];
  pwm1.portD=&pwm_portD[0];*/

  //TODO: optimieren, nur nötige Elemente kopieren oder wieder mit pointern arbeiten
  for(int i=0;i<6;i++){
    pwm1.value[i]=calc_pwm_value[i];
    pwm1.portB[i]=calc_pwm_portB[i];
    pwm1.portD[i]=calc_pwm_portD[i];
  }

  pwm1.compare_count=PWM_port_counter-1;

  return &pwm1;
}
