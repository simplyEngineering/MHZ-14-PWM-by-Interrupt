# MHZ-14-PWM-by-Interrupt

Simple Interrupt driven CO2 level measurement  for microcontroller projects (Arduino/ESP32/ESP8266 etc) using the PWM output of the MHZ-14 device.

This code is offered as a simple but very reliable and accurate means of measuring the Pulse Width Ratio of the output from an MHZ-14/17 type sensor.  It's minimalist in the extreme,  requires no complex classes or libraries and will run with little or no  interference  to other running code.   

The ppm calculation is that supplied by the device manufacturer:  

    ppm (parts per  million CO2) = 5000 * (tHi - 2)/(tHi + tLo - 4)  where tHi and tLo are the Hi and Lo times measured in mS.  

This formula is for a device with a 0 - 5000ppm range.  For a 0 - 2000ppm device the 5000 multiplier in the numerator would be changed to 2000.

