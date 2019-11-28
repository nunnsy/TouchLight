#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

 /*
  * Modified by Daniel Nunns, 2019.
  * WIP.
  */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

int LED = 9;

long avg = 0;

long newValue = 0;

long tolerance = 1000;

bool pressed = false;
bool pressedBuffer = false;

bool flip = false;
int brightness = 0;

void setup()                    
{
//   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
//cs_4_2.set_CS_AutocaL_Millis(0xF);
   Serial.begin(9600);
   pinMode(LED, OUTPUT);
   analogWrite(LED, 0);

   avg = cs_4_2.capacitiveSensorRaw(30);
}

void loop()                    
{
    long start = millis();
    newValue =  cs_4_2.capacitiveSensorRaw(30);

    if (newValue > avg + tolerance) {
      // change state
      pressed = true;
    } else {
      pressed = false;
    }

    
      avg = (avg + newValue) / 2;

//    Serial.print(millis() - start);        // check on performance in milliseconds
//    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.println(newValue);                  // print sensor output 1

    if (pressed && !pressedBuffer) {
      //trigger
//      flip = !flip;
      brightness += 85;
      if (brightness > 255) {
        brightness = 0;
      }
//      digitalWrite(LED, flip);
      analogWrite(LED, brightness);
    }

    pressedBuffer = pressed;

    delay(10);                             // arbitrary delay to limit data to serial port 
}
