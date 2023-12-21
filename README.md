Final project - Crazy Kitchen
--- 
Group : 9  
Member : B09901043 陳昱樺 B09901134 林容丞

Demo video
---
Demo : https://www.youtube.com/watch?v=qfzO6oCcIFI

Introduction
---
This is an interesting game for two players. Players need to collect various ingredients first, then process them into dishes. Finally, players should take dishes to the serving area to get points. There are two modes in this game, versus mode and team mode. We believe players can have fun in Crazy Kitchen.

Prerequisites
---
There are six things a user should prepare to run this project:
* Mbed Studio
* STM32 development board
* Joystick * 2
* Button * 3
* Buzzer
* Python

Hardware connection
---
* Player 1 jotstick : A0 for x-axis and A1 for y-axis
* Player 2 jotstick : A2 for x-axis and A3 for y-axis
* Player 1 button : D8
* Player 2 button : D7
* Pausing button : D4
* Buzzer : D9

Usage
---
Here are some steps to run this project properly:
1. Build the environment for socket programming. One can refer to this website to finish this step. [https://github.com/Chen-Yuhua/HW2-Socket-Programming-and-Data-Visualization]
2. Replace main.cpp with the one in this repository.
3. Include pitches.h into the project.
4. Finish hardware connection.
5. Run main.cpp in Mbed Studio and main.py in Python.

License
---
None.

Acknowledgements
---
Here are some resources we refer to to finish this project:
* Joystick module usage  
[https://shop.mirotek.com.tw/arduino/arduino-adv-8/]
* Button module usage  
[https://blog.jmaker.com.tw/arduino-buttons/]
* Buzzer module usage  
[https://shop.mirotek.com.tw/arduino/arduino-start-10/]
* Mbed program example for socket programming  
[https://github.com/ARMmbed/mbed-os-example-sockets]
* Pitches for notes  
[https://gist.github.com/mikeputnam/2820675]
* Game example using Tkinter  
[https://github.com/chaiyenwu/python]
* Tkinter canvas usage  
[https://shengyu7697.github.io/python-tkinter-canvas/?fbclid=IwAR09GuSAVgkDUkuLKacSlHE6nP6Q70kdlGXi_zEaXvQenwKF8RxeAe3DS7E]
