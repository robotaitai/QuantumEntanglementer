#include <Arduino.h>

const int sensorPins[] = {A0, A1, A2, A3, A4};
const int numSensors = 5;
const int numReadings = 10;

int readings[numSensors][numReadings];  // Multi-dimensional array to hold last 10 readings for each sensor
int readIndex = 0;                      // The index of the current reading
int total[numSensors];                  // The running total of readings for each sensor
int average[numSensors];                // The average of the readings for each sensor

int ledPin = 13; // LED pin

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
  // Initialize all the readings to 0 and totals to 0
  for (int i = 0; i < numSensors; i++) {
    total[i] = 0;
    for (int j = 0; j < numReadings; j++) {
      readings[i][j] = 0;
    }
  }
}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    total[i] -= readings[i][readIndex];     // Subtract the last reading
    readings[i][readIndex] = analogRead(sensorPins[i]);  // Read from the sensor
    total[i] += readings[i][readIndex];     // Add the reading to the total
    average[i] = total[i] / numReadings;    // Calculate the average
  }

  readIndex++;   // Move to the next position in the array
  if (readIndex >= numReadings) {
    readIndex = 0;  // If we're at the end of the array, wrap around to the beginning
  }

  // Print the average values over serial
  for (int i = 0; i < numSensors; i++) {
    Serial.print(average[i]);
    if (i < numSensors - 1) {
      Serial.print(",");  // Print a comma between values, but not after the last value
    }
  }
  Serial.println();  // Print a newline at the end
  
  digitalWrite(ledPin, HIGH);   // Turn the LED on
  delay(25);                    // Wait for 25ms
  digitalWrite(ledPin, LOW);    // Turn the LED off
  delay(25);                    // Wait for 25ms
}