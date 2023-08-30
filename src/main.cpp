#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

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

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);
    Serial.begin(115200); // Initialize serial communication

}

void leds_update() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, led_color_array[i]);
  }
  strip.show();
}

void leds_off() {
    for (int i = 0; i < LED_COUNT; i++) {
      led_color_array[i]=0;
  }
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, led_color_array[i]);
  }
  strip.show();
}

void setGroupColor(int groupIndex, uint32_t color) {
  int startIndex = groupStartIndices[groupIndex];
  int endIndex = startIndex + groupLengths[groupIndex] - 1;
  
  for (int i = startIndex; i <= endIndex; i++) {
    led_color_array[i] = color;
  }
}

void middle_rainbow(int wait){
    setGroupColor(5, RED_COLOR); // Set groups 5 and 6 to white
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second
  
  setGroupColor(4, ORANGE_COLOR); // Set groups 4 and 7 to red
  setGroupColor(6, ORANGE_COLOR);
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second

  setGroupColor(3, YELLOW_COLOR); // Set groups 4 and 7 to red
  setGroupColor(7, YELLOW_COLOR);
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second

  setGroupColor(2, GREEN_COLOR); // Set groups 4 and 7 to red
  setGroupColor(8, GREEN_COLOR);
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second

  setGroupColor(1, BLUE_COLOR); // Set groups 4 and 7 to red
  setGroupColor(9, BLUE_COLOR);
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second

  setGroupColor(0, PURPLE_COLOR ); // Set groups 4 and 7 to red
  setGroupColor(10, PURPLE_COLOR);
  leds_update(); // Update the strip
  delay(wait); // Pause for 1 second
  leds_off();
  delay(wait); // Pause for 1 second
}

void middle_rainbow_accending(){
  for(int i=300; i<0; i-=20){
    middle_rainbow(i);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the entire line until newline character
    
    if (command.length() >= 4) {
      int group = command.charAt(0) - '0'; // Get the group number (subtract '0' to convert from ASCII to integer)
      uint8_t r = command.substring(1, 4).toInt(); // Get the red component as an integer
      uint8_t g = command.substring(4, 7).toInt(); // Get the green component as an integer
      uint8_t b = command.substring(7, 10).toInt(); // Get the blue component as an integer
      
      
      uint32_t color = strip.Color(r * 25, g * 25, b * 25); // Convert 0-9 range to 0-255 range
      setGroupColor(group, color);
      leds_update();
    }
  }
}