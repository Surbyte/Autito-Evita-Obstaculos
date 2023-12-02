#include <Arduino.h>

// Jugaad - 101//
// ARDUINO OBSTACLE AVOIDING CAR//
// Install Necessary Libraries//
#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>
#include <LibPrintf.h>


#define TRIG_PIN          A0
#define ECHO_PIN          A1
#define MAX_DISTANCE      100
#define MAX_SPEED         190
#define MAX_SPEED_OFFSET  20
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;
boolean goesForward = false;
int distance = 100;
int speedSet = 0;
unsigned long rampaTime;

int readPing() {
  int cm =  sonar.ping_cm();
  if (cm == 0)   cm = 250;
  return cm;
}

int lookRight()
{
  myservo.write(50);
  delay(100);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
}

int lookLeft()
{
  myservo.write(170);
  delay(100);
  int distance = readPing();
  delay(100);
  myservo.write(115);
  return distance;
}


void moveStop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  //static int speed = 0;
  //static byte acelero = 0;

  //printf("ADELANTE goesFoward %d acelero : %d\n", goesForward?1:0, acelero);
  printf("ADELANTE goesFoward %d\n", goesForward?1:0);
  if (!goesForward)   {
    // switch(acelero) {
    //   case 0: motor1.run(FORWARD);
    //           motor2.run(FORWARD);
    //           motor3.run(FORWARD);
    //           motor4.run(FORWARD);
    //           acelero = 1;
    //           rampaTime = millis();
              
    //           break;
    //   case 1: if (millis()- rampaTime > 5UL)  {
    //               if (speedSet < MAX_SPEED) {
    //                   speedSet +=2; 
    //                   printf("vel : %3d\n", speedSet);
    //                   rampaTime = millis();
    //               } 
    //               else {
    //                 goesForward = true;
    //                 speedSet = 0;
    //                 acelero = 0;
    //               }
    //           }
    //           break;

    // }
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    delay(200);
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)  {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2)   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  printf("GIRO IZQUIERDA\n");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  printf("GIRO DERECHA\n");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}


void setup() {
  myservo.attach(10);
  myservo.write(115);
  delay(2000);
  distance = readPing();
  Serial.begin(115200);
}

void loop() {
  int distanceR = 0;
  int distanceL = 0;
  delay(40);

  if (distance <= 25)
  {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    // delay(200);
    distanceR = lookRight();
    // delay(200);
    distanceL = lookLeft();
    // delay(200);
    if (distanceR >= distanceL)     {
      turnRight();
      moveStop();
    }
    else     {
      turnLeft();
      moveStop();
    }
  }
  else   {
    moveForward();
  }
  distance = readPing();
}