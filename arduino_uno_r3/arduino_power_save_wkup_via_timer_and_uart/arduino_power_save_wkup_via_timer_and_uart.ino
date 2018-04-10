/*
   Sketch for testing sleep mode with wake up on WDT.
   Donal Morrissey - 2011.

*/
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)
#define SOIL_MOISTURE (A1)
#define PUMP_SWITCH (A2)
volatile int f_wdt = 1;



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
    Serial.println("WDT Overrun!!!");
  }
}


/***************************************************
    Name:        enterSleep

    Returns:     Nothing.

    Parameters:  None.

    Description: Enters the arduino into sleep mode.

 ***************************************************/
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();

  /* Now enter sleep mode. */
  sleep_mode();

  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();
}



/***************************************************
    Name:        setup

    Returns:     Nothing.

    Parameters:  None.

    Description: Setup for the serial comms and the
                  Watch dog timeout.

 ***************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.

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

  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
}



/***************************************************
    Name:        enterSleep

    Returns:     Nothing.

    Parameters:  None.

    Description: Main application loop.

 ***************************************************/
void loop()
{
  if (f_wdt == 1)
  {
    /* Toggle the LED */
    int soil_moisture_value = analogRead(SOIL_MOISTURE);
    Serial.println("soil_moisture_value"); 
    Serial.println(soil_moisture_value);
    if (soil_moisture_value >= 900) { //表示土壤太干,需要浇水
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(PUMP_SWITCH, HIGH);
      delay(10 * 1000); //睡眠5秒
      digitalWrite(PUMP_SWITCH, LOW);
    }
    /* Don't forget to clear the flag. */
    f_wdt = 0;
    digitalWrite(LED_PIN, LOW);
    /* Re-enter sleep mode. */
    enterSleep();
  }
  else
  {
    /* Do nothing. */
  }
}
