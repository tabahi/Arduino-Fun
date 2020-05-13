#include <TimerOne.h>



int pwm_pin = 10;
unsigned long t = 500; //us
unsigned int duty = 512;

void setup()
{

  Serial.begin(115200);    
  Timer1.initialize(t);
  Timer1.pwm(pwm_pin, duty, t); 
  
  Serial.println("Start!");

}

void loop()
{
  delay(2000);
  t = t*100;
  
  if(t>100000)
  {
   t = 0; 
  }
  Timer1.pwm(pwm_pin, duty, t);

}
