#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// --- Constants for analog reading ---
const int sensorPins[] = {A0, A1, A2, A3, A4};
const int numSensors = 5;
const int numReadings = 10;

int readings[numSensors][numReadings];
int readIndex = 0;
int total[numSensors];
int average[numSensors];
int ledPin = 13;

// --- Constants for LED Strip ---
#define LED_PIN    6
#define LED_COUNT 144

const int groupLengths[] = {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 12};
const int groupStartIndices[] = {0, 13, 26, 39, 52, 65, 78, 91, 104, 117, 130};
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t led_color_array[LED_COUNT];

const uint32_t RED_COLOR = strip.Color(255, 0, 0);
const uint32_t ORANGE_COLOR = strip.Color(255, 200, 0);
const uint32_t YELLOW_COLOR = strip.Color(255, 255, 0);
const uint32_t GREEN_COLOR = strip.Color(0, 255, 0);
const uint32_t BLUE_COLOR = strip.Color(0, 0, 255);
const uint32_t INDIGO_COLOR = strip.Color(75, 0, 130);
const uint32_t VIOLET_COLOR = strip.Color(148, 0, 211);
const uint32_t WHITE_COLOR = strip.Color(255, 255, 255);
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);
const uint32_t PURPLE_COLOR = strip.Color(148, 0, 211);

void setGroupColor(int groupIndex, uint32_t color);
void leds_update();

void setup() {
    pinMode(ledPin, OUTPUT);
    strip.begin();
    strip.show();
    strip.setBrightness(50);
    Serial.begin(115200);

    for (int i = 0; i < numSensors; i++) {
        total[i] = 0;
        for (int j = 0; j < numReadings; j++) {
            readings[i][j] = 0;
        }
    }
}

void leds_update() {
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, led_color_array[i]);
    }
    strip.show();
}

void leds_off() {
    for (int i = 0; i < LED_COUNT; i++) {
        led_color_array[i] = 0;
    }
    leds_update();
}

void setGroupColor(int groupIndex, uint32_t color) {
    int startIndex = groupStartIndices[groupIndex];
    int endIndex = startIndex + groupLengths[groupIndex] - 1;

    for (int i = startIndex; i <= endIndex; i++) {
        led_color_array[i] = color;
    }
}

void loop() {
    // --- Analog Reading Code ---
    for (int i = 0; i < numSensors; i++) {
        total[i] -= readings[i][readIndex];
        readings[i][readIndex] = analogRead(sensorPins[i]);
        total[i] += readings[i][readIndex];
        average[i] = total[i] / numReadings;
    }
    readIndex++;
    if (readIndex >= numReadings) readIndex = 0;

    for (int i = 0; i < numSensors; i++) {
        Serial.print(average[i]);
        if (i < numSensors - 1) Serial.print(",");
    }
    Serial.println();

    digitalWrite(ledPin, HIGH);
    delay(25);
    digitalWrite(ledPin, LOW);
    delay(25);

    // --- LED Code ---
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');

        if (command.length() >= 4) {
            int group = command.charAt(0) - '0';
            uint8_t r = command.substring(1, 4).toInt();
            uint8_t g = command.substring(4, 7).toInt();
            uint8_t b = command.substring(7, 10).toInt();

            uint32_t color = strip.Color(r * 25, g * 25, b * 25);
            setGroupColor(group, color);
            leds_update();
        }
    }
}





