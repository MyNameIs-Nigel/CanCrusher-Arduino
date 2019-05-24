/* Can Crusher Device - Automation
 *  
 *  v0.10.00 Rel: 2019-05-02
 *  Author: Ernie Smith
 *  esmithiii2003@yahoo.com
 *  
 */

const int HomeSwitchPin = 7;
const int LimitSwitchPin = 8;
const int StartSwitchPin =9;
const int FwdPosPin=10;
const int FwdNegPin=11;
const int RevPosPin=12;
const int RevNegPin=13;
const unsigned long MaxElapsedForward_ms = 15000;

int iState = 0; 
int val = 0;
unsigned long StartTimer = 0;
unsigned long CurrentTime = 0;



/*
 * Int iState:
 * 0 = At Home Position
 * 1 = Moving Forward
 * 2 = At Limit position
 * 3 = Reversing
 * 
 */

void setup() {
  Serial.begin(9600);
  Serial.println("Setting pins");
  
  pinMode(HomeSwitchPin, INPUT);
  pinMode(LimitSwitchPin, INPUT);
  pinMode(StartSwitchPin, INPUT);
  pinMode(FwdPosPin, OUTPUT);
  pinMode(FwdNegPin, OUTPUT);
  pinMode(RevPosPin, OUTPUT);
  pinMode(RevNegPin, OUTPUT);
  //Serial.println("Calling StopMovement()");
  
  StopMovement();
}

void StopMovement(){
  digitalWrite(FwdPosPin, HIGH);
  digitalWrite(FwdNegPin, HIGH);
  digitalWrite(RevPosPin, HIGH);
  digitalWrite(RevNegPin, HIGH);
  Serial.println("StopMovement()");
}

void ForwardMovement(){
  val = digitalRead(LimitSwitchPin); 
  if (val == HIGH) {
    StopMovement();
    iState = 2;
  } else {
    if (StartTimer == 0) {
      StartTimer = millis();
    }
    CurrentTime = millis();
    if ((CurrentTime-StartTimer)>MaxElapsedForward_ms){
      StopMovement();
      iState=2;
    }else {
      if (iState != 1) {
        Serial.print("ForwardMovement Setting Pins iState= " );
        Serial.println (iState);
        iState = 1;
        digitalWrite(RevPosPin, HIGH);
        digitalWrite(RevNegPin, HIGH);
        digitalWrite(FwdPosPin, LOW);
        digitalWrite(FwdNegPin, LOW);
      }
    }
  }
}

void ReverseMovement(){
  val = digitalRead(HomeSwitchPin); 
  StartTimer=0;
  if (val == HIGH) {
    StopMovement();
    iState = 0;
  } else {
    if (iState != 3) {
      Serial.print("ReverseMovement Setting Pins iState= " );
      Serial.println (iState);
      iState = 3;
      digitalWrite(FwdPosPin, HIGH);
      digitalWrite(FwdNegPin, HIGH);
      digitalWrite(RevPosPin, LOW);
      digitalWrite(RevNegPin, LOW);
    }
  }
}

void loop() {
  switch(iState){
    case 0: // At Home Position (or restarting)
      val = digitalRead(StartSwitchPin);
      if (val == HIGH) {
        ForwardMovement();
      } 
      break;
    case 1: // Moving Forward
      ForwardMovement();
      break;
    case 2: // At limit position
      ReverseMovement();
      break;
    case 3: // Reversing
      ReverseMovement();
      break;
  }
  delay(1);

}
