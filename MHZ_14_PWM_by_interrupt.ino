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
circuit arrangement.

FUNCTION
The MHZ-14 generates a digital Pulse Width Modulated signal, the ratio 
of Hi to Lo times indicating CO2 in parts per million (ppm) according
to the formula: 
    CO2_ppm = k * (tHi - 2)/(tHi + tLo - 4)
where tHi and tLo are the measured Hi and Lo times of the pulse in 
milliseconds and k is either 2000 or 5000 depending on the range of the
specific device.

In setup() an interrupt is 'attached' to PWM_IN (pin 10 in this example),
is configured to trigger on the falling edge of its input, and executes
one of the two interrupt service routines (isrF). By so doing the time
is registered in the loStart variable and the active edge of the interrupt
is changed from falling to rising.  When the next rising edge of the 
interrupting signal arrives the other (isrR) interrupt service routine 
is executed the time of which is recorded in the hiStart variable and the 
active edge is again changed back to falling.  Once both loStart and hiStart
variables are aquired the ppm measurement is computed according to the 
formula. The cycle is repeated at an interval in milliseconds according
to the samplePeriod definition. Other operations as required will be executed
in the loop() process.

Feel free to contact the author (stuart@ceng.me.uk)  if you have any queries.
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
