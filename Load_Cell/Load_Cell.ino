/*This code was written for an arduino teensy communicating to a load cell amplifier over an i2c interface to a sparkfun HX711 load cell amplifier
 * The setup code runs an auto-calibration script to obtain a calibration constant for the load cell
 * The loop simply prints the force value to the serial bus on repeat
 */

#include "HX711.h"

#define DOUT  18
#define CLK  19

//change the following constant to match the mass of your calibration weight
const float known_weight = 3083.0; //grams

HX711 scale;

float calibration_factor;

void setup() {
  Serial.begin(9600);
  Serial.println("Load cell must be unloaded for the initialization process to begin.  Remove any weight and press any key to continue");

  while(!Serial.available()) {
    delay(10);
  }
  (void) Serial.readString();
  
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();

  delay(100);
  Serial.println("Load cell initialized.  Add the calibration weight then press any key to continue");

  while(!Serial.available()) {
    delay(10);
  }
  (void) Serial.readString();
  
  float initial_reading = scale.get_units(10);
  Serial.print("Initial raw reading: ");
  Serial.println(initial_reading);

  calibration_factor = initial_reading / known_weight;
  Serial.print("Corresponding calibration factor:");
  Serial.println(calibration_factor, 10);

  scale.set_scale(calibration_factor);
  Serial.print("New reading, with calibration factor: ");
  Serial.println(scale.get_units(10));

  Serial.println("Manually enter a float to adjust the calibration factor.  Alternatively, press 'e' to end calibration with the current factor and begin reporting data");

  while(true) {
    while(!Serial.available()) {
      delay(10);
    }
    
    String input = Serial.readString();
    Serial.println(input);
    if (input.charAt(0) == 'e') {
      break;
    }

    calibration_factor = input.toFloat();
    scale.set_scale(calibration_factor);
    Serial.print("New reading, with current calibration factor: ");
    Serial.println(scale.get_units(10));
  }
}

void loop() {
  Serial.println(scale.get_units(), 4); //scale.get_units() returns a float
}
