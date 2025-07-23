/*
DESCRIPTION
This sketch allows attiny85 to work as a fast pulser.  Using active low logic.
 DATE created: 06.2024
 Author:  Areg Danagoulian, aregjan@mit.edu
*/
#include <Tiny4kOLED_bitbang.h>

#define pulse_pin 1 //pulser out
//#define INPUT_PIN_1 5

int period=50; //the period in miliseconds
float duty=0.01; //the duty in percents
uint32_t pulse_width=10; //the pulse width in us

void setup() {

//  pinMode(INPUT_PIN_1,INPUT);

  oled.begin(128, 32, sizeof(tiny4koled_init_128x32br), tiny4koled_init_128x32br);
  oled.enableChargePump();  // The default is off, but most boards need this.

  //  oled.begin(128, 64, sizeof(tiny4koled_init_128x64), tiny4koled_init_128x64);
  //  oled.setRotation(0);
  oled.setFont(FONT6X8);  //font6x8, font8x16
  oled.clear();
  oled.on();
  oled.print("Booted...");

  pinMode(A3,INPUT); //analog in
  pinMode(A2,INPUT); //analog in

  pinMode(pulse_pin,OUTPUT);
  PORTB |= 1<<pulse_pin; //have it on

}

int count=0;
void loop() {



  if(millis()%period==0) {
    pulse();
    delay(2); //you need this for this condition not to trigger multiple times
  }
  if(millis()%1000==0) {
    read_settings_values(); //once per sec read the settings 
    updateDisplay();
  }

}


void pulse(){

     
    PORTB ^= 1<<pulse_pin; //turn off PB1
//    delayMicroseconds(period*1000.*duty); //this is blocking, however this is very precise.  Here are some possible values for the laser delay:
    delayMicroseconds(pulse_width); //this is blocking, however this is very precise.  Here are some possible values for the laser delay:
    // *  50us for the Chinese 1.1 W.  
    // * 25us for the 25mW and 5mW normally produces 2-3 pulses. 
    // * 40us -- for demo purposes only.  
    PORTB |= 1<<pulse_pin; //turn on PB1

}
void read_settings_values(){

  uint16_t v2=analogRead(A2); //2^10 ADC, physical pin 3
  uint16_t v3=analogRead(A3); //2^10 ADC, physical pin 2

  period=1000.*v2/pow(2,10); // range: 0-1000 ms
  duty  =0.001*v3/pow(2,10); //range: 0-0.1%
  pulse_width=uint32_t(float(period)*1000.*duty); //width in us

}
void updateDisplay() {
  char buf[10];
  dtostrf(duty*100, 8, 6, buf); // format with 2 decimal places

  oled.clear();
  oled.print(F("Period (ms): "));   oled.print(period);
  oled.print(F("\nDuty (%): ")); oled.print(buf);
//  oled.setCursor(0, 6);
}