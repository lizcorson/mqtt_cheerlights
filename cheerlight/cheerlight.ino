/* cheerlight
 *  Set an LED color strip from the internet!
 *
 * Liz Corson
 * 27 March 2016
 */

#include <ESP8266WiFi.h>
#include "FastLED.h"

const char* ssid     = "...";
const char* password = "...";
const char* host = "api.thingspeak.com";

long lastUpdateTime = 0;
const long updateWaitTime = 3000; //x.000 second wait
const int feedLength = 100;
bool firstUpdate = false;

#define NUM_LEDS 60
#define DATA_PIN 13 // LED strip on pin 13

int rvals[NUM_LEDS];
int gvals[NUM_LEDS];
int bvals[NUM_LEDS];

int rvalsFeed[feedLength];
int gvalsFeed[feedLength];
int bvalsFeed[feedLength];

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  LEDsOff();
  //initialize arrays
  for(int i = 0; i < NUM_LEDS; i++ ) {
    rvals[i] = 0;
    gvals[i] = 0;
    bvals[i] = 0;
  }
  for(int i = 0; i < feedLength; i++ ) {
    rvalsFeed[i] = 0;
    gvalsFeed[i] = 0;
    bvalsFeed[i] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((unsigned long)(millis() - lastUpdateTime) > updateWaitTime  || firstUpdate == false) {
    getColorFeed();
    SetLEDsFeed();
    if (firstUpdate == false) {
      firstUpdate = true;
    }
//    String rawColor = getCurrentColor();
//    String hexColorString = rawColor.substring(1,8);
//    // Get rid of '#' and convert it to integer
//    long colorNumber = (int) strtol( &hexColorString[1], NULL, 16);
//    // Split them up into r, g, b values
//    int r = colorNumber >> 16;
//    int g = colorNumber >> 8 & 0xFF;
//    int b = colorNumber & 0xFF;
//    //SetAllLEDs(r,g,b);
//    if (isNewColor(r,g,b)) {
//      updateRGBArrays(r,g,b);
//      Serial.print("New color! ");
//      Serial.print(hexColorString);
//      Serial.print(": ");
//      Serial.print(r);
//      Serial.print(",");
//      Serial.print(g);
//      Serial.print(",");
//      Serial.println(b);
//      SetLEDChain();
//    }

  }
}

String getCurrentColor() {
  String colorString = "";
  //Serial.print("connecting to ");
  //Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "";
  }

  // We now create a URI for the request
  String url = "/channels/1417/field/2/last.txt";

  //Serial.print("Requesting URL: ");
  //Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  while(client.available()){
    line = client.readStringUntil('\r');
    //Serial.print(line);
    if (line.indexOf('#') > 0) {
      //Serial.println("COLOR!!!!!!!!");
      colorString = line;
    }

    //delay(500);
  }
  lastUpdateTime = millis();
  return colorString;

}

void getColorFeed() {
  //Serial.print("connecting to ");
  //Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/channels/1417/feed.csv";

  //Serial.print("Requesting URL: ");
  //Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  int feedIndex = 0;
  while(client.available()){
    line = client.readStringUntil('\n');
    //Serial.println(line);
    //delay(500);
    int poundindex = line.indexOf('#');
    if (poundindex > 0) {
      String thisColorString = line.substring(poundindex,poundindex+7);
      Serial.println(thisColorString);
      //colorString = line;
      // Get rid of '#' and convert it to integer
      long colorNumber = (int) strtol( &thisColorString[1], NULL, 16);
      // Split them up into r, g, b values
      int r = colorNumber >> 16;
      int g = colorNumber >> 8 & 0xFF;
      int b = colorNumber & 0xFF;

      rvalsFeed[feedIndex] = r;
      gvalsFeed[feedIndex] = g;
      bvalsFeed[feedIndex] = b;

      Serial.print(feedIndex);
      Serial.print(": ");
      Serial.print(rvalsFeed[feedIndex]);
      Serial.print(",");
      Serial.print(gvalsFeed[feedIndex]);
      Serial.print(",");
      Serial.println(bvalsFeed[feedIndex]);
      feedIndex++;
    }

    //delay(500);
  }
  lastUpdateTime = millis();

}

void SetAllLEDs(int r, int g, int b) {
    for(int i = 0; i < NUM_LEDS; i++ ){
    leds[i].setRGB(r,g,b);  // setRGB functions works by setting
                           // (RED value 0-255, GREEN value 0-255, BLUE value 0-255)
                           // RED = setRGB(255,0,0)
                           // GREEN = setRGB(0,255,0)
    leds[i].fadeLightBy(150);
    FastLED.show();
  }
}

void SetLEDChain() {
    for(int i = 0; i < NUM_LEDS; i++ ){
      leds[i].setRGB(rvals[i],gvals[i],bvals[i]);  // setRGB functions works by setting
                             // (RED value 0-255, GREEN value 0-255, BLUE value 0-255)
                             // RED = setRGB(255,0,0)
                             // GREEN = setRGB(0,255,0)

      leds[i].fadeLightBy(150);
      FastLED.show();
    }
}

void SetLEDsFeed() {
  for(int i = 0; i < NUM_LEDS; i++ ){
    leds[i].setRGB(rvalsFeed[feedLength-1-i],gvalsFeed[feedLength-1-i],bvalsFeed[feedLength-1-i]);  // setRGB functions works by setting
                           // (RED value 0-255, GREEN value 0-255, BLUE value 0-255)
                           // RED = setRGB(255,0,0)
                           // GREEN = setRGB(0,255,0)
    Serial.print(rvalsFeed[feedLength-1-i]);
    Serial.print(",");
    Serial.print(gvalsFeed[feedLength-1-i]);
    Serial.print(",");
    Serial.println(bvalsFeed[feedLength-1-i]);
    leds[i].fadeLightBy(150);
    FastLED.show();
  }
}

bool isNewColor(int r, int g, int b) {
  if (r == rvals[0] && g == gvals[0] && b == bvals[0]) {
    return false;
  } else {
    return true;
  }
}

void updateRGBArrays(int r, int g, int b) {
  for(int i = NUM_LEDS - 1; i > 0; i--) {
    rvals[i] = rvals[i-1];
    gvals[i] = gvals[i-1];
    bvals[i] = bvals[i-1];
  }
  rvals[0] = r;
  gvals[0] = g;
  bvals[0] = b;

  // note this should be replaced by leds[i] = leds[j]; maybe??
}

void LEDsOff() {
  for(int i = 0; i < NUM_LEDS; i++ ){
    leds[i] = CRGB::Black;
    FastLED.show();
  }
}
