
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "general.h"
#include "pwm_generator.h"
#include "color_calculator.h"
#include "fader.h"

int main(void) {
  init_pwm();
  init_fader();

  //teste HSV Methode
  //Rot (0,255,X):
  //	V= 50, 16% duty und  12mA
  //	V=100, 30% duty und  23mA
  //	V=255, 71% duty und 597mA
  //runHSV(0,4095,4095);	//rot
  //runHSV(1320,4095,4095);	//grün
  //runHSV(2640,4095,4095);	//blau
  runHSV(0,0,1000);		//weiß

  //teste Slot Animation
  /*HSV_anim anim1 = {0,4095,4095,+3960,0,0,500};
  runSlot(anim1);*/

  //teste Program Animation
  /*newProgram();  //FLASH
  HSV_anim anim2 = {0,4095,0,0,0,3000,500};
  setSlot(anim2);
  HSV_anim anim3 = {1320,4095,0,0,0,3000,500};
  setSlot(anim3);
  HSV_anim anim4 = {2640,4095,0,0,0,3000,500};
  setSlot(anim4);
  runProgram();*/

  /*newProgram();  //pulsierendes Weiß
  HSV_anim anim2 = {0,0,10,0,0,1010,100};
  setSlot(anim2);
  HSV_anim anim3 = {0,0,1010,0,0,-1000,100};
  setSlot(anim3);
  runProgram();*/

  sei();
  while(1){
    faderThread();
  }
} 
