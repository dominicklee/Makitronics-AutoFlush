// macros from DateTime.h 
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN) 
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

void getTimeFromMillis(long val);

void setup(){
Serial.begin(115200);
}

void loop(){  
 getTimeFromMillis(millis() / 1000);
 delay(1000);
}

void getTimeFromMillis(long val)
{  
  int8_t days = elapsedDays(val);
  int8_t hours = numberOfHours(val);
  int8_t minutes = numberOfMinutes(val);
  int8_t seconds = numberOfSeconds(val);

  String sessionTime = String(hours) + "hrs " + String(minutes) + "mins " + String(seconds) + "secs";
  Serial.println(sessionTime);
}

