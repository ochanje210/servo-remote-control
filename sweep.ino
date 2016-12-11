#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

String receivedData;
char serialEndByte = '\n';

#define PIN            6
#define NUMPIXELS      24

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 10; // delay for half a second

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(115200);
  Serial.println("Hello IoT Testbed");
  myservo.write(0);
}
int ref = 0;
void loop() {
  if (Serial.available() > 0) {
    char character = Serial.read();
    if (character == serialEndByte) {
      Serial.print("Received: ");
      Serial.println(receivedData);
      receivedData.trim();

      int colonPosition  = receivedData.indexOf(':');
      String key = receivedData.substring(0,colonPosition);
      int value = receivedData.substring(colonPosition+1).toInt();

      if (key.equals("rotate") ) {
        for (pos = 0; pos <= value; pos += 1) {
          myservo.write(pos);
          delay(10);
        }
        for (pos = value; pos >= 0; pos -= 1) { 
          myservo.write(pos);
          delay(10);              
        }
      }
      receivedData = "";
    } else {
      receivedData.concat(character);
    }
  }
}
