// Code for HamsterWheel Tachometer using esp32/ep8266, Hall Effect sensor and uploading data to Googlesheet using IFTTT webhook.

// Made by Stu Patience (www.driverlesscrocodile.com) using code for button debounce created 21 Nov 2006  by David A. Mellis / modified 30 Aug 2011 by Limor Fried / modified 28 Dec 2012 by Mike Walters / modified 30 Aug 2016 by Arturo Guadalupiat
// at https://learn.adafruit.com/make-it-switch/debouncing 

// and instructions for IFTTT webhook to upload data to a googlesheet from: https://microcontrollerslab.com/esp32-esp8266-publish-sensor-readings-google-sheets-via-ifttt/

// Feel free to use, adapt and share this code however is most helpful to your project.


//Notes from IFTTT (copied and pasted for reference):
//To trigger an Event
//Make a POST or GET web request to:
//https://maker.ifttt.com/trigger/Hamster_Tach/with/key/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//With an optional JSON body of:
//{ "value1" : "", "value2" : "", "value3" : "" }
//The data is completely optional, and you can also pass value1, value2, and value3 as query parameters or form variables. This content will be passed on to the action in your Applet.
//You can also try it with curl from a command line.
//curl -X POST https://maker.ifttt.com/trigger/Hamster_Tach/with/key/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

// Code body:

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <Wire.h>

const char* ssid     = "XXXXXXXXX"; // Write your SSID
const char* password = "XXXXXXXXX";  //Write your Password

const char* IFTTT_URL = "/trigger/Hamster_Tach/with/key/XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";  // MAKE SURE YOU UPDATE THIS WITH THE CORRECT KEY
const char* server = "maker.ifttt.com";

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;   // GPIO 5 > NODEMCU Pin D1 (the for the pushbutton input pin)

// const int ledPin = 4;      // GPIO 4 > NODEMCU Pin D2 (for the LED output pin)

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin
int tachcount = 0;   // sets the variable for the tachometer count
int reportfreq = 60000;  // sets the frequency for reporting count, in milliseconds

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 1;    // the debounce time in milliseconds; increase if the output flickers DECREASED TO 5 for hall effect switch
unsigned long timer = 0;             // initialise timer variable

void setup() {

  // initialise buttonPin (for the hall effect sensor)
  
  pinMode(buttonPin, INPUT);

//this was an optional extra output to see if the code was working properly
// pinMode(ledPin, OUTPUT); 
// set initial LED state
//  digitalWrite(ledPin, ledState);

  // intialise Serial output
  Serial.begin(9600);
  delay(5000);
  Serial.println("Intialising Serial Output... \nWelcome to HAMSTER_TACH 2021 - \"See how they run..\" ");
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = !digitalRead(buttonPin);
//  Serial.println(reading);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED and count a new rotation if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        tachcount += 1;
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
  
  // trigger the serial report
  
  if (millis() - timer > reportfreq) {
    String string1 = "Tachcount: ";
    
    Serial.println(string1 + tachcount);// print to serial - later this will be uploaded and report
    timer = millis(); // set the timer to the current time
    initWifi();
    makeIFTTTRequest();
    tachcount = 0;
  }
  }
  
void initWifi() {
  Serial.print("Connecting... "); 
  WiFi.begin(ssid, password);  

  int timeout = 10 * 4; // 10 seconds
  while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if(WiFi.status() != WL_CONNECTED) {
     Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: "); 
  Serial.print(millis());
  Serial.print(", IP address: "); 
  Serial.println(WiFi.localIP());
}

void makeIFTTTRequest() {
  Serial.print("Connecting to "); 
  Serial.print(server);
  
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(IFTTT_URL);

  String jsonObject = String("{\"value1\" : \"") + tachcount + "\"}";
                     
  client.println(String("POST ") + IFTTT_URL + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);
  Serial.println(jsonObject);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nClosing Connection");
  client.stop(); 
}
