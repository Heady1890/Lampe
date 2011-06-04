
#include <avr/io.h>
#include <avr/interrupt.h>

#include "general.h"
#include "color_calculator.h"
#include "pwm_generator.h"
#include "fader.h"


//slot* slots[10];
HSV_anim current_slot={0,0,0,0,0,0,10};

program prog;
program current_prog;
//uint8_t current_prog_id=0;

uint8_t new_value=FALSE,run_thread=FALSE;
uint16_t anim_counter;
uint8_t slot_counter;

uint8_t mode;

//thread
uint16_t h,s,v;

//interrupt
uint16_t timer_counter,timer_count;


ISR(TIMER0_OVF_vect){
  if(timer_counter==timer_count){
    timer_counter=0;
	new_value=TRUE;
  }
  timer_counter++;
}

void init_fader(){
  TCCR0B |= 0x01;
  TIMSK0 |= 0x01;
  timer_count=COUNTER_10ms_8Mhz;
}


void faderThread(){
  if(run_thread==TRUE && new_value==TRUE){
    new_value=FALSE;
    
	if(anim_counter>=current_slot.time){
      anim_counter=0;
	  if(mode==MODE_PROG){
	    slot_counter++;
        if(slot_counter>=prog.slot_count)
		  slot_counter=0;
        current_slot=prog.next_slot[slot_counter];
	  }
	}

    //TODO check if short cast is alright, maybe overflow
    h=current_slot.H+((long)current_slot.H_delta*anim_counter)/(short)current_slot.time;
    s=current_slot.S+((long)current_slot.S_delta*anim_counter)/(short)current_slot.time;
	v=current_slot.V+((long)current_slot.V_delta*anim_counter)/(short)current_slot.time;

    setHSV(h,s,v);
	anim_counter++;
  }
}


void setSlot(HSV_anim anim){
  prog.next_slot[prog.slot_count]=anim;
  prog.slot_count++;
}

void newProgram(){
  prog.slot_count=0;
}

void runSlot(HSV_anim anim){
  mode=MODE_SLOT;
  run_thread=TRUE;
  current_slot=anim;

  h=current_slot.H;
  s=current_slot.S;
  v=current_slot.V;
  setHSV(h,s,v);
  timer_counter=0;
  anim_counter=1;
}

void runProgram(){
  mode=MODE_PROG;
  run_thread=TRUE;
  current_prog = prog;

  current_slot=prog.next_slot[0];
  h=current_slot.H;
  s=current_slot.S;
  v=current_slot.V;
  setHSV(h,s,v);
  timer_counter=0;
  anim_counter=1;
}

//TODO: interrupt deaktivieren
void runHSV(uint16_t h, uint16_t s, uint16_t v){
  run_thread=FALSE;
  setHSV(h,s,v);
}

void runWhite(uint16_t brightness){
  run_thread=FALSE;
  setHSV(0,0,brightness);
}
