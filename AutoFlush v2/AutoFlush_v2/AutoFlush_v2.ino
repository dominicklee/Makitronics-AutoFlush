/* Makitronics AutoFlush v2 - last revised 2/1/2020
 *  [Bringing your home's bathroom to the next level]
 * Written by Dominick Lee (dominick@makitronics.com)
 * To build this, you will need a Heltec WiFi Kit 8, Sharp GP2Y0A21 IR sensor, 55G Metal Gear Servo, Long USB cable, USB adapter, and my 3D printed parts
 * For details, check out: https://github.com/dominicklee/Makitronics-AutoFlush
 * 
 * Configuration parameters:
 * WiFi SSID/password
 * Distance of detection (in CM)
 * Minimum dection time (in seconds)
 * Flush duration (in seconds)
 * Tank refill duration (in seconds)
 * 
 * This version features:
 * Automatic clock display
 * User session time (aka how long you sat on toilet)
 * Visual progress bar during activation and refilling
 * AutoFlush v2 features a smaller form-factor with more capabilities
 * 
 * Possible future add-ons:
 * Local Weather display
 * Alert for long-time toilet sitting (may help prevent DVT, no guarantees)
 * 
 */

#include <SharpIR.h>
#include <Servo.h> 
#include <U8g2lib.h>

//SharpIR
#define ir A0
#define model 20150   //we are actually using GP2Y0A21Y, but 20150 gives more accurate results on ESP

//Servo variables
int servoPin = D8;
int normalAngle = 0;  //might need to experiment with this one
int flushAngle = 70;  //might need to experiment with this one

//Configuration parameters
String ssidName = "";   //WiFi SSID
String password = "";   //WiFi password
int8_t cmDistance = 80;    //Distance of detection in centimeters (X cm or closer to activate)
int8_t minDetectTime = 3;  //X seconds you need to stand there to activate
int8_t flushDuration = 5;  //Hold the toilet for X seconds
int8_t refillDuration = 25; //Wait X seconds after flush for toilet to refill

//Screen variables
String topMsg = ""; //top message - could be time, session
String mainMsg = "";  //button message - could be Active, Flushing, etc
bool flashing = false;  //whether the button message should flash
int8_t perc = 0;
int8_t flushSec = 0;  //this gets incremented every second of flush
String sessionTime = ""; //for storing session time

/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

//Few more variables for sit-down time
long timeActiveStart = 0; //millis timestamp when activated

SharpIR SharpIR(ir, model);
Servo myservo; 
//U8G2 Contructor
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

//Declare function prototypes
void flushToilet();
void drawScreen(int8_t stage);
void getTimeFromMillis(long val);
void progBarTime(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent, String msg);
void button(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, String str, bool clicked);

void setup() 
{
  Serial.begin(115200);
  u8g2.begin();
  
}  //end setup

void loop() 
{
  int dis = SharpIR.distance();  // this returns the distance to the object you're measuring
  //Serial.print("Mean distance: ");  // returns it to the serial monitor
  //Serial.println(dis);
  
  //Handle Automatic Detection
  if (dis < cmDistance)
  {
    Serial.println("Initial detection");
    //show progress bar go up until minDetectTime
    perc = 0; //set percent to 0%
    drawScreen(0);
    
    dis = SharpIR.distance();  // check distance again
    
    if (dis < cmDistance)  //check
    {
      //show Active button lit
      Serial.println("Person detected");
      timeActiveStart = millis();
      drawScreen(1);
      
      int timeout = 0;
      while (timeout <= 10)
      {
        dis = SharpIR.distance();  // keep checking every 200ms if someone is there
        if (dis < cmDistance)  //person still there
        {
          Serial.println("Person still there");
          timeout = 0;
          delay(200);  //wait
        }
        else
        {
          Serial.println(timeout);
          timeout ++;
          delay(200);  //count 10 of these for 2 second delay
        }
      }
      flushToilet();  //we're out of the while loop. clear to flush
    }  
  }
  else
  {
    //this gets run when no obstace is near the sensor. we should blank the display
    u8g2.clear();
  }
  
}  //end loop

void flushToilet()
{
  long elapsedSitting = millis() - timeActiveStart; //first log the time-done so we don't include flushing
  flushSec = 0; //reset
    
  Serial.println("Flushing");
  myservo.attach(servoPin);  // attaches the servo on pin D8 to the servo object 
  myservo.write(flushAngle);  // flush down
  for (int i = 0; i < flushDuration; i++) {
    flushSec = i; //set global - this is so that we can blink the "Flushing" UI
    drawScreen(2); //update display
    delay(1000);  //delay 1 second for flushDuration seconds
  }
  myservo.write(normalAngle);
  
  //update "Flushing" UI a couple more times (to give 2.5 secs for the servo to return)
  flushSec++; //keep incrementing global
  drawScreen(2); //update display
  delay(1000);  //wait 1 sec
  flushSec++; //keep incrementing global
  drawScreen(2); //update display
  delay(1500);  //wait 1.5 sec
  myservo.detach();  // release servo

  getTimeFromMillis(elapsedSitting / 1000); //this will set sessionTime so it can be displayed
  drawScreen(3); //update display to show the session time
  delay(2500);  //wait 2.5 seconds

  Serial.println("Refilling");
  perc = 0; //reset progress bar percent
  drawScreen(4); //update display to show progress bar
}

void getTimeFromMillis(long val)
{  
  int8_t days = elapsedDays(val);
  int8_t hours = numberOfHours(val);
  int8_t minutes = numberOfMinutes(val);
  int8_t seconds = numberOfSeconds(val);

  sessionTime = String(hours) + "hr " + String(minutes) + "min " + String(seconds) + "sec";
  Serial.println(sessionTime);
}

void progBarTime(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t percent, String msg)
{
    // can't draw it smaller than 10x8
    height = height < 8 ? 8 : height;
    width = width < 10 ? 10 : width;

    // draw percentage
    u8g2.setFont(u8g2_font_6x10_tr);
    //u8g2.drawStr(x + width / 2 - 5 * 1, y - 1, (String(percent) + String("%")).c_str());  //display percent
    //String msg = "10:31 PM";
    u8g2.drawStr((width / 2) - (msg.length() * 5 / 2), y - 1, msg.c_str());

    // draw it
    u8g2.drawRFrame(x, y + 3, width, height, 4);
    u8g2.drawBox(x + 2, y + 5, (width - 4) * (percent / 100.0), height - 4);
}

void drawScreen(int8_t stage)
{
    /*  The stages are all blocking, but they block deliberately according to our set paramters
     *  We are being creative with the UI here and trying to keep what we had in AutoFlush v1
     *  
     *  The stages are as follows:
     *    0 - Time w/ prog. bar go up (for minDetectTime)
     *    1 - Active button lit
     *    2 - Flashing flushing button (flash for flushDuration)
     *    3 - Session end time button (for 2.5 seconds)
     *    4 - Refilling w/ prog. bar go up (for refillDuration)
     */

    if (stage == 0) //Time w/ prog
    {
      while (perc < 100)  //count up to 100%
      {
        u8g2.firstPage();
        do {
          u8g2.setDrawColor(1);
          progBarTime(u8g2, 2, 12, u8g2.getDisplayWidth() - 5, 15, perc, "10:31 PM"); //show prog bar
          perc++; //imcrement percent
          int8_t delayPerPercent = minDetectTime * 10;  //gets ms for each percent delay
          delay(delayPerPercent);
        } while ( u8g2.nextPage() );
      }
    }
    else if (stage == 1)  //Active button (no delays here)
    {
      u8g2.firstPage();
      do {
        u8g2.setDrawColor(1);
        button(u8g2, 2, 4, 124, "Active", true);
      } while ( u8g2.nextPage() );
    }
    else if (stage == 2) //Flushing button (no delays here)
    {
      u8g2.firstPage();
      do {
        u8g2.setDrawColor(1);
      button(u8g2, 2, 4, 124, flushSec % 2 ? "Flushing..." : "Flushing...", flushSec % 2 ? true: false);
      } while ( u8g2.nextPage() );
    }
    else if (stage == 3) //Session end time
    {
      u8g2.firstPage();
      do {
        u8g2.setDrawColor(1);
        button(u8g2, 2, 4, 124, sessionTime, true);
      } while ( u8g2.nextPage() );
    }
    else if (stage == 4) //Refilling w/ prog
    {
      while (perc < 100)  //count up to 100%
      {
        u8g2.firstPage();
        do {
          u8g2.setDrawColor(1);
          progBarTime(u8g2, 2, 12, u8g2.getDisplayWidth() - 5, 15, perc, "Refilling..."); //show prog bar
          perc++; //imcrement percent
          int delayPerPercent = (refillDuration - 2) * 10;  //gets ms for each percent delay
          delay(delayPerPercent);
        } while ( u8g2.nextPage() );
      }
    }
    else {  
      //do nothing
    }
}

void button(U8G2 u8g2, uint8_t x, uint8_t y, uint8_t width, String str, bool clicked)
{
    int getMaxCharHeight = 9;
    int charWidth = 7;

    if (clicked) {
        u8g2.setDrawColor(1);
        u8g2.drawRBox(x, y+1, width,  getMaxCharHeight + 12, 2);
        u8g2.setDrawColor(0);
        u8g2.setFont(u8g2_font_profont15_mr);
        u8g2.drawStr((width/2) - (charWidth * str.length() / 2), getMaxCharHeight + y + 7, (String(str)).c_str());
    } else {
        u8g2.setDrawColor(1);
        u8g2.drawRFrame(x, y, width,  getMaxCharHeight + 14, 4);
        u8g2.setFont(u8g2_font_profont15_mr);
        u8g2.drawStr((width/2) - (charWidth * str.length() / 2), getMaxCharHeight + y + 7, (String(str)).c_str());
    }
}
