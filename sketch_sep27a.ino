#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <PubSubClient.h>

const char *ssid =  "YYYYYYY";        //Your Access Point or Personal Hotspot, cannot be longer than 32 characters!
const char *pass =  "ZZZZZZ5";    //Your Access Point or Personal Hotspot password
const char* serverTS = "api.thingspeak.com";
String apiKey = "KEY";          //Insert your Channel API Key here
//const int pingPin = 2;          //Ultrasonic connected to GPIO0
int TRIGGER = 5; //Pin D1 = TRIGGER
int ECHO = 4; //Pin D2 = ECHO
void setup() 
{
  pinMode(0,OUTPUT);            //LED connected to GPIO2
  Serial.begin(115200);         //Recommended speed is 115200
  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);
  connectWifi();
  }

void loop() 
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  

  duration = pulseIn(ECHO, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(100);

  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second

  sendHeight(cm);
}
void connectWifi()
{ 
  Serial.print("Connecting to "+*ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect
long microsecondsToInches(long microseconds) 
{ 
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds) {
  
  return microseconds / 29 / 2;
}

 void sendHeight(float cm)
{  
  WiFiClient tclient;//not to be confused with "client" in PubSub{}, and wclient for mqtt
   if (tclient.connect(serverTS, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   //Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field1=";
   postStr += String(cm);
   postStr += "\r\n\r\n";
   
   tclient.print("POST /update HTTP/1.1\n");
   tclient.print("Host: api.thingspeak.com\n");
   tclient.print("Connection: close\n");
   tclient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   tclient.print("Content-Type: application/x-www-form-urlencoded\n");
   tclient.print("Content-Length: ");
   tclient.print(postStr.length());
   tclient.print("\n\n");
   tclient.print(postStr);
   delay(1000);
   
   }//end if

 tclient.stop();
}
