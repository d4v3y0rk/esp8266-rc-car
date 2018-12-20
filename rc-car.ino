#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "XXXX";
const char* ssid = "XXXX";
const char* password = "XXXX";

/* define L293D motor control pins */
int leftMotorForward = D7;     /*  IN3   */
int rearMotorForward = D1;   /* IN1  */
int leftMotorBackward = D8;    /* IN4   */
int rearMotorBackward = D3;  /* -> IN2  */


/* define L298N or L293D enable pins */
int rearMotorENB = D6; /* Motor-A Enable - EN1 */
int leftMotorENB = D5;  /* Motor-B Enable - EN2*/

void setup()
{
  /* initialize motor control pins as output */
  pinMode(leftMotorForward, OUTPUT);
  pinMode(rearMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rearMotorBackward, OUTPUT);

  /* initialize motor enable pins as output */
  pinMode(leftMotorENB, OUTPUT); 
  pinMode(rearMotorENB, OUTPUT);

  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);

  Serial.begin(115200);
  delay(10);
  /* this is not needed normally but I am connecting this RC car to my iPhone hotspot to drive it anywhere */
  WiFi.enableSTA(true);
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  
  
  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk
  while (Blynk.connect() == false) {
    // Wait until connected
  }
  Serial.println("Connected to Blynk server");
}

/* Main Loop */
void loop() {
  Blynk.run();
} 

/* forward */
void MotorForward(void)   
{
  digitalWrite(rearMotorENB,HIGH);
  digitalWrite(rearMotorForward,HIGH);
  digitalWrite(rearMotorBackward,LOW);
}

/* reverse */
void MotorBackward(void) {
  digitalWrite(rearMotorENB,HIGH);
  digitalWrite(rearMotorBackward,HIGH);
  digitalWrite(rearMotorForward,LOW);
}
/* steering */
void Left(void) {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
}

void Right(void) {
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
}

/* stop rear motor and center the front wheels */
void MotorStop(void) {
  digitalWrite(rearMotorENB,LOW);
  digitalWrite(rearMotorForward,LOW);
  digitalWrite(rearMotorBackward,LOW);
}

void Center(void) {
  digitalWrite(leftMotorENB,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
}

/* Functions cooresponding to the virtual pins in the Blynk phone app */

BLYNK_WRITE(V0) {
  int speedfb = param.asInt(); // read button
  if ( speedfb > 250 ) {
      Serial.println("Moving forward");
      MotorForward();
    }
    else if ( speedfb <= 250 && speedfb >= -250 ) {
      MotorStop();
    }
    else if ( speedfb < -250 ) {
      MotorBackward();
    }
}

BLYNK_WRITE(V1) {
  int speedlr = param.asInt(); // read button
  if ( speedlr > 250 ) {
      Left();
    }
    else if ( speedlr <= 250 && speedlr >= -250 ) {
      Center();
    }
    else if ( speedlr < -250 ) {
      Right();
    }
}
