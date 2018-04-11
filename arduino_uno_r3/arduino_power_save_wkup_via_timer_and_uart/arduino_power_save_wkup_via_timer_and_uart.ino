//在此基础上增加WTD唤醒
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)
#define SOIL_MOISTURE (A1)
#define PUMP_SWITCH (A2)
volatile int f_wdt = 1;
const byte AWAKE_LED = 8;
const byte GREEN_LED = 9;
const unsigned long WAIT_TIME = 5000;

ISR (PCINT2_vect)
{
  // handle pin change interrupt for D0 to D7 here
}  // end of PCINT2_vect

/***************************************************
    Name:        ISR(WDT_vect)

    Returns:     Nothing.

    Parameters:  None.

    Description: Watchdog Interrupt Service. This
                 is executed when watchdog timed out.

 ***************************************************/
ISR(WDT_vect)
{
  if (f_wdt == 0)
  {
    f_wdt = 1;
  }
  else
  {
    //Serial.println("WDT Overrun!!!");
  }
}

void setup() 
{
  pinMode (GREEN_LED, OUTPUT);
  pinMode (AWAKE_LED, OUTPUT);
  //digitalWrite (AWAKE_LED, HIGH);
  Serial.begin (9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE, INPUT);
  pinMode(PUMP_SWITCH, OUTPUT);
    /*** Setup the WDT ***/

  /* Clear the reset flag. */
  MCUSR &= ~(1 << WDRF);

  /* In order to change WDE or the prescaler, we need to
     set WDCE (This will allow updates for 4 clock cycles).
  */
  WDTCSR |= (1 << WDCE) | (1 << WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1 << WDP0 | 1 << WDP3; /* 8.0 seconds */

  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
} // end of setup

unsigned long lastSleep;

void loop() 
{
  if (millis () - lastSleep >= WAIT_TIME)
  {
    f_wdt = 0;
    lastSleep = millis ();

    noInterrupts ();

    byte old_ADCSRA = ADCSRA;
    // disable ADC
    ADCSRA = 0;  
    // pin change interrupt (example for D0)
    PCMSK2 |= bit (PCINT16); // want pin 0
    PCIFR  |= bit (PCIF2);   // clear any outstanding interrupts
    PCICR  |= bit (PCIE2);   // enable pin change interrupts for D0 to D7

    set_sleep_mode (SLEEP_MODE_PWR_DOWN);  
    power_adc_disable();
    power_spi_disable();
    power_timer0_disable();
    power_timer1_disable();
    power_timer2_disable();
    power_twi_disable();

    UCSR0B &= ~bit (RXEN0);  // disable receiver
    UCSR0B &= ~bit (TXEN0);  // disable transmitter

    sleep_enable();
    digitalWrite (AWAKE_LED, LOW);
    interrupts ();
    sleep_cpu ();      
    digitalWrite (AWAKE_LED, HIGH);
    sleep_disable();
    power_all_enable();

    ADCSRA = old_ADCSRA;
    PCICR  &= ~bit (PCIE2);   // disable pin change interrupts for D0 to D7
    UCSR0B |= bit (RXEN0);  // enable receiver
    UCSR0B |= bit (TXEN0);  // enable transmitter
  }  // end of time to sleep
  if (Serial.available () > 0)  //server通过UART唤醒设备的
  {
    Serial.println("WDT Overrun!!!");
    byte flashes = Serial.read () - '0';
    if (flashes > 0 && flashes < 10)
      {
      // flash LED x times 
      for (byte i = 0; i < flashes; i++)
        {
        digitalWrite (GREEN_LED, HIGH);
        delay (200);  
        digitalWrite (GREEN_LED, LOW);
        delay (200);  
        }
      }        
  }  // end of if
  else if(1 == f_wdt) {
      Serial.println("WTD wakeup");
      f_wdt = 0;
    }
}  // end of loop

