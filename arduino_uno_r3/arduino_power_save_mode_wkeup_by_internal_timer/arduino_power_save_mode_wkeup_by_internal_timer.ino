/*
   Sketch for testing sleep mode with wake up on timer.
   Donal Morrissey - 2011.

*/
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)
#define SOIL_MOISTURE (A1)
#define PUMP_SWITCH (A2)
volatile int f_timer = 0;

/***************************************************
    Name:        ISR(TIMER1_OVF_vect)

    Returns:     Nothing.

    Parameters:  None.

    Description: Timer1 Overflow interrupt.

 ***************************************************/
ISR(TIMER1_OVF_vect)
{
  /* set the flag. */
  if (f_timer == 0)
  {
    f_timer = 1;
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
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  /* Disable all of the unused peripherals. This will reduce power
     consumption further and, more importantly, some of these
     peripherals may generate interrupts that will wake our Arduino from
     sleep!
  */
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();
  //
  //power_all_disable();
  /* Now enter sleep mode. */
  sleep_mode();

  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();
}



/***************************************************
    Name:        setup

    Returns:     Nothing.

    Parameters:  None.

    Description: Setup for the serial comms and the
                  timer.

 ***************************************************/
void setup()
{
  Serial.begin(9600);

  //Serial.println("setup");
  /* Don't forget to configure the pin! */
  pinMode(LED_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE, INPUT);
  pinMode(PUMP_SWITCH, OUTPUT);
  /*** Configure the timer.***/

  /* Normal timer operation.*/
  TCCR1A = 0x00;

  /* Clear the timer counter register.
     You can pre-load this register with a value in order to
     reduce the timeout period, say if you wanted to wake up
     ever 4.0 seconds exactly.
  */
  TCNT1 = 0x0000;

  /* Configure the prescaler for 1:1024, giving us a
     timeout of 4.09 seconds.
  */
  TCCR1B = 0x05;

  /* Enable the timer overlow interrupt. */
  TIMSK1 = 0x01;
}

/***************************************************
    Name:        enterSleep

    Returns:     Nothing.

    Parameters:  None.

    Description: Main application loop.

 ***************************************************/
/*
  状态机 for 浇水系统
  STATE1:init
  STATE2  : 获取土壤湿度传感器的数据并通过UART(GX-xUART)上传到SERVER,等待Response
  STATE3  : 接收来自Server的command
*/

typedef enum {
  STATE1 = 1,
  STATE2 = 2, //
  STATE3 = 3
} STATES;
//init在setup中完成
STATES current_state = 2;
void loop()
{
  switch (current_state) {
    case STATE2: {
        break;
      };
    case STATE3: {
        break;
      };
    default: {
        Serial.println("error");
      }
  }
  if (f_timer == 1)
  {
    f_timer = 0;
    /* Toggle the LED */
    //digitalWrite(LED_PIN, HIGH);
    int soil_moisture_value = analogRead(SOIL_MOISTURE);
    Serial.println("soil_moisture_value");
    Serial.println(soil_moisture_value);
    if (soil_moisture_value >= 900) { //表示土壤太干,需要浇水
      digitalWrite(PUMP_SWITCH, HIGH);
      digitalWrite(LED_PIN, HIGH);
      delay(1000 * 2); //睡眠5秒
      digitalWrite(PUMP_SWITCH, LOW);
    }
    digitalWrite(LED_PIN, LOW);
    /* Re-enter sleep mode. */
    //Serial.println("before sleep");
    enterSleep();
    //Serial.println("after sleep");
  }
}
