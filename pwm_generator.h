

#ifndef _PWM_GEN_
#define _PWM_GEN_

#define TOLERANCE 50

/*
  ROT
  	

  */

#define PORT_R 0x20
#define PORT_G 0x80
#define PORT_B 0x40
#define PORT_W 0x01
#define PORT_L 0x02

//the absolute maximum duty cycle
#define MAX_DUTY_R 87
#define MAX_DUTY_G 98
#define MAX_DUTY_B 98
#define MAX_DUTY_W 98
#define MAX_DUTY_L 100

//the typical duty cycle 
#define TYP_DUTY_R 72 //72@650mA
#define TYP_DUTY_G 30 //30@690mA, 33@780mA
#define TYP_DUTY_B 28 //25@650mA, 28@750mA, 30@800mA
#define TYP_DUTY_W 30 //90
#define TYP_DUTY_L 100

typedef struct _PWM{
  uint16_t value[6];
  uint8_t portB[6];
  uint8_t portD[6];
  uint8_t compare_count;
} PWM;

extern void init_pwm(void);
extern void setPWM(PWM* pwm);

#endif
