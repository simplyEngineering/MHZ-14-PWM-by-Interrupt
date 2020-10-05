/*
This Arduino sketch by simplyEngineering offers a simple, code-efficient, 
accurate and reliable means of reading the PWM output of the MHZ-14 CO2 
sensor. It avoids time wasting delay() and polling loops by using the 
interrupt capabilities of the Arduino and ESP32/8266 family of 
microcontrollers.  

NOTE 1
There is a warm-up time specified by the device manufacturer within which
the device may output invalid readings.  Users are encouraged to consult
the manufacturers data sheet.

NOTE 2
A free microcontroller input pin is allocated which is connected to the 
PWM output of the sensor. In this example pin10 of an ESP8266 is used 
which may be varied according to the specific microcontroller type and 
specific circuit arrangement.
*/


#define PWM_IN 10
#define samplePeriod 5000

volatile  int   hiStart,loStart = 0;
int             tHi, tLo, CO2_ppm = 0;
volatile bool   edgeR, edgeF = false;
long            timerStart = 0;

void setup(){
  Serial.begin(115200);
  delay(500);
  Serial.println("MHZ_14_PWM_by_interrupt");
  pinMode(PWM_IN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PWM_IN),isrF,FALLING);
  timerStart = millis();
}

ICACHE_RAM_ATTR void isrR()
{
  edgeR = true;
  hiStart = millis();
  attachInterrupt(digitalPinToInterrupt(PWM_IN),isrF,FALLING);
}

ICACHE_RAM_ATTR void isrF()
{
  edgeF = true;
  loStart = millis();
  attachInterrupt(digitalPinToInterrupt(PWM_IN),isrR,RISING);
}

void loop(){
  if (edgeR){
    tLo = hiStart - loStart;
    edgeR = false;
  }
  if (edgeF){
    edgeF = false;
    tHi = loStart - hiStart;
    CO2_ppm = 5000 * (tHi - 2)/(tHi + tLo - 4);
  }
  if(millis() - timerStart > samplePeriod) {
    Serial.print("CO2 = "); Serial.print(CO2_ppm); Serial.println(" parts per million");
    timerStart = millis(); 
  }
  ////////////////////////
  // do your thing here //
  ////////////////////////
}
