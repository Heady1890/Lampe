
#ifndef _FADER_
#define _FADER_

#define COUNTER_10ms_8Mhz 312
#define COUNTER_10ms_20Mhz 781


//#define MAX_PROG 2

#define MODE_SLOT 1
#define MODE_PROG 2


/*
  HSV_anim	animation

  H,S,V		are the starting values (0, 4095)
  deltas	are the changes made in an specific time (-32768, +32767)
  			this are 8 rotations around the H axis
  time		is the duration of the animation in 10 milliseconds (1-65535)
  			you can choose between 10ms up to 655,35s which is 10,9 minutes
  */
/*typedef struct{
  uint8_t H,S,V;
  short H_delta, S_delta, V_delta;
  uint16_t time;
} HSV_anim;*/

typedef struct{
  uint16_t H,S,V;
  short H_delta, S_delta, V_delta;
  uint16_t time;
} HSV_anim;

typedef struct{
  uint8_t slot_count;
  HSV_anim next_slot[5];
} program;


extern void init_fader(void);
extern void faderThread(void);
extern void newProgram(void);
extern void setSlot(HSV_anim anim);
extern void runSlot(HSV_anim anim);
extern void runProgram();
extern void runHSV(uint16_t h, uint16_t s, uint16_t v);

#endif
