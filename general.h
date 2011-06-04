
#ifndef _GENERAL_
#define _GENERAL_

#define TRUE 0x01
#define FALSE 0x00

#define CPU_CLOCK 8000000L
#define TOP_VALUE 16384			//4095

#define PWM_DURATION 128		//in microseconds


/* STRUCTS */
/* saves one HSV Value */
typedef struct{
  uint16_t H,S,V;
} HSV;

typedef struct{
  uint16_t R,G,B,W;
} RGBW;


#endif
