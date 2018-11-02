#include <PinChangeInt.h>


//#include <EnableInterrupt.h>

//NOTE: LIKELY NEED TO ALTER ALL FUNCTIONS SO THEY WORK WITH BOTH PLAYER1 AND PLAYER2 AND PLAYER3, currently they are just wrriten using p1 variables and all
//could, uhhhhh do the thing where you use a generic parameter and pass an argument into the function depending on which button is hit?
//need 3 different ISRs??? idk maybe not, maybe only one and all buttons can access the same one. idk bro.
//+6 offset for player2
// assign LEDs and button to pins


//shift register declaration
int dataPin = 11;
int clockPin = 12;
int latchPin = 8;


int p1HitLed = A5;
int p2HitLed = 6;
int p3HitLed = 13;
int p3_3 = 7;

//
int p1Led = 0;
int p2Led = 0;
int p3Led = 0;

int p1Button = 2;
int p2Button = 3;
int p3Button = 4;
int p4Button = 10;

int scoreArray[3] = {0, 0, 0};


byte data = 0;

// declare variables
int delayTime = 500; // time delay between lights on/off

int p1Score = 0; //note: could do a function that does a reset of all the scores and starts a new game?
int p2Score = 0;
int p3Score = 0;
int scoreLimit = 10;



void setup() { //this is all good
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  Serial.begin(9600); //setup for the serial stuff, just so we can print to console for debugging purposes

  attachInterrupt(0, p1Input, FALLING); // specify interrupt routine
  attachInterrupt(1, p2Input, FALLING);
  attachPinChangeInterrupt(p3Button, p3Input, FALLING); // interrupt connected to pin 4
  attachPinChangeInterrupt(p4Button, ChangeDiff, FALLING); // interrupt connected to pin 10

  pinMode(p1Button, INPUT); //set player1 button pin as input
  pinMode(p2Button, INPUT); //set player2 button pin as input
  pinMode(p3Button, INPUT); //set player1 button pin as input
  pinMode(p4Button, INPUT); //set player1 button pin as input

  pinMode(p1HitLed, OUTPUT); //set LED pins as output
  pinMode(p2HitLed, OUTPUT); //set LED pins as output
  pinMode(p3HitLed, OUTPUT); //set LED pins as output
  pinMode(p3_3, OUTPUT); //set LED pins as output
}

void ChangeDiff() {
  delayTime = delayTime - 100;
}



void shiftWrite(byte out) {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  updateShiftRegister(out);

}


void updateShiftRegister(byte out)
{
  shiftOut(dataPin, clockPin, MSBFIRST, out);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

bool shiftRead(int pin) {


  byte check = 0;
  bitWrite(check, pin, 1);

  if (pin == 8) {
    if (digitalRead(p3_3) == HIGH) {
      return true;
    }
  }
  if ((check & data) != 0) {
    return true;
  }
  return false;
}



void moleWhacked(int score, int player) {

  String Output = ("Player " + String(player) + " has score " + String(score) + "\n");
  Serial.print(Output);
  if (player == 1) {
    digitalWrite(p1HitLed, HIGH); //illuminate score led

  }
  else if (player == 2) {
    digitalWrite(p2HitLed, HIGH); //illuminate score led

  }
  else if (player == 3) {
    digitalWrite(p3HitLed, HIGH); //illuminate score led

  }


}

void p1Input() { //ISR for the player1 button


  pinMode(p1Button, OUTPUT); //change button to a mode of output (so nothing can happen) as a means of disabling any further input during this LED light (preventing multiple points from one mole whack)
  if (shiftRead(p1Led) == true) { // if any led is on

    p1Score++; //increment score
    moleWhacked(p1Score, 1);
  }
}
void p3Input() { //ISR for the player1 button


  pinMode(p3Button, OUTPUT); //change button to a mode of output (so nothing can happen) as a means of disabling any further input during this LED light (preventing multiple points from one mole whack)
  if (shiftRead(p3Led) == true) { // if any led is on

    p3Score++; //increment score
    moleWhacked(p3Score, 3);
  }
}

void p2Input() { //ISR for the player2 button
  pinMode(p2Button, OUTPUT); //change button to a mode of output (so nothing can happen) as a means of disabling any further input during this LED light (preventing multiple points from one mole whack)
  if (shiftRead(p2Led) == true) { // if any led is on

    p2Score++; //increment score
    moleWhacked(p2Score, 2);
  }


}
void finishGame(int who) {
  pinMode(p1Button, OUTPUT); //change button to input again before next led lights to let them have a go at whacking it
  pinMode(p2Button, OUTPUT);
  pinMode(p3Button, OUTPUT);
  byte out = 0;
  updateShiftRegister(out);
  delay(700);
  bitWrite(out, 0, 1);
  String Output = ("PLAYER " + String(who) + " HAS WON !!!!!!!!!!\n");
  int j = 0;
  for (int i = 0; i < 100; i++) {
    if ((j > 1) && (j < 7)) {
      bitWrite(out, j + 1, 1);
      bitWrite(out, j - 2, 0);
    }
    else if (j == 1) {
      bitWrite(out, j + 1, 1);
      digitalWrite(p3_3, LOW); //illuminate score led
    }
    else if (j == 0) {
      bitWrite(out, j + 1, 1);
      bitWrite(out, 7, 0);
    }
    else if (j == 7) {
      digitalWrite(p3_3, HIGH); //illuminate score led
      bitWrite(out, j - 2, 0);
    }
    else if (j == 8) {
      bitWrite(out, 0, 1);
      bitWrite(out, j - 2, 0);
    }
    j++;
    if (j == 9) {
      j = 0;
    }
    updateShiftRegister(out);



    Serial.print(Output);
    delay((delayTime/6) * 2 );
    
   

  
  }
  pinMode(p1Button, INPUT); //change button to input again before next led lights to let them have a go at whacking it
  pinMode(p2Button, INPUT);
  pinMode(p3Button, INPUT);

  Output = ("PLAYER " + String(who) + " HAS WON !!!!!!!!!!\n");
  for (int k = 0; k < 10; k++) {
    Serial.print(Output);
  }

  p1Score = 0; //note: could do a function that does a reset of all the scores and starts a new game?
  p2Score = 0;
  p3Score = 0;

  scoreArray[who - 1] += 1;
  digitalWrite(p3_3, LOW);
  for (int c = 0; c < 3; c++) {
    Output = ( "Player " + String((c) + 1) + " has total score " + String(scoreArray[c]) + "\n"  );
    Serial.print(Output);
  }
 
}




void loop() {

  //constructing a byte with random bits selected
  p1Led = random(3);
  p2Led = random(3) + 3;
  p3Led = random(3) + 6;
  bitWrite(data, p1Led, 1);
  bitWrite(data, p2Led, 1);
  if (p3Led != 8) {
    bitWrite(data, p3Led, 1);
  } else {
    digitalWrite(p3_3, HIGH); //illuminate score led

  }
  shiftWrite(data); //light the LEDs

  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  Serial.print("");
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****
  //*****DO NOT REMOVE*****


  delay(delayTime); //hold led on for delayTime

  data = 0;
  shiftWrite(data);
  
  if (p3Led == 8) {

    digitalWrite(p3_3, LOW); //illuminate score led

  }

  digitalWrite(p1HitLed, LOW); //illuminate score led
  digitalWrite(p2HitLed, LOW); //illuminate score led
  digitalWrite(p3HitLed, LOW); //illuminate score led



 
  delay(delayTime); //hold leds off for delayTime
 
  pinMode(p1Button, INPUT); //change button to input again before next led lights to let them have a go at whacking it
  pinMode(p2Button, INPUT);
  pinMode(p3Button, INPUT);
  
  if (p1Score == scoreLimit) { //check if scorelimit has been reached
    finishGame(1); //if so call the finishGame function
     ChangeDiff();
  }
  else if (p2Score == scoreLimit) {
    finishGame(2);
     ChangeDiff();
  }
  else if (p3Score == scoreLimit) {
    finishGame(3);
     ChangeDiff();
  }


}
