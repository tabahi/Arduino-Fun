/*
 * GccApplication1.c
 *
 * Created: 21-Aug-16 6:55:06 PM
 * Author : Abdul Rehman
 */ 

#define F_CPU 1000000UL  // 1 MHz - set the clock speed here if its not 1Mhz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/wdt.h>


//Pin 5 = Pin PB0

// Wake up by WDT interrupt.
// Don't need to do anything here but (auto-) clearing the interrupt flag.
EMPTY_INTERRUPT(WDT_vect)



//Delay in powerdown mode. Wake up by watchdog interrupt.
void delay_power_down_wdt(uint8_t wdto)
{
  wdt_reset();
  wdt_enable(wdto);
  //WDTCSR |= (1 << WDIE);	//for 1 second delay
  WDTCSR = (1<<WDIE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0); //2 second delay
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  // Make sure interrups are enabled and the I flag is restored
  NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
  {
    sleep_cpu();
    wdt_disable();
  }
  sleep_disable();
}



int main(void)
{
	//B0:Led output
	//B1: INT1, push button
	DDRB = 0b00000000;		// Set PB1 as input (Using for interupt INT1)
	PORTB = 0b00000010;		// Enable PB1 pull-up resistor
	
	
	GICR = 1<<INT1;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge

  sei();	//global interrupt enable

  ADCSRA = 0;  //disable ADC

  while (1)
  {
    DDRB = 0b00000001; // PB0 output

    PORTB |= 1;			//2ms flash
    _delay_ms(2);
    PORTB &= ~1;


    DDRB = 0b00000000; //set Port B as input


    delay_power_down_wdt(WDTO_2S);
  }
}
