# Battleship
Arduino implementation of the game Battleship

Author: ALAN FAN and DAVID LIANG

Course: UIC CS 362, Computer Design
        FALL 2016
        Pofessor Cynthia Taylor
        
FILES INCLUDED:
  -Battleship.ino                -> the actual Arduino code for the program
  -Final_Project_Sketch2_bb.png  -> an image of the breadboard circuitry
  -FinalProjReport.pdf           -> a write up detailing my Final Project experience
  
  
**************************************************************************************************
                                       PROGRAM DESCRIPTION
**************************************************************************************************
1. Setup Phase
  During the setup phase you are given the option of setting up a 2x1 ship, 3x1 ship, and a
  4x1 ship in respective order. During that time you are given an option to give them a
  horizontal or vertical orientation with the press of the red button or yellow button
  respectively. After that you can press the red button to move them into a designated
  horizontal position and the yellow button to confirm. After that you can press the red
  button to move the ship vertically and yellow to confirm again. Once the setup for the 3
  ships are complete, the LCD monitor will prompt “Waiting for opponent to finish setting
  up”. Once both of the players are done setting up the ships they are moved to the second
  stage of the game.
2. Playing Phase
  After all the ships have been set up by both players the players are free to play the game.
  The way moves are made are the same as the way that ships were set up. The game is
  turn based so it waits until both players have confirmed their moves and then once they
  are both performed the game proceeds. There are two different messages that the
  player will receive depending on whether they hit a target or missed a target. On a hit,
  the player will receive a message saying: “you have now hit a ship (number of ship spots
  left - 1) spots left to bomb” On the flipside the player that misses will have a “you have
  missed a ship and (number of ship spots left) spots left to bomb”. Once the game has
  concluded and one player has hit the maximum number of ship spots and in this case it
  was 9, the LCD monitor will display “You have won!” and exit appropriately.
  
  
**************************************************************************************************
                                       CIRCUITRY DESCRIPTION
**************************************************************************************************
The parts used for this project were
  - 2x Arduino UNO's
  - 2x liquid crystal display (LCD) 16x2 monitors
  - 4x buttons
  - 2 potentiometers
  - 2x LED 8x8 dot matrices (red)
  - 4x 8 bit shift registers
  - 4x 10kΩ resistors
  - 18x 220Ω resistors
  - many wires and female-to-female wire connectors
  
A Fritzing diagram of the circuitry can be found in the repository as PNG file and also in the PDF
report on page 6.  The only omitted part of the circuitry in the Fritzing diagram is the wire
connecting the TX port of one of Arduino to the RX port of the other, and a wire the connecting
the RX port of the former to the TX port of the latter.



**************************************************************************************************
                                       CODE USE INFORMATION
**************************************************************************************************
Feel free to use this code with proper author citation.
I would love to hear any feedback.
You may reach me at alan (dot) w (dot) fan (at) gmail (dot) com
