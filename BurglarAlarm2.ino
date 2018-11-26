#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeOrion.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;

MeUltrasonicSensor ultrasonic(3);
MePIRMotionSensor pir(4);
Me7SegmentDisplay disp(PORT_6);
MeRGBLed rgbled(8, 4);


int countdown = 0;
double lastDistance;
static double TOLERANCE = 2.5;
static int COUNTDOWN = 6;

void setup(){
  // Init serial communications
  Serial.begin(9600);
  // Init buzzer
  pinMode(SCL, OUTPUT);
  // Init 7 seg & set brighthness
  disp.init();
  disp.set(BRIGHTNESS_2);

  lastDistance = ultrasonic.distanceCm();
}

void loop() {
  // Get PIR reading
  int isDetected = pir.isHumanDetected();

  // Get ultrasonic distance
  double distance = ultrasonic.distanceCm();
  // Did distance change > tolerance?
  boolean distanceChange = abs(lastDistance - distance) > TOLERANCE;

  // Debug information
  Serial.print("Human? ");
  Serial.print((isDetected)?"Y ":"N ");
  Serial.print(" Dist: ");
  Serial.print(distance);
  Serial.print(" DistChg? ");
  Serial.print((distanceChange)?"Y ":"N ");

  disp.display(distance);

  // PIR detects and distance changed 
//  if (isDetected && distanceChange) {
  // PIR detects OR distance changed 
  if (isDetected || distanceChange) {
    countdown = COUNTDOWN;
  }
  if (countdown >0){
    buzz(50, 500);
    rgbled.setColor(1,255,0,0);
    rgbled.show();
    rgbled.setColor(3,255,0,0);
    rgbled.show();
    rgbled.setColor(2,0,0,255);
    rgbled.show();
    rgbled.setColor(4,0,0,255);
    rgbled.show();
    _delay(.1);
    rgbled.setColor(1,0,0,255);
    rgbled.show();
    rgbled.setColor(3,0,0,255);
    rgbled.show();
    rgbled.setColor(2,255,0,0);
    rgbled.show();
    rgbled.setColor(4,255,0,0);
    rgbled.show();
    _delay(.1);
    countdown -= 1;
  }
  else {
    rgbled.setColor(0,0,0,0);
    rgbled.show();
    _delay(.2);
  }
  // Save previous distance.
  lastDistance = distance;
  Serial.println();
}

void buzz(int duration, int note) {
  for (int i = 0; i < duration; ++i) {
    digitalWrite(SCL, HIGH);
    delayMicroseconds(note);
    digitalWrite(SCL, LOW);
    delayMicroseconds(note);
  }
}

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime)_loop();
}

void _loop(){
    
}

