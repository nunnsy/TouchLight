//#include <Arduino.h> // needed for main()//

//#include <ATtinySerialOut.h>

//#include <avr/pgmspace.h> // needed for P/STR()/

#include <CapacitiveSensor.h>

/*
   Uses a high value resistor e.g. 10M between send pin and receive pin.
   Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
   Receive pin is the sensor pin - try different amounts of foil/metal on this pin.
*/

/*
   Modified by Daniel Nunns, 2019.
   WIP.
*/


#define PIN_TOUCH_SENSOR    4
#define PIN_TOUCH_REFERENCE 3

#define PIN_LED_CONTROL     1

#define TOUCH_TOLERANCE     200

// 10M resistor between pins.
CapacitiveSensor cs = CapacitiveSensor(PIN_TOUCH_REFERENCE, PIN_TOUCH_SENSOR);

long avg = 0;

long newValue = 0;

bool pressed = false;
bool pressedBuffer = false;

#define BRIGHTNESS_LEVELS 4
uint8_t brightness[] = {0, 100, 200, 255};
uint8_t brightness_index = 0;

void setup() {
  //   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  //cs_4_2.set_CS_AutocaL_Millis(0xF);


  pinMode(PIN_LED_CONTROL, OUTPUT);
  analogWrite(PIN_LED_CONTROL, brightness[brightness_index]);

  avg = cs.capacitiveSensorRaw(30);
}

void loop() {
  newValue =  cs.capacitiveSensorRaw(30);

  if (newValue > avg + TOUCH_TOLERANCE) {
    // change state
    pressed = true;
  } else {
    pressed = false;
  }


  avg = (avg + newValue) / 2;

  if (pressed && !pressedBuffer) {
    // Trigger
    
    brightness_index += 1;
    if (brightness_index == BRIGHTNESS_LEVELS) {
      brightness_index = 0;
    }
    
    analogWrite(PIN_LED_CONTROL, brightness[brightness_index]);
  }

  pressedBuffer = pressed;

  delay(10);                             // arbitrary delay to limit data to serial port
}
