const int speakerPin = 0;    // the pin that will be used for speaker output
const int tones[] = {1915, 1700, 1519, 1432, 2700}; // The last tone is the fail tone
const int ledpins[] = {13, 11, 10, 9}; //This was most convenient for soldering
const int buttonpins[] = {2, 4, 6, 8}; //This made soldering easy since I skipped every other pin

// Variables will change:
int buttonState[] = {0,0,0,0};         // current state of the button
int lastButtonState[] = {0,0,0,0}; 
int buttonPushCounter[] = {0,0,0,0}; 

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void setup() {
  // initialize the button pin as a input:
  randomSeed(analogRead(A0));
  int i;
  for (i = 0; i < 4; i = i + 1){
    pinMode(buttonpins[i], INPUT);
  }
  // initialize the LED as an output:
  for (i = 0; i < 4; i = i + 1){
    pinMode(ledpins[i], OUTPUT);
  }
  pinMode(speakerPin, OUTPUT);
  // initialize serial communication:
//  Serial.begin(9600);
}
int gamestate = 0;
int waitingforinput = 0;
int currentlevel = 1; // This is the level (also the number of button presses to pass to next level)
long randnum = 0; //initialize long variable for random number from 0-100.
int randnumint = 0; //initialize random integer for loop. Will be from 1-4 later.
int butwait = 500; //amount of time to wait for next button input (ghetto de-bounce)
int ledtime = 500; //amount of time each LED flashes for when button is pressed
int numlevels = 10; //number of levels until the game is won
int pinandtone = 0; //This integer is used when the sequence is displayed
int correct = 0; //This variable must be 1 in order to go to the next level
int speedfactor = 5; //This is the final speed of the lights and sounds for the last level. This increases as more games are won
int leddelay = 200; //Initializing time for LED. This will decrease as the level increases

void loop() {
  
int numarray[numlevels];
int userarray[numlevels];

//The following clears out both arrays for new games
int i;

if (gamestate == 0){
for (i = 0; i < numlevels; i = i + 1) {
   numarray[i]=0;
   userarray[i]=0;
  randnum = random(1,100);
    if (randnum <= 25) {
      randnumint = 0;
    }
    if (randnum > 25){
    randnumint = 1;
  }
    if (randnum > 50){
    randnumint = 2;
  }
    if (randnum > 75){
    randnumint = 3;
  }
  numarray[i]=randnumint;
//  Serial.println(numarray[i]);
}
if (i == numlevels){
 gamestate = 1; 
}
}


//The following will show the user the current sequence
if (waitingforinput == 0){
 delay (200);
i = 0;
for (i = 0; i < currentlevel; i= i + 1){
  leddelay = ledtime/(1+(speedfactor/numlevels)*(currentlevel - 1));
      pinandtone = numarray[i];
      digitalWrite(ledpins[pinandtone], HIGH);
      playTone(tones[pinandtone], leddelay);
      digitalWrite(ledpins[pinandtone], LOW);
      delay(100/speedfactor);
    }
    waitingforinput = 1;
}
i = 0;
int buttonchange = 0;    
int j = 0; // This is the current position in the sequence
while (j < currentlevel){    
    while (buttonchange == 0){
          for (i = 0; i < 4; i = i + 1){ 
          buttonState[i] = digitalRead(buttonpins[i]);
          buttonchange = buttonchange + buttonState[i];
        }
    }
     for (i = 0; i < 4; i = i + 1){
        if (buttonState[i] == HIGH) {
            digitalWrite(ledpins[i], HIGH);
            playTone(tones[i], ledtime);
            digitalWrite(ledpins[i], LOW);
            waitingforinput = 0;
            userarray[j]=i; 
            buttonState[i] = LOW;
            buttonchange = 0;
         }
       } 
        if (userarray[j] == numarray[j]){
            j++;  
            correct = 1;
            }
        else{
          
            correct = 0;
            i = 4;
            j = currentlevel;
            waitingforinput = 0;
        }
}

if (correct == 0){
  delay(300);
  i = 0;
  gamestate = 0;
  currentlevel = 1;
  for (i = 0; i < 4; i = i + 1){
         digitalWrite(ledpins[i], HIGH);
      }
          playTone(tones[4], ledtime);
   for (i = 0; i < 4; i = i + 1){
         digitalWrite(ledpins[i], LOW);   
       }
       delay (200);
  for (i = 0; i < 4; i = i + 1){
         digitalWrite(ledpins[i], HIGH);
      }
          playTone(tones[4], ledtime);
   for (i = 0; i < 4; i = i + 1){
         digitalWrite(ledpins[i], LOW);   
       }
       
       delay(500);
       gamestate = 0;
}

//Serial.println("The user sequence is");
//for (i = 0; i < numlevels; i = i + 1) {
//   Serial.println(userarray[i]);
//}
//Serial.println("The answers are");
//for (i = 0; i < numlevels; i = i + 1) {
//   Serial.println(numarray[i]);
//}

if (correct == 1){
    currentlevel++;
    waitingforinput = 0;
    }
if (currentlevel == numlevels){
  delay(500);
  // The following is the victory anmiation/sound:
  int notes[] = {2, 2, 2, 2, 0, 1, 2, 1, 2};
  int note = 0;
  int tempo[] = {200, 200, 200, 400, 400, 400, 200, 200, 600}; 
  int breaks[] = {100, 100, 100, 200, 200, 200, 300, 100, 200}; 
  for (i = 0; i < 9; i = i + 1){
  note = notes[i];
    digitalWrite(ledpins[note], HIGH);
    playTone(tones[note], tempo[i]);
    digitalWrite(ledpins[note], LOW);
    delay(breaks[i]);
  }
gamestate = 0;
currentlevel = 1;
numlevels = numlevels + 2;
speedfactor = speedfactor + 1;
    }
 
}
