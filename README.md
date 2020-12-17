# Snake Game Console

1. [Introduction](#Introduction)
2. [LCD Pinout](#LCD-Pinout)
3. [LCD Operation](#LCD-Operation)
4. [Game Operation](#Game-Operation)
5. [Used Libraries](#Used-Libraries)
6. [Issues and Solutions](#Issues-and-Solutions)


## Introduction

The objective of this task was to implement a snake game in a Microcontroller Atmega328P to be displayed by a LCD Nokia 5110. The displayed would be updated by the Microcontroller through the SPI protocol.

Besides the Arduino Board containing the Microcontroller, we used 4 buttons to control the snake movement and 1 extra button for the options, as well as the wires and resistors to perform the connections.


<p align="center">
  <img width="270" height="360" src="/img/131903153_230500798617574_727849156976634678_n.jpg">
</p>
<p align="center">
  <sub>
Snake Game in the LCD
  </sub>
</p>




## LCD Pinout

The LCD Nokia 5110 contains 8 pins with the following features:


| Pin Number   |      Pin Label     |    Pin Features |    Input/Output   |  Notes | 
|----------|:-------------:|:-------------:|:-------------:|:-------------:|
| 1 | VCC | Positive power supply | Input | Supply range is between 2.7V and 3.3V |
| 2 | GND | Ground | Input | Ground |
| 3 | SCE | Chip select | Input | Enables the Transmission of Data (Active low) |
| 4 | RST | Reset | Input | Resets the Display |
| 5 | D/C | Mode select | Input | Select between command mode (low) and data mode (high). |
| 6 | DN(MOSI) | Serial data in | Input | Pin in which the Data is Transmitted |
| 7 | SCLK | Serial clock | Input | Clock signal: 0.0 to 4.0 Mbits/s |
| 8 | LED | LED backlight supply | Input | Controls the Display Light through the Current |


<p align="center">
  <img width="333" height="333" src="/img/524dc9ea757b7f616e8b4569.png">
</p>
<p align="center">
  <sub>
LCD Pins
  </sub>
</p>



## LCD Operation


After the declaring the LCD pins as inputs and the initialization of the SPI through the first thing we needed to do was to configure the LCD. In order to do that we sent command messages, with the lenght of 1 byte in which every combination of the 8 bits leads to a specific configuration command (Instruction).

The set of instructions is showned in the image below:


<p align="center">
  <img width="511" height="512" src="/img/unnamed.png">
</p>
<p align="center">
  <sub>
LCD Instructions
  </sub>
</p>



### Send a Command

In order to send a command, the first thing we need to do is to enabled the Transmission of Data, we can do that by setting the SCE pin LOW. Then we need to enable the Command Mode by setting the pin D/C LOW as well. The last thing is just writing the command (1 byte) in the Serial Pin (DN(MOSI)) and wait till the data is sent, by monitoring the SPI Interrupt Flag in the SPI Status Register or waiting 8 clock cycles. 

After that we need to disable the Transmission of Data to prevent sending random data. We do that setting the SCE pin back to HIGH.




### Sending Data to the Screen 

In order to send data, the first thing we need to do is to enabled the Transmission of Data, we can do that by setting the SCE pin LOW. Then we need to enable the Data Mode by setting the pin D/C HIGH. The last thing is just writing the command (1 byte) in the Serial Pin (DN(MOSI)) and wait till the data is sent, by monitoring the SPI Interrupt Flag in the SPI Status Register or waiting 8 clock cycles. 

After that we need to disable the Transmission of Data to prevent sending random data. We do that setting the SCE pin back to HIGH.

To get more information about the SPI Registers check the ATMega328P datasheet: [ATMega328P Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)


### Screen Display Control

The LCD has a resolution of 48 x 84 pixels, making a total of 4032 pixels. However we can't control each pixel individually, we can only control pixels in blocks of 8, with each pixel being a single bit. With that in mind, we can control and have access to the display state using an array of bytes (8 bits) with a position for each block of 8 pixels, making an array with 504 positions.


The LCD Display is divided in 6 banks, each one with 84 columns with each column representing 1 byte, and every pixel 1 bit. When the respective bit is set to 1 the pixel is turned ON and when is set to 0 it turned OFF, having the Least Significant Bit on the Top and the Most Significant Bit on the Botton of the column. 


<p align="center">
  <img width="640" height="406" src="/img/PCD8544.png">
</p>
<p align="center">
  <sub>
Display Address Structure
  </sub>
</p>


To update the display we can run a loop through each position of the array and send the value as the data. The LCD will update that position in the screen and then advance to the next position. When we pretend to change only the state of a few pixels, we can make use of the SET ADDRESS OF RAM instructions presented on the [LCD Operation](#LCD-Operation), to jump to the desired position in the screen and only change that one, instead of running through every position everytime a change occurs.

According with the respective configuration, we can update the display of two different ways, Horizontal Addressing or Vertical Addressing.


<p align="center">
  <img width="367" height="220" src="/img/2.PNG">
</p>
<p align="center">
  <sub>
Horizontal Addressing
  </sub>
</p>


<p align="center">
  <img width="367" height="220" src="/img/11.PNG">
</p>
<p align="center">
  <sub>
Vertical Addressing
  </sub>
</p>



## Game Operation

The Game was developed with a bunch of different menus, besides the game itself. 
A start menu that allows the game to begin. A pause menu that saves the current information about the game and allows to continue the game later or to
quit it and a Game Over Menu that shows the score, after the snake dies.
And a Win Menu that shows that the player reached the goal.

In order to allow the logic transmission between menus and the continuity of the game, it was developed using a State Machine as showned in the figure below: 


<p align="center">
  <img width="346" height="429" src="/img/state machine.png">
</p>
<p align="center">
  <sub>
Game State Machine
  </sub>
</p>



## Used Libraries

To allow a simplest control of the display, we used a library to control the writing of data, commands, the display update through SPI, based on the following Library: [Github: Nokia 5110 AVR Library](https://github.com/LittleBuster/avr-nokia5110) but with some minor modifications.


## Issues and Solutions

Due to some bugs related to the undesired shifting of the image on the LCD (bugs that we couldn't debug), we opted for sending each bit of data one by one on the Serial Pin and change the clock state for each one, instead of just writing the whole byte of data in the SPI Data Register. That allowed us to have more control in the data that was being sent to the LCD Driver and stopped the undesired shifting of the image.


