# SmartThermo

Author: Luca Plian  
Platform: Arduino UNO compatible  
Group: 332CA  

## Overview

SmartThermo is a smart temperature monitoring system designed to visualize and respond to room temperature changes. It includes real-time temperature sensing, a dynamic visual display, automatic cooling, and audible alerts. 


## Features

- Real-time temperature measurement using DS18B20
- RGB LED indicates temperature levels using color
- Servo motor simulates a traditional thermometer
- Automatic fan activation for cooling
- Buzzer alarm for high temperature threshold
- LCD 1602 with I²C interface displays temperature and messages
- RTC module (DS3231) resets max and min temperature values each new day
- Button 1: Turns off the buzzer
- Button 2: Cycles through maximum temperature, minimum temperature, and current date on the LCD
