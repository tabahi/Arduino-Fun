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


/*
  Delay in powerdown mode. Wake up by watchdog interrupt.
*/
void delay_power_down_wdt(uint8_t wdto)
{
  wdt_reset();
  wdt_enable(wdto);
  //WDTCSR |= (1 << WDIE);
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
  //DDRB = 1 << 2; // PB2 output


  sei();

  //PINB = 1<<2; //Toggle PB2
  
  ADCSRA = 0;  //disable ADC

  while (1)
  {
    DDRB = 1; // PB0 output
    // Short flash
    PORTB |= 1;
    _delay_ms(2);
    PORTB &= ~1;
    DDRB = 0; //set Port B as input


    delay_power_down_wdt(WDTO_1S);
  }
}
