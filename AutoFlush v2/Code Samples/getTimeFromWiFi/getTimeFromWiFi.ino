/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <TimeLib.h>

#define USE_SERIAL Serial

String ssidName = "DLwireless";
String password = "22342234";

ESP8266WiFiMulti WiFiMulti;

void setTimeUsingWIFI();
void digitalClockDisplay();
String printDigits(int digits);

void setup() {

    USE_SERIAL.begin(115200);
    Serial.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssidName.c_str(), password.c_str());

    setTimeUsingWIFI();
    if(timeStatus()!= timeSet) 
       Serial.println("Unable to sync time with the server");
    else
       Serial.println("Server has set the system time");  
}

void loop() {
  if (timeStatus() == timeSet) {
    digitalClockDisplay();
  } else {
    Serial.println("The time has not been set");
    delay(5000);
  }
  delay(1000);

}

void setTimeUsingWIFI()
{
  //This sets the time using Makitronics web server. Any server that provides unix timestamp will do.
  //This is easier to use than NTP time examples and does not require you to specify the timezone.
  
  if((WiFiMulti.run() == WL_CONNECTED)) { // wait for WiFi connection

      HTTPClient http;

      USE_SERIAL.print("[HTTP] begin...\n");
      // configure traged server and url
      http.begin("http://aws.makitronics.com/iot/projects/autoflush/index.php"); //HTTP

      USE_SERIAL.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if(httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if(httpCode == HTTP_CODE_OK) {
              String payload = http.getString();
              USE_SERIAL.println(payload);

              //time for some parsing
              setTime(payload.toInt()); //now finally set the time
          }
      } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
  }
}

void digitalClockDisplay()
{
  // digital clock display of the time
  String timeNow = printDigits(hourFormat12()) + ":" + printDigits(minute());
  if (isAM()) {
    timeNow.concat(" AM");
  } else {
    timeNow.concat(" PM");
  }
  Serial.println(timeNow); 
}

String printDigits(int digits)
{
  // utility function for digital clock display: returns leading 0 if needed
  String result = "";
  if (digits < 10)
    result = "0";
  result.concat(String(digits));
  return result;
}

