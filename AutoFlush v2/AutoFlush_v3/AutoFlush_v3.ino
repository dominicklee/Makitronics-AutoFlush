//Written by Dominick Lee (dominick@makitronics.com)
//To build this, you will need 8x Neopixels WS2812, an Arduino Nano, servo, SharpIR senosr, and a TP4056 module.
//Adjust the flush timings as desired.

#include <SharpIR.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h> 

//SharpIR
#define ir A0
#define model 1080

//Neopixels
#define PIN 3
#define NUMPIXELS 8

SharpIR SharpIR(ir, model);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Servo myservo; 

int maxDist = 85;

void flushToilet()
{
  Serial.println("Flushing");
  pixels.setPixelColor(0, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(2, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(3, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(4, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(5, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(6, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.setPixelColor(7, pixels.Color(0,0,150)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo.write(50);  // flush down
  delay(3500);  // wait 1.5 seconds
  myservo.write(0);
  delay(3500);  // wait 1.5 seconds
  myservo.detach();  // release

  
  Serial.println("Refilling tank");
  for(int j = 0; j < 30; j++)
  {
    for(int i=0;i<NUMPIXELS;i++)
    {
      pixels.setPixelColor(0, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(2, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(3, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(4, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(5, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(6, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(7, pixels.Color(0,0,0)); // Moderately bright green color.
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,150)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(100); // Delay for a period of time (in milliseconds).
      Serial.println(i);
    }
    for(int i=(NUMPIXELS-2);i > 0;i--)
    {
      pixels.setPixelColor(0, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(2, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(3, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(4, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(5, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(6, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(7, pixels.Color(0,0,0)); // Moderately bright green color.
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,150)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(100); // Delay for a period of time (in milliseconds).
      Serial.println(i);
    }
  }
      //last pixel
      pixels.setPixelColor(0, pixels.Color(0,0,150)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(2, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(3, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(4, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(5, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(6, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.setPixelColor(7, pixels.Color(0,0,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(100); // Delay for a period of time (in milliseconds).
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library
  pixels.setBrightness(50);
  
  
}  //end setup

void loop() 
{
  
  int dis = SharpIR.distance();  // this returns the distance to the object you're measuring

  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  
  //Handle Automatic Detection
  if (dis < maxDist)
  {
    Serial.println("Initial detection");
    
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(300); // Delay for a period of time (in milliseconds).
    }
    
    dis = SharpIR.distance();  // this returns the distance to the object you're measuring
    if (dis < maxDist)  //check
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(0, pixels.Color(120,100,0)); // Moderately bright blue color
      pixels.setPixelColor(1, pixels.Color(120,100,0)); // Moderately bright blue color
      pixels.setPixelColor(2, pixels.Color(120,100,0)); // Moderately bright blue color
      pixels.setPixelColor(3, pixels.Color(120,100,0)); // Moderately bright blue color
      pixels.setPixelColor(4, pixels.Color(120,100,0)); // Moderately bright blue color
      pixels.setPixelColor(5, pixels.Color(120,100,0)); // Moderately bright green color.
      pixels.setPixelColor(6, pixels.Color(120,100,0)); // Moderately bright green color.
      pixels.setPixelColor(7, pixels.Color(120,100,0)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      Serial.println("Person detected");
      
      int timeout = 0;
      while (timeout <= 10)
      {
        dis = SharpIR.distance();  // this returns the distance to the object you're measuring
        if (dis < maxDist)  //person there
        {
          Serial.println("Person still there");
          timeout = 0;
          delay(220);  //wait
          
        }
        else
        {
          Serial.println(timeout);
          timeout ++;
          delay(220);  //wait 10 of these for 2 second delay
        }
      }

      //clear to flush
      flushToilet();
      
    }
    
  }
  else
  {
    for(int i=0;i<NUMPIXELS;i++)
    {
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
    
      pixels.show(); // This sends the updated pixel color to the hardware.
    
      delay(100); // Delay for a period of time (in milliseconds).
    }
  }

}  //end loop
