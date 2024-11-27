/*
“L” We will send this when the light should start filling up
The fill up is currently set to take 1 second (this might be adjusted later depending on how easy or hard it is for the kids to hold on the target that long, but let’s start with 1 second for the lights to fill all the way up as well).
“A” We will send this if the user stops hovering over the target before the 1 second completes, and should trigger the light to reset/turn back off
“S” We will send this when the user has held on the target long enough to shoot, so the actuator should be triggered and the light should reset/turn back off
*/

#include <FastLED.h>

#define PIN        5 // Pin on the Arduino is connected to the NeoPixels
#define SHOOT_PIN  8 // SHooter ping

// Number of NeoPixels attached to the Arduino
#define NUMPIXELS 10

CRGB leds1[NUMPIXELS];

#define DELAYVAL 125 // Time (in milliseconds) to pause between pixels
String a; // variable for char read from serial 

char receivedChar;
boolean newData = false;
int stop_shoot = 0;

void setup() {
  Serial.begin(9600); // setup serial comm
  Serial.println("Pluto Launcher v 1.2 new");

  FastLED.addLeds<NEOPIXEL, PIN>(leds1, NUMPIXELS);
  FastLED.setBrightness(80);
  FastLED.clear(); //clears ALL leds on both strips
  FastLED.show();

  pinMode(SHOOT_PIN, OUTPUT);
}

void loop() {

  recvOneChar();
  showNewData();

  FastLED.clear(); //clears ALL leds on both strips
  FastLED.show();
  delay(200);

  leds1[0] = CRGB::White;
  leds1[NUMPIXELS] = CRGB::White;
  //leds1[NUMPIXELS - 1] = CRGB::White;
  //leds1[NUMPIXELS - 2] = CRGB::White;
  FastLED.show();

  if (receivedChar == 'L'){
  //  Serial.println("In loop");
  recvOneChar();

  for(int i=0; i<NUMPIXELS/2; i++) { // For each pixel...
    recvOneChar();
    if (receivedChar == 'A'){
      stop_shoot = 1;
      Serial.println("Abort");
      i=NUMPIXELS;
    }

    leds1[i] = CRGB::White;
    leds1[NUMPIXELS - i] = CRGB::White;

    FastLED.show();
    delay(100);
  }

  if (!stop_shoot){
    digitalWrite(SHOOT_PIN, HIGH);
    delay(1000);
    digitalWrite(SHOOT_PIN, LOW);
    stop_shoot = 0;
  }
  stop_shoot = 0;
  }

//  if (receivedChar == 'S'){
    //  Shoot
//    Serial.println("Shoot");
//    digitalWrite(SHOOT_PIN, HIGH);
//    delay(1000);
//    digitalWrite(SHOOT_PIN, LOW);
//}
}

void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChar);
        newData = false;
    }
}