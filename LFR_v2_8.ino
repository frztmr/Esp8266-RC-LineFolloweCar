//******* WiFi Robot Remote Control Mode - MX1508 or L298N ********/
//PART: ESP12 ESP8266, IR OBSTACLE SENSOR, MX1508/L298N, STEP UP MODULE OR 7,4V BATTERY
//THIS MODEL RECOMENDEDLY USING 2 BATERY (7,4V) BUT CAN RUNNING WITH 1 BATTERY (3.7V)
//This program specially created for AZARY CLUB ROBOTNIK DIVISION
//Actually this is second version. the first one only remote control with no line follower features 


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <ArduinoOTA.h>

// connections for drive Motor FR & BR
int enA = D5;
int in1 = D2;
int in2 = D3;
// connections for drive Motor FL & BL
int in3 = D4;
int in4 = D1;
int enB = D6;

//IR Sensor setup:
const int irRightPin = D7;  // signal right IR sensor connected to Arduino's pin D7
const int irLeftPin = D8;   // signal left IR sensor connected to Arduino's pin D8
const int wifiLedPin = D0;  // set digital pin D0 as indication, the LED turn on if NodeMCU connected to WiFi as STA mode

boolean stateRightIR = 0;  // state to store irCenterPin irRightPin
boolean stateLeftIR = 0;   // state to store irCenterPin irLeftPin

String command;         // store command aplication
int SPEED = 1023;       // set 1023 to max speed motor
int speed_Coeff = 3;

ESP8266WebServer server(80);      // Create a webserver object that listens for HTTP request on port 80

unsigned long previousMillis = 0;

const char *ssid = "RN_PBS_01";        //ssid name
const char *password = "r0b0tn1k";     //ssid passoword

//========================================================= MAIN SET UP =====================================================
void setup() {
  Serial.begin(115200);      // set up Serial library at 115200 bps
  Serial.println();
  Serial.println("WiFi Robot Line Follower Mode - L298N 2A");
  Serial.println("---------------------------------------------");

  pinMode(irRightPin, INPUT);    // Sets the irRightPin as an Input
  pinMode(irLeftPin, INPUT);   // Sets the irCenterPin as an Input
  pinMode(stateRightIR, INPUT);   // Sets the stateRightIR as an Input
  pinMode(stateLeftIR, INPUT);   // Sets the stateLeftIR as an Input
  pinMode(wifiLedPin, OUTPUT);  // sets the Wifi LED pin as an Output
  
   // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

    // Set the speed to start, from 0 (off) to 1023 (max speed)
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(wifiLedPin, HIGH);

  // Set up SSID Name and password
    WiFi.mode(WIFI_AP);                          // define Access point mode 
    WiFi.softAP(ssid, password);              //create a SSID
    
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("");
    Serial.println("WiFi-AP-Mode");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    digitalWrite(wifiLedPin, HIGH);   // Wifi LED off when status as AP mode
    delay(3000);
      
  server.on ( "/", HTTP_handleRoot );       // call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound ( HTTP_handleRoot );    // when a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                           // actually start the server
  
  ArduinoOTA.begin();                       // enable to receive update/uploade firmware via Wifi OTA
 }

 
//======================================================== MAIN PROGRAM ======================================================
 void loop() {
    ArduinoOTA.handle();          // listen for update OTA request from clients
    server.handleClient();        // listen for HTTP requests from c`lients
    
      command = server.arg("State");          // check HTPP request, if has arguments "State" then saved the value
      if (command == "F") Forward();          // check string then call a function or set a value
      else if (command == "B") Backward();
      else if (command == "R") TurnRight();
      else if (command == "L") TurnLeft();
      else if (command == "G") ForwardLeft();
      else if (command == "H") BackwardLeft();
      else if (command == "I") ForwardRight();
      else if (command == "J") BackwardRight();
      else if (command == "S") Stop();
//      else if (command == "V") BeepHorn();
      else if (command == "W") TurnLightOn();
      else if (command == "w") TurnLightOff();
      else if (command == "0") SPEED = 330;
      else if (command == "1") SPEED = 400;
      else if (command == "2") SPEED = 470;
      else if (command == "3") SPEED = 540;
      else if (command == "4") SPEED = 610;
      else if (command == "5") SPEED = 680;
      else if (command == "6") SPEED = 750;
      else if (command == "7") SPEED = 820;
      else if (command == "8") SPEED = 890;
      else if (command == "9") SPEED = 960;
      else if (command == "q") SPEED = 1023;
}

// function prototypes for HTTP handlers
void HTTP_handleRoot(void){
  server.send ( 200, "text/html", "" );       // Send HTTP status 200 (Ok) and send some text to the browser/client
  
  if( server.hasArg("State") ){
     Serial.println(server.arg("State"));
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// function to move forward
void Forward(){ 
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
}

// function to move backward
void Backward(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
}

// function to turn right
void TurnRight(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
}

// function to turn left
void TurnLeft(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
}

// function to move forward left
void ForwardLeft(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED/speed_Coeff);
}

// function to move backward left
void BackwardLeft(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED/speed_Coeff);
}

// function to move forward right
void ForwardRight(){
  analogWrite(enA, SPEED/speed_Coeff);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
}

// function to move backward right
void BackwardRight(){ 
  analogWrite(enA, SPEED/speed_Coeff);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
}

// function to stop motors
void Stop(){  
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

// function to beep a buzzer
//void BeepHorn(){
//  digitalWrite(buzPin, HIGH);
//  delay(150);
//  digitalWrite(buzPin, LOW);
//  delay(80);
//}

// function to turn on LED
void TurnLightOn(){
  //print the detected condition by 2 IR sensor
  //Serial.println("IR_L:"+String(stateLeftIR)+"\tIR_R:"+String(stateRightIR)); 

  // FORWARD
    if((digitalRead(irLeftPin)== false) && (digitalRead(irRightPin)==false)){ 
    digitalWrite(in1, HIGH);  //FRONT RIGHT
    digitalWrite(in2, LOW);   //BACK RIGHT
    digitalWrite(in3, HIGH);  //FRONT LEFT
    digitalWrite(in4, LOW);   //BACK LEFT
  }

  // RIGHT TURN
  if((digitalRead(irLeftPin)== false) && (digitalRead(irRightPin)==true)){
    digitalWrite(in1, LOW);   //FRONT RIGHT
    digitalWrite(in2, HIGH);  //BACK RIGHT
    digitalWrite(in3, HIGH);  //FRONT LEFT
    digitalWrite(in4, LOW);   //BACK LEFT
  }

  
//  if((digitalRead(irLeftPin)==true) && (digitalRead(irRightPin)==false)){
//    digitalWrite(in1, LOW);   //FRONT RIGHT
//    digitalWrite(in2, HIGH);  //BACK RIGHT
//    digitalWrite(in3, HIGH);  //FRONT LEFT
//    digitalWrite(in4, LOW);   //BACK LEFT
//  }

  //LEFT TURN
  if((digitalRead(irLeftPin)==true) && (digitalRead(irRightPin)==false)){
    digitalWrite(in1, HIGH);  //FRONT RIGHT
    digitalWrite(in2, LOW);   //BACK RIGHT
    digitalWrite(in3, LOW);   //FRONT LEFT
    digitalWrite(in4, HIGH);  //BACK LEFT
  }
  
// if((digitalRead(irLeftPin)==true) && (digitalRead(irRightPin)==false)){
//   digitalWrite(in1, HIGH);  //FRONT RIGHT
//   digitalWrite(in2, LOW);   //BACK RIGHT
//   digitalWrite(in3, LOW);   //FRONT LEFT
//   digitalWrite(in4, HIGH);  //BACK LEFT
//  }

  // STOP
  if((digitalRead(irLeftPin)==true) && (digitalRead(irRightPin)==true)){
    digitalWrite(in1, LOW);  //FRONT RIGHT
    digitalWrite(in2, LOW);   //BACK RIGHT
    digitalWrite(in3, LOW);  //FRONT LEFT
    digitalWrite(in4, LOW);   //BACK LEFT
  }  
}

// function to turn off LED
void TurnLightOff() {
    digitalWrite(in1, LOW);  //FRONT RIGHT
    digitalWrite(in2, LOW);   //BACK RIGHT
    digitalWrite(in3, LOW);  //FRONT LEFT
    digitalWrite(in4, LOW);   //BACK LEFT
   }

 

  
