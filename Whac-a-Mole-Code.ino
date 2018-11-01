//NOTE: LIKELY NEED TO ALTER ALL FUNCTIONS SO THEY WORK WITH BOTH PLAYER1 AND PLAYER2 AND PLAYER3, currently they are just wrriten using p1 variables and all
//could, uhhhhh do the thing where you use a generic parameter and pass an argument into the function depending on which button is hit?
//need 3 different ISRs??? idk maybe not, maybe only one and all buttons can access the same one. idk bro.
//+6 offset for player2
// assign LEDs and button to pins
int ledPin[] = {4,5,6,7,8,9};
int p1Button = 2;
int p2Button = 3;
int p3Button;
int p1ScoreLED = 10;
int p2ScoreLED = 11;
int p3ScoreLED;

// declare variables
int delayTime = 500; // time delay between lights on/off
int randNum1;
int randNum2;
int randNum3;
int p1Score = 0; //note: could do a function that does a reset of all the scores and starts a new game?
int p2Score = 0;
int p3Score = 0;
int scoreLimit = 10;
int servo;

void setup() {
  Serial.begin(9600); //setup for the serial stuff, just so we can print to console for debugging purposes
  
  attachInterrupt(0, p1Input, FALLING); // specify interrupt routine
  attachInterrupt(1, p2Input, CHANGE);
  
  for (int i=0; i<6; i++){
    pinMode(ledPin[i], OUTPUT); //set LED pins as output
  }
  
  pinMode(p1Button, INPUT); //set player1 button pin as input
  pinMode(p2Button, INPUT); //set player2 button pin as input
  pinMode(p1ScoreLED, OUTPUT); //set score led pin as output
  pinMode(p2ScoreLED, OUTPUT);
}

void moleWhacked(int score, int scoreLED){
  Serial.print(score); //print score to console
  Serial.print("\n");
  digitalWrite(scoreLED, HIGH); //illuminate score led
  }

void p1Input() { //ISR for the player1 button
  if (digitalRead(ledPin[randNum1]) == HIGH){ // if any led is on
      Serial.print("P1 ISR\n");
      p1Score++; //increment score
    moleWhacked(p1Score, p1ScoreLED);
  }
  pinMode(p1Button, OUTPUT); //change button to a mode of output (so nothing can happen) as a means of disabling any further input during this LED light (preventing multiple points from one mole whack)
}

void p2Input(){ //ISR for the player2 button
  if (digitalRead(ledPin[randNum2]) == HIGH){ // if any led is on
    Serial.print("P2 ISR\n");
    p2Score++; //increment score
    moleWhacked(p2Score, p2ScoreLED);
  }
  pinMode(p2Button, OUTPUT);
}

void difficultlyInc() {

}

void finishGame(int score, int button){ //function that activates when the score limit is reached
  score = 0; //reset the players score
  Serial.print("game over!\n"); //print to console for debugging purposes
  pinMode(button, OUTPUT); //disable any input, well, since game is over
    
  for (int i=0; i<10; i++){
    digitalWrite(ledPin[0], HIGH);
    delay(100);
    digitalWrite(ledPin[1], HIGH);
    delay(100);
    digitalWrite(ledPin[2], HIGH);
    delay(100);
    digitalWrite(ledPin[2], LOW);
    delay(100);
    digitalWrite(ledPin[1], LOW);
    delay(100);
    digitalWrite(ledPin[0], LOW);
    delay(100);
    }
  pinMode(button, INPUT); //enable button again for next game
}

void loop() {
  //noInterrupts();
  randNum1 = random(3); //pick a random number
  randNum2 = random(3) + 3;
  
  digitalWrite(ledPin[randNum1], HIGH); //light the player1 LED with the random number
  digitalWrite(ledPin[randNum2], HIGH); //light the player2 LED
  delay(delayTime); //hold led on for delayTime
  
  digitalWrite(ledPin[randNum1], LOW);
  digitalWrite(ledPin[randNum2], LOW);
  delay(delayTime); //hold leds off for delayTime
  
  if(digitalRead(p1ScoreLED) == HIGH | p2ScoreLED == HIGH){
    digitalWrite(p1ScoreLED, LOW); //turn off score led
    digitalWrite(p2ScoreLED, LOW);
  }

  if(digitalRead(p2ScoreLED) == HIGH){
    digitalWrite(p2ScoreLED, LOW);
    }
  
  pinMode(p1Button, INPUT); //change button to input again before next led lights to let them have a go at whacking it
  pinMode(p2Button, INPUT);
  
  if (p1Score == scoreLimit){ //check if scorelimit has been reached
    finishGame(p1Score, p1Button); //if so call the finishGame function
    }
  else if (p2Score == scoreLimit){
    finishGame(p2Score, p2Button);
    }
}
