#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
  
//define your default values here, if there are different values in config.json, they are overwritten.
char distance_detection[5];
char min_detect_duration[5];
char flush_duration[5];
char refill_duration[5];
char normal_angle[5];
char flush_angle[5];

//flag for saving data
bool shouldSaveConfig = false;

WiFiManager wifiManager;
  
//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    
    //clean FS, for testing
    if (!SPIFFS.exists("/formatComplete.txt")) 
    {
      Serial.println("Please wait 30 secs for SPIFFS to be formatted");
      SPIFFS.format();
      Serial.println("Spiffs formatted");
  
      File f = SPIFFS.open("/formatComplete.txt", "w");
      if (!f) {
        Serial.println("file open failed");
      } else {
        f.println("Format Complete");
      }
      f.close();
    } 
    else 
    {
      Serial.println("SPIFFS is formatted. Moving along...");
    }

    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(distance_detection, json["distance_detection"]);
          strcpy(min_detect_duration, json["min_detect_duration"]);
          strcpy(flush_duration, json["flush_duration"]);
          strcpy(refill_duration, json["refill_duration"]);
          strcpy(normal_angle, json["normal_angle"]);
          strcpy(flush_angle, json["flush_angle"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read



  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_distance_detection("distance_detection", "Distance Detection (cm)", distance_detection, 3);
  WiFiManagerParameter custom_min_detect_duration("min_detect_duration", "Min Detect Duration (sec)", min_detect_duration, 3);
  WiFiManagerParameter custom_flush_duration("flush_duration", "Flush Duration (sec)", flush_duration, 3);
  WiFiManagerParameter custom_refill_duration("refill_duration", "Refill Duration (sec)", refill_duration, 3);
  WiFiManagerParameter custom_normal_angle("normal_angle", "Normal Angle (deg)", normal_angle, 3);
  WiFiManagerParameter custom_flush_angle("flush_angle", "Flush Angle (deg)", flush_angle, 3);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  //WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  //wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_distance_detection);
  wifiManager.addParameter(&custom_min_detect_duration);
  wifiManager.addParameter(&custom_flush_duration);
  wifiManager.addParameter(&custom_refill_duration);
  wifiManager.addParameter(&custom_normal_angle);
  wifiManager.addParameter(&custom_flush_angle);
    
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoFlush Config", "12345678")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yaaay :)");

  //read updated parameters
  strcpy(distance_detection, custom_distance_detection.getValue());
  strcpy(min_detect_duration, custom_min_detect_duration.getValue());
  strcpy(flush_duration, custom_flush_duration.getValue());
  strcpy(refill_duration, custom_refill_duration.getValue());
  strcpy(normal_angle, custom_normal_angle.getValue());
  strcpy(flush_angle, custom_flush_angle.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["distance_detection"] = distance_detection;
    json["min_detect_duration"] = min_detect_duration;
    json["flush_duration"] = flush_duration;
    json["refill_duration"] = refill_duration;
    json["normal_angle"] = normal_angle;
    json["flush_angle"] = flush_angle;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) 
  {
    String incoming = Serial.readString();
    if (incoming == "config") 
    {
      Serial.println("Settings reset");
      wifiManager.resetSettings();
    }
  }

  if (millis() % 2000 == 0) { //print every 2 seconds
    int cmDistance, minDetectTime, flushDuration, refillDuration, normalAngle, flushAngle;
    
    cmDistance = atoi(distance_detection);
    minDetectTime = atoi(min_detect_duration);
    flushDuration = atoi(flush_duration);
    refillDuration = atoi(refill_duration);
    normalAngle = atoi(normal_angle);
    flushAngle = atoi(flush_angle);

    Serial.println(cmDistance);
    Serial.println(minDetectTime);
    Serial.println(flushDuration);
    Serial.println(refillDuration);
    Serial.println(normalAngle);
    Serial.println(flushAngle);
    Serial.println();
  }

}
