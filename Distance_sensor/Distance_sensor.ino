/* Batifier/Dolphinator
 * A portable distance sensor with vibration feedback
 * Using the NewPing library by teckel: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home
 * This is just a barebone sketch that runs the most simple design: 1 ultrasonic sensor, 1 vibration motor, 1 Arduino Uno, 1 Powerbank and some cables
 */

#include <NewPing.h>
#define TriggerPin 3  // Trigger and Echo come through the same pin
#define MaxDist 200   // Max distance of 2 meters
#define VibroPin 4    // Digital output pin for vibration motor
#define LEDPin1 8     // Light up some LEDs to create enough current drainage to keep the powerbank running
#define LEDPin2 9
#define LEDPin3 10

NewPing sonar(TriggerPin,TriggerPin,MaxDist);
int dist;                   // Save the distance for vibration pattern
static int VibroDur = 50;   // Duration of one vibration pulse. Stays the same
int VibroPause;             // Pause between two pulses. Becomes shorter with smaller distance

void setup() {
  Serial.begin(115200);             // Open serial monitor at 115200 baud to see ping results.
  pinMode(VibroPin, OUTPUT);
  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);

  digitalWrite(LEDPin1, HIGH);      // Light up the LEDs - this is not necessary if you use a different power source
  digitalWrite(LEDPin2, HIGH);
  digitalWrite(LEDPin3, HIGH);
}

void loop() {
  dist = sonar.convert_cm(sonar.ping_median(10));     // Convert the median of 10 measurements to cm
  Serial.println(dist);
  VibroPause = (dist^2)*3;                            // Result of some playing around - this varies the pause duration. I found the mixture of exponential and linear change most intuitive.
  
  if(dist > 30){                                      // If the distance is smaller than 30 cm, the vibration motor goes into "warning mode"
    digitalWrite(VibroPin, HIGH);
    delay(VibroDur);
    digitalWrite(VibroPin, LOW);
    delay(VibroPause);
  }
  else if (dist <= 30 && dist != 0){
    for(int i = 0; i < 5; i++){
      digitalWrite(VibroPin, HIGH);
      delay(50);
      digitalWrite(VibroPin, LOW);
      delay(50);
    }
  }
}
