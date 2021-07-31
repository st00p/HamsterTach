# HamsterTach
Code for an esp32/esp8266 based tachometer for a hamster wheel using a hall-effect sensor and uploading data to a Googlesheet via IFTTT.
Made 2021 by Stu Patience, www.driverlesscrocodile.com.

Please use and remix this code in anyway that helps your project.

## Hardware:
- esp32 or esp8266 (NodeMCU) microcontroller with wifi
- Hall Effect Sensor A3144
- One LED
- 10k resistor ?
- 100k resistor
- Mini breadboard
- Connecting wires
- Small neodium magnet

## Software
- Arduino IDE

## Concept
The Hall effect sensor acts as a button, switching ON when the magnet is close to the sensor (i.e. each revolution).
The esp32 is programmed to count each time the switch turns on.
Every minute, the data is uploaded to a googlesheet using an IFTTT webhook.

## Credits

Made using code for button debounce created 21 Nov 2006  by David A. Mellis / modified 30 Aug 2011 by Limor Fried / modified 28 Dec 2012 by Mike Walters / modified 30 Aug 2016 by Arturo Guadalupiat at https://learn.adafruit.com/make-it-switch/debouncing 

Instructions for IFTTT webhook to upload data to a googlesheet from: https://microcontrollerslab.com/esp32-esp8266-publish-sensor-readings-google-sheets-via-ifttt/
