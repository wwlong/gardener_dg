#include <avr/sleep.h>
#include <avr/power.h>

const byte AWAKE_LED = 8;
const byte GREEN_LED = 9;
const unsigned long WAIT_TIME = 5000;

ISR (PCINT2_vect)
{
  // handle pin change interrupt for D0 to D7 here
}  // end of PCINT2_vect

void setup() 
{
  pinMode (GREEN_LED, OUTPUT);
  pinMode (AWAKE_LED, OUTPUT);
  digitalWrite (AWAKE_LED, HIGH);
  Serial.begin (9600);
} // end of setup

unsigned long lastSleep;

void loop() 
{
  if (millis () - lastSleep >= WAIT_TIME)
  {
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
  Serial.println("hahaha\n");
  if (Serial.available () > 0)
  {
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
}  // end of loop

