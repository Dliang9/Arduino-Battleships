/*
AUTHOR: Alan Fan
        
COURSE: UIC CS 342
        Fall 2016
        Professor Cynthia Taylor
*/


#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int redButtonPin = 14;
int yellowButtonPin = 15;
int playerBoard[8][8];

int colClockPin = 8; int colLatchPin = 9;
int colDataPin = 10;  int rowClockPin = 19;
int rowLatchPin = 18;
int rowDataPin = 17;
byte ledStates[8] = {B01110000, B11111000, B11111100, B01111110, B01111110, B11111100, B11111000, B01110000};
byte GroundLEDs [8] = {B01111111, B10111111, B11011111, B11101111, B11110111, B11111011, B11111101, B11111110}; 

int enemyBoard[8][8];
int spotsBombed[8][8];

int hitCounter = 0;
int placesToHit = 0;

void setup() {
  pinMode(redButtonPin, INPUT);
  pinMode(redButtonPin, INPUT);
  lcd.begin(16, 2);
  pinMode(rowLatchPin, OUTPUT);
  pinMode(rowClockPin, OUTPUT);
  pinMode(rowDataPin, OUTPUT);
  
  pinMode(colLatchPin, OUTPUT);
  pinMode(colClockPin, OUTPUT);
  pinMode(colDataPin, OUTPUT);      
  
  digitalWrite(colLatchPin, LOW);
  shiftOut(colDataPin, colClockPin, MSBFIRST, B11111111);  
  digitalWrite(colLatchPin, HIGH);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j ++) {
      playerBoard[i][j] = 0;
      spotsBombed[i][j] = 0;
      enemyBoard[i][j] = 0;
    }
  }
  placesToHit = 9;

  Serial.begin(9600);
  printStringScrolling("Welcome to Battleship by David and Alan ");
  printStringScrolling("First you need to set up your ships! ");
  printStringScrolling("Set up your Destroyer ");
  printStringScrolling("Press red for a horizontal ship, yellow for vertical ");
  setBoat(2);
  printStringScrolling("Set up your cruiser ");
  printStringScrolling("Press red for a horizontal ship, yellow for vertical ");
  setBoat(3);
  printStringScrolling("Set up your battleship ");
  printStringScrolling("Press red for a horizontal ship, yellow for vertical ");
  setBoat(4);

  communicateBoards();
  printStringScrolling("Finished setting up board, now you can play, may the best bomber win ");
}

void loop() {
  
  bool validInput = false;
  int col;
  int row;

  while (!validInput) {
    printStringScrolling("Press red to pick the bomb column, yellow to confirm. ");
    col = getBombCol();
    printStringScrolling("Press red to pick the bomb row, yellow to confirm. ");
    row = getBombRow(col);

    if (spotsBombed[row][col] == 1) {
      printStringScrolling("You have already bombed that spot, ");
    }
    else {
      spotsBombed[row][col] = 1;
      validInput = true;
    }
  }

  Serial.write(B1);

  printStringStatic("WAITING FOR", "OPPONENT");
  while(Serial.available() != 1)
  {
    ; //wait for opponent
  }

  Serial.read(); //process B1
  printStringStatic("ENEMY HAS ALSO", "MADE MOVE");
  delay(3000);
  lcd.clear();

  if (enemyBoard[row][col] == 1) {
    placesToHit--;
    hitCounter++;
    printStringScrolling("You have now hit " + String(hitCounter) + " spot(s). Only " + String(placesToHit) + " left! ");
  }
  else {
    printStringScrolling("Miss: you still have " + String(placesToHit) + " spot(s) left. ");
  }

  if (placesToHit <= 0) {
    printStringStatic("VICTORY!", "VICTORY!");
    while(1)
    {
      ;//game over
    }
  }
}

void GroundCorrectLED (byte states){
  
    digitalWrite(colLatchPin, LOW);
    shiftOut(colDataPin, colClockPin, MSBFIRST, states);  
    digitalWrite(colLatchPin, HIGH);
    
}
void SetStates (byte statesx){
       
    digitalWrite(rowLatchPin, LOW);
    shiftOut(rowDataPin, rowClockPin, LSBFIRST, statesx);  
    digitalWrite(rowLatchPin, HIGH);
    
}

void printStringScrolling(String myString) {
  int start = 0;
  int last = 0;
  
  for (int i = 0; i < myString.length()+16; i ++) {
    lcd.clear();
    lcd.print(myString.substring(start,last));
    if (last < 16) {
      last++;
    }
    else if (last == myString.length() - 1) {
      start++;
    }
    else {
      start++; last++;
    }
    delay(180);
  }
}

void setBoat (int boatLength) {

  int boatOrientation;
  int buttonColorPin = indefButtonPress();
  if (buttonColorPin == yellowButtonPin){
     boatOrientation = 2;
     printStringScrolling("You have selected a vertical ship ");
  }
  else if (buttonColorPin == redButtonPin) {
     boatOrientation = 1;
     printStringScrolling("You have selected a horizontal ship ");
  }

  printStringScrolling("Press the red button to move the ship right horizontally, yellow to confirm ");
  int boatCol = setBoatHorizontally(boatOrientation, boatLength);
  printStringScrolling("Press the red button to move the ship down vertically, yellow to confirm ");
  setBoatVertically(boatOrientation, boatLength, boatCol);
}

int indefButtonPress () {
  int yellowOldState = LOW;
  int redOldState = LOW;
  while (true) {
    int currYellowState = digitalRead(yellowButtonPin);
    int currRedState = digitalRead(redButtonPin);
    
    if (currYellowState != yellowOldState && currYellowState == LOW){
      return yellowButtonPin;
    }
    else if (currRedState != redOldState && currRedState == LOW) {
       return redButtonPin;
    }
    yellowOldState = currYellowState;
    redOldState = currRedState;
    delay(15);
  }
}

int setBoatHorizontally(int orientation, int boatLength) {
  bool done;
  int currCol;
  int tempBoard[8][8];
  if (orientation == 1) { //horizontal
    
    int boat[boatLength];
    for (int i = 0; i < boatLength; i++)
      boat[i] = i;
  
    currCol= 0;
    
    while (1) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          tempBoard[i][j] = playerBoard[i][j];
        }
      }
  
      for (int i = 0; i < boatLength; i++)
        tempBoard[0][boat[i]] = 1;
      
      printBoardOnLEDMatrix(tempBoard);
      int buttonPressed = timedButtonPress();
      
      if (buttonPressed == yellowButtonPin){
        return currCol;
      }
      else if (buttonPressed == redButtonPin) {
        currCol++;
        if(currCol+boatLength > 8) {
          currCol = 0; //reset column (warp edges)
        }
        for (int i = 0; i < boatLength; i++) {
            tempBoard[0][boat[i]] = 0;
            boat[i] = currCol + i;
            tempBoard[0][currCol + i] = 1;
        }
      }
    } 
  }
  else if (orientation == 2) { //vertically
    currCol= 0;
    
    while (1) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          tempBoard[i][j] = playerBoard[i][j];
        }
      }
  
      for (int i = 0; i < boatLength; i++)
        tempBoard[i][currCol] = 1;
      
      printBoardOnLEDMatrix(tempBoard);
      int buttonPress = timedButtonPress();
      if (buttonPress == yellowButtonPin){
        return currCol;
      }
      else if (buttonPress == redButtonPin) {
        for (int i = 0; i < boatLength; i++) {
          tempBoard[i][currCol] = 0;
        }
        currCol++;
        if(currCol >= 8) {
          currCol = 0;
        }
        for (int i = 0; i < boatLength; i++) {
            tempBoard[i][currCol] = 1;
        }
      }
    }
  }
}

void printBoardOnLEDMatrix(int tempBoard[8][8]) {
  for (int z = 0; z <5; z++) {
    for(int i=0;i<8;i++){  
        
      byte data = B0;
      for (int j = 0; j < 8; j++) {
        bitWrite(data, j, tempBoard[j][i]);
      }
      SetStates(data);
      GroundCorrectLED (GroundLEDs[i]);  
      
      delay(1);
      
      digitalWrite(colLatchPin, LOW);
      shiftOut(colDataPin, colClockPin, MSBFIRST, B11111111);  
      digitalWrite(colLatchPin, HIGH);

      delay(1);
    }
  }
}

int timedButtonPress () {
  int yellowOldState = LOW;
  int redOldState = LOW;
  for (int i = 0; i < 300; i++) {
    int currYellowState = digitalRead(yellowButtonPin);
    int currRedState = digitalRead(redButtonPin);
      
    if (currYellowState != yellowOldState && currYellowState == LOW){
      return yellowButtonPin;
    }
    else if (currRedState != redOldState && currRedState == LOW) {
       return redButtonPin;
    }
    yellowOldState = currYellowState;
    redOldState = currRedState;
    delay(1);
  }
  return -1;
}

void setBoatVertically(int orientation, int boatLength, int boatCol) {
  bool done;
  int currRow;
  int tempBoard[8][8];
  
  if (orientation == 2) { //vertical
    int boat[boatLength];
    for (int i = 0; i < boatLength; i++) {
      boat[i] = i;
    }
  
    done = false;
    currRow = 0;
    
    while (!done) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          tempBoard[i][j] = playerBoard[i][j];
        }
      }
  
      for (int i = 0; i < boatLength; i++) 
        tempBoard[boat[i]][boatCol] = 1;
     
      
      printBoardOnLEDMatrix(tempBoard);

      int buttonPressed = timedButtonPress();
      if (buttonPressed == yellowButtonPin){
        bool validBoat = true;
        for (int i = 0; i < boatLength; i++)
        {
          if (playerBoard[i + currRow][boatCol] == 1)
          {
            printStringScrolling("ERROR: Overlap ");
            validBoat = false;
            break;
          }
        }
        if (validBoat)
        {
          for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
              playerBoard[i][j] = tempBoard[i][j];
            }
          }
          done = true;
        }
      }
      else if (buttonPressed == redButtonPin) {
        currRow++;
        if(currRow+boatLength > 8) {
          currRow = 0;
        }
        for (int i = 0; i < boatLength; i++) {
            boat[i] = currRow + i;
        }
      }
    }
  }
  else if (orientation == 1) { // horizontal
    done = false;
    currRow = 0;
    
    while (!done) {
  
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          tempBoard[i][j] = playerBoard[i][j];
        }
      }
  
      for (int i = 0; i < boatLength; i++) {
        tempBoard[currRow][boatCol + i] = 1;
      }
      
      printBoardOnLEDMatrix(tempBoard);
      int buttonPressed = timedButtonPress();
      if (buttonPressed == yellowButtonPin){
        bool validBoat = true;
        for (int i = 0; i < boatLength; i++)
        {
          if (playerBoard[currRow][boatCol+i] == 1)
          {
            printStringScrolling("ERROR: Overlap ");
            validBoat = false;
            break;
          }
        }
        if (validBoat)
        {
          for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
              playerBoard[i][j] = tempBoard[i][j];
            }
          }
          done = true;
        }
      }
      else if (buttonPressed == redButtonPin) {
        currRow++;
        if(currRow >= 8) {
          currRow = 0;
        }
      }
    }
  }
}

int getBombCol() {
  int bombCol = 0;
  bool done = false;
  int prevVal;
  int tempBoard[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      tempBoard[i][j] = spotsBombed[i][j];
    }
  }
  
  while (!done) {
    prevVal = spotsBombed[0][bombCol];
    tempBoard[0][bombCol] = 1;
      
    printBoardOnLEDMatrix(tempBoard);
    int buttonPressed = timedButtonPress();
    if (buttonPressed == yellowButtonPin){
      return bombCol;
    }
    else if (buttonPressed == redButtonPin) {
      tempBoard[0][bombCol] = prevVal;
      bombCol++;
      if(bombCol >= 8) {
        bombCol = 0;
      }
    }
  }
}

int getBombRow(int bombCol) {

  int bombRow = 0;
  bool done = false;
  int prevVal;
    
  int tempBoard[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      tempBoard[i][j] = spotsBombed[i][j];
    }
  }
  
  while (!done) {
    prevVal = spotsBombed[bombRow][bombCol];
    tempBoard[bombRow][bombCol] = 1;
      
    printBoardOnLEDMatrix(tempBoard);
    int buttonPressed = timedButtonPress();
    if (buttonPressed == yellowButtonPin){
      return bombRow;
    }
    else if (buttonPressed == redButtonPin) {
      tempBoard[bombRow][bombCol] = prevVal;
      bombRow++;
      if(bombRow >= 8) {
        bombRow = 0;
      }
    }
  }
}

void communicateBoards() {
  
  byte boardToSend[8];
  for (int i = 0; i < 8; i++)
  {
    boardToSend[i] = getRowAsBytes(i);
  }
  
  Serial.write(boardToSend, 8);

  printStringStatic("WAITING FOR", "OPPONENT");
  while (Serial.available() != 8)
  {
    ; //wait
  }

  byte boardReceived[8];
  Serial.readBytes(boardReceived, 8);
  for (int i = 0; i < 8; i++)
  {
    byte rowByte = boardReceived[i];
    for (int j = 0; j < 8; j++)
    {
      int x = rowByte & (1 << j);
      if (x != 0)
      {
        x = x >> j;
      }
      enemyBoard[i][j] = x;
    }
  }
  
  printStringStatic("BOARD SETUP","COMPLETE");
  delay(3000);
  lcd.clear();
}

byte getRowAsBytes(int row)
{
  byte returnVal = B00000000;
  for (int i = 0; i < 8; i++)
  {
    if (playerBoard[row][i] == 1)
    {
      returnVal = returnVal | (B1 << i);
    }
  }
  return returnVal;
}

void printStringStatic(String upper, String lower)
{
  lcd.clear();
  lcd.print(upper);
  lcd.setCursor(0,1);
  lcd.print(lower);
}

