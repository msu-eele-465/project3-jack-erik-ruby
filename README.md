# EELE 465 Project 3

## Introduction
In this project, a keypad will be used to select from different time-varying patterns displayed on eight LEDs using your MSP430 and a breadboard. Patterns will be selected based on which button is pressed on your keypad. An unlock code will be required to begin the display of the LED patterns. An RGB LED will be used a status indicator.

## Architecture
There are a total of 3 external devices: RGB LED, LED Bar, and a 4x4 membrane keypad. The architecture of the project using these devices with the MSP430 is as follows.
<img src="https://github.com/msu-eele-465/project3-jack-erik-ruby/blob/main/docs/resources/SoftwareArchitecture.svg" alt="architecture" style="width:50%; height:auto;"> 

## Flowchart
Project has a locked state and unlocked state. After unlocking, the user can choose a pattern. The RGB LED and LED Bar change color depending on the state and pattern.
<img src="https://github.com/msu-eele-465/project3-jack-erik-ruby/blob/main/docs/resources/Project3_Charts.svg" alt="flowchart" style="width:75%; height:auto;"> 

## Circuit diagram

<img src="https://github.com/msu-eele-465/project3-jack-erik-ruby/blob/main/docs/resources/CircuitDiagram.svg" alt="circuit diagram" style="width:50%; height:auto;">
