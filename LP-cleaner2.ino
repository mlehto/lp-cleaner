#include <AccelStepper.h>
//4s 6rp = 1.5rps = 90RPM
//1:4 = 22,5 RPM
#define STEP 9
#define DIR 8
#define M0 5
#define M1 6
#define M2 7

#define DIRECTION1 2
#define DIRECTION2 3

#define SPEED1 19
#define SPEED2 18

AccelStepper stepperH(1 , STEP, DIR);

// Define maximum and minimum speed in steps per second
#define  MAX_SPEED 3800

int requestedSpeed = 0;
int actualSpeed = 0;
int requestedDirection = 1;
int actualDirection = 1;

long lastUpdated;
long lastUpdatedSpeed;

void setup() {
  stepperH.setMaxSpeed(MAX_SPEED);
  stepperH.setAcceleration(8000);
  
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  setSpeedToFraction(8);

  pinMode(DIRECTION1, INPUT_PULLUP);
  pinMode(DIRECTION2, INPUT_PULLUP);
  pinMode(SPEED1, INPUT_PULLUP);
  pinMode(SPEED2, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {  
  stepperH.runSpeed();

  if (lastUpdated + 250 < millis()) {
    bool speed1 = digitalRead(SPEED1) == LOW;
    bool speed2 = digitalRead(SPEED2) == LOW;
    bool direction1 = digitalRead(DIRECTION1) == LOW;
    bool direction2 = digitalRead(DIRECTION2) == LOW;
    
    if (speed1) {
      Serial.println("speed1");
      requestedSpeed = MAX_SPEED / 4;
    }
    if (speed2) {
      Serial.println("speed2");
      requestedSpeed = MAX_SPEED;
    }
    if (!speed1 && !speed2) {
      requestedSpeed = 0;
      requestedSpeed = MAX_SPEED / 2;
    }

    if (direction1) {
      requestedDirection = -1;
    }
    if (direction2) {
      requestedDirection = 1;
    }
    if (!direction1 && !direction2) {
      requestedSpeed = 0;
    }
    lastUpdated = millis();
  }

  if (lastUpdatedSpeed + 10 < millis()) {
    if (actualDirection != requestedDirection) {
      if (actualSpeed == 0) {
        actualDirection = requestedDirection;
      }
      else {
        requestedSpeed = 0;
      }      
    }
    
    if (requestedSpeed > actualSpeed) actualSpeed = actualSpeed + 100;
    if (requestedSpeed < actualSpeed) actualSpeed = actualSpeed - 100;

    stepperH.setSpeed(actualSpeed * actualDirection * 1.0);
    lastUpdatedSpeed = millis();
  }
}

void setSpeedToFraction(int divider) {
  switch (divider) {
    case 1:
      digitalWrite(M0, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      break;
    case 2:
      digitalWrite(M0, HIGH);
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      break;
    case 4:
      digitalWrite(M0, LOW);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      break;
    case 8:
      digitalWrite(M0, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      break;
    case 16:
      digitalWrite(M0, LOW);
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      break;
    case 32:
      digitalWrite(M0, HIGH);
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      break;
    default:
      digitalWrite(M0, HIGH);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      break;
  }  
}

