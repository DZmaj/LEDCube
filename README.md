# LEDCube
Various routines for a 3x3x3 matrix of PWM LEDs powered by a ATMega 2560.

• An Arduino powered 3 x 3 x 3 LED matrix cube capable of individually turning on any combination of the 27 LEDs with the use of multiplexing.
• All the LEDs were PWM LEDs, meaning their brightness can be controlled from 0-100%.

• The microchip used, an ATMEGA 2560, does not have enough pins to control all 27 LEDs individually. To remedy this, multiplexing and 
extremely quick LED blinks are used to give the illusion ofindependent control of every LED.

• Designed and wrote code for 7 different ”light shows” which demonstrate this independent control, including turning on all LEDs one at a time and creating a ”Knight Rider scanner” effect with the help of PWM.
