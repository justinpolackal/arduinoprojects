/*
   IRremote: IRrecvDump - dump details of IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
   JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
   LG added by Darryl Smith (based on the JVC protocol)
*/
#include <Servo.h>
#include <IRremote.h>
int i = 0;;
int a = 0;
Servo s;
/*
   Default is Arduino pin D11.
   You can change this to another available Arduino Pin.
   Your IR receiver should be connected to the pin defined here
*/
int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  s.attach(9);
  delay(50);
  s.write(0);
  pinMode(9,OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Starting");
}


void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");
//  if(results->value == 0x882C9158)
//  {
//    if(i == HIGH)
//    i=LOW;
//
//    else if(i == LOW)
//    i=HIGH;
//    
//    digitalWrite(9,i);
//    delay(500);
//  }
   if(results->value == 0xEBBEE2B5)
  {
    Serial.println(a);
    if(a<180)
    a = a+10;
//    analogWrite(9,a);
  }

  else if(results->value == 0xE892FE5B)
  {
    Serial.println(a);
    if(a>0)
    a = a-10;
//    analogWrite(9,a);
  }

  Serial.print(" ");

  Serial.println();
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    dump(&results);
    if(i==0)
    i=180;

    else if(i==180)
    i=0;
    
        s.write(i);

    irrecv.resume(); // Receive the next value
  }
  
}
