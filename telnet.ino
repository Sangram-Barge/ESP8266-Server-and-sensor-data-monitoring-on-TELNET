#include<ESP8266WiFi.h>
#include<LiquidCrystal.h>

const int port = 8888; // using 8888 as telnet port.
const int ledPin = D7; // declare a led pin.
const int contrast = D0; // VEE pin of the display.
const int rs = D1; // register select pin of displa.
const int en = D2; // enable pin of display.
const int d4 = D3; // pin d4 of the lcd.
const int d5 = D4; // pin d5 of the lcd.
const int d6 = D5; // pin d6 of the lcd.
const int d7 = D6; // pin d7 of the lcd.
const int tempSensor = A0; // sensor pin.

float temp; // variable to store temperature.
float mV; // variable to store analog voltage.
float adcVal; // variable to store adc reading.

const char ssid[] = "sangram"; // wifi name.
const char pass[] = "sangram12345"; // wifi password.

WiFiServer server(port); // intialise server object at virtual port.
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // initialise lcd object.

void setup(){
  Serial.begin(115200); // intialise serial communication.
  lcd.begin(16,2); // begin the lcd with 16 characters and 2 rows.
  pinMode(ledPin,OUTPUT); 
  pinMode(tempSensor, INPUT);
  digitalWrite(ledPin,LOW);
  analogWrite(contrast, 255); // set the contrast of the lcd.

  clearScreen();                     
  lcd.setCursor(0, 0);              
  delay(5);                         
  lcd.print("Mini Project    ");     
  lcd.setCursor(0, 1);              
  delay(5);                         
  lcd.print("Data monitoring");     
  delay(2000);                      
  
  WiFi.begin(ssid,pass); // begin wifi connection.
  clearScreen();
  while(WiFi.status() != WL_CONNECTED){ // wait while wifi is connected.
    lcd.print(" .");
    delay(500);
  }
  clearScreen();
  lcd.println("Conected        ");
  delay(2000);
  clearScreen();
  lcd.print("use telnet port: "); // notify that wifi is connected.
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP()); // print local ip of the Server.

  server.begin(); // intialise server.
}

void loop(){
  WiFiClient client = server.available(); // initialise client object with status of the server.

  while(client){
    if(client.connected()){               
      Serial.println("client connected"); // if telnet terminal is accessed, notify the user that client is connected.
    }                                                   

    while(client.connected()){    // while client is connected stay in the loop.
      if(client.available()>0){   // if data on telnet port is available,
        int com = client.read();  // read the data.

        if(com == 116){  // command t ('116' refers to letter 't' on keyboard)
          adcVal = analogRead(tempSensor);
          temp = adcVal*3.222656/10;
          client.println("");
          client.print("temperature is: ");
          client.print(temp);
          client.println("C");
          client.println("");
          client.print("enter command<< ");
        }

        if(com == 108){  // command l ('108' refers to letter 'l' on keyboard)
          digitalWrite(ledPin,!digitalRead(ledPin));
          client.println("");
          client.println("lights changed");
          client.println("");
          client.print("enter command<< ");
        }

        if(com == 104){ // command h ('104' refers to letter 'h' on keyboard)
          client.println(""); 
          client.println("Use command 'l' to turn light ON/OFF");
          client.println("Use command 't' to get the temperature value");
          client.println("");
          client.print("enter command<< ");
        }
      }
    }
    client.stop(); // if client is disconnected, stop the transfer.
    Serial.println("disconnected"); // notify the user that client.
  }
}

void clearScreen(){   // lcd clear sequence.
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
}