// Chuck Faranda chuck@ccdastro.net gleaned from numerous sources in the public domain - use at your own risk :-)

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <math.h>

// include LCD library code
#include <LiquidCrystal.h>
// include DHT library code
#include <DHT.h>
#include <DHT_U.h>

String serialin;
String str;
String str2;
String str3;
String str4;

//store relay states
int relay1state;
int relay2state;
int relay3state;
int relay4state;

int index = 0;                  // the index of the current reading
double total = 0;                  // the running total
double average = 0;                // the average
//int average2 = 0;             // the average
float Temp1 = 00.0; 
int LDR_Pin = A1; //analog pin 1 for photoresistor
#define DHTPIN 2            // DHT22 data pin is connected to Arduino pin 8
 
// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(3, 14, 15, 16, 17, 18); // 8,9,10,11,12 for uno
 
#define DHTTYPE DHT22      // DHT22 sensor is used
DHT dht(DHTPIN, DHTTYPE);   // Initialize DHT library

#define relay1 4
#define relay2 5
#define relay3 6
#define relay4 7
#define opened 2
#define closed 1

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// the router's gateway address:
byte gateway[] = { 192, 168, 1, 1 };// insert your gateway
// the subnet:
byte subnet[] = { 255, 255, 255, 0 };
IPAddress ip(192,168,1,88); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80

void setup() {
    Ethernet.begin(mac, ip, gateway, subnet);  // initialize Ethernet device
    server.begin();           // start to listen for clients

  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);
  dht.begin();

  //Begin Serial Comunication(configured for 9600baud)
  Serial.begin(9600);
  //Serial.setTimeout(5000); 
  //pin relay as OUTPUT
  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  pinMode(DHTPIN, INPUT);
 
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,LOW);
  
  Serial.write("Viv#\n");
  
}
boolean isValidNumber(String str)
{
   boolean isNum=false;
   if(!(str.charAt(0) == '+' || str.charAt(0) == '-' || isDigit(str.charAt(0)))) return false;

   for(byte i=1;i<str.length();i++)
   {
       if(!(isDigit(str.charAt(i)) || str.charAt(i) == '.')) return false;
   }
   return true;
}
void loop() {

EthernetClient client = server.available();  // try to get client

relay1state = digitalRead (relay1);
relay2state = digitalRead (relay2);
relay3state = digitalRead (relay3);
relay4state = digitalRead (relay4);

//get temp & hum from DTH22
str = round(dht.readHumidity());
Temp1 = (dht.readTemperature ()* 1.8 +32);
str2 = Temp1;
str3 = analogRead(LDR_Pin);

if ((str.length() == 0) || (str2.length() == 0)){
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    return;
  }
if (isValidNumber(str) == "false") {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    return;
  } 
if (isValidNumber(str2) == "false") {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    return;
  } 
if ((str.length()>= 8) || (str2.length()>= 8)){
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Error");
    return;
  } 

  // write data to LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp = ");
  lcd.setCursor(7, 0);
  lcd.print(Temp1,1);
  lcd.print((char)223);         // degree symbol
  lcd.print(" F");
  lcd.setCursor(0, 1);
  lcd.print("Hum  = ");
  lcd.setCursor(7, 1);
  lcd.print(str);
  lcd.print(" %");
  
  //Verify connection by serial
  if (Serial.available()>0) {
    //Read Serial data and alocate on serialin

    serialin = Serial.readStringUntil('#');
        
     if (serialin == "stop1"){
        //digitalWrite(dewPin,LOW);
        digitalWrite(relay1,LOW);
     }
     if (serialin == "stop2"){
        //digitalWrite(dewPin,LOW);
        digitalWrite(relay2,LOW);
     }
     if (serialin == "stop3"){
        //digitalWrite(dewPin,LOW);
        digitalWrite(relay3,LOW);
     }
     if (serialin == "stop4"){
        //digitalWrite(dewPin,LOW);
        digitalWrite(relay4,LOW);
     }
       
      if (serialin == "relay1"){ // Two Pipeines(||) to make a boolean OR Comparission
         digitalWrite(relay1,HIGH);
        //val = digitalRead(open);
      }
      if (serialin == "relay2"){ // Two Pipeines(||) to make a boolean OR Comparission
         digitalWrite(relay2,HIGH);
      //val = digitalRead(close);
      }
       if (serialin == "relay3"){ // Two Pipeines(||) to make a boolean OR Comparission
       digitalWrite(relay3,HIGH);
       //val = digitalRead(open);
      }
      if (serialin == "relay4"){ // Two Pipeines(||) to make a boolean OR Comparission
      digitalWrite(relay4,HIGH);
      //val = digitalRead(close);
      }
      if (serialin == "get"){
       Serial.println (str +"," + str2 +"," + str3 +"," + relay1state +"," + relay2state +"," + relay3state +"," + relay4state + "#");
       //Serial.println(digitalRead (relay1));
       // delay(500);
     }
  }  
   if (client) {  // got web client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {

                    // send a standard http response header
client.println("HTTP/1.1 200 OK"); 
client.println("Content-Type: text/html"); 
client.println("Connection: close"); // connection closed completion of response 
client.println("Refresh: 10"); // refresh the page automatically every 5 sec 
client.println(); 
client.println("<!DOCTYPE HTML>"); 
client.println("<html><body>"); 
client.print("<h1>Vivarium Controller</h1>");
client.print("<span style=\"font-size: 26px\";><br>  Temperature is "); 
client.print(str2); 
client.println(" \xB0 F<br>"); 
client.print("<br>  Humidity is "); 
client.print(str); 
client.println(" %<br>"); 
client.println("</body></html>"); 
                 
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)
}
