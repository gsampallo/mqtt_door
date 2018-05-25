#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define pinSensor1 0
#define pinSensor2 2

const char* ssid = "wifi";
const char* password = "clave";
const char* mqtt_server = "server";
const char* keyDevice = "SENSORPUERTA";

String linkNotificacion = "webservice";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

volatile int state1 = false;
volatile int flag1 = false;
const char* door_state1 = "closed1";

unsigned long previousMillis1 = 0; 
const long interval = 1000;

void changeDoorStatus1() {

    unsigned long currentMillis1 = millis();
 
    if(currentMillis1 - previousMillis1 >= interval) {
        previousMillis1 = currentMillis1;   
      
        state1 = !state1;

        if(digitalRead(pinSensor1) == LOW) {
          door_state1 = "opened1";
        } else {
          door_state1 = "closed1";
        }
        
        flag1 = true;
    }
    
}

volatile int state2 = false;
volatile int flag2 = false;
const char* door_state2 = "closed1";

unsigned long previousMillis2 = 0; 


void changeDoorStatus2() {

    unsigned long currentMillis2 = millis();
 
    if(currentMillis2 - previousMillis2 >= interval) {
        previousMillis2 = currentMillis2;   
      
        state2 = !state2;

        if(digitalRead(pinSensor2) == LOW) {
          door_state2 = "opened2";
        } else {
          door_state2 = "closed2";
        }
        
        flag2 = true;
    }
    
}


void setup() {
  pinMode(pinSensor2,OUTPUT);
  pinMode(pinSensor1,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pinSensor1), changeDoorStatus1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinSensor2), changeDoorStatus2, CHANGE);
  
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 

}

void setup_wifi() {

  delay(10);
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

}

void reconnect() {

  while (!client.connected()) {

    if (client.connect(keyDevice,"usuario","clave")) {
      Serial.println("connected");
      client.subscribe("ALARMA");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
  if(flag1){
    client.publish("SENSORPUERTA",door_state1 );
    flag1 = false;
  }

  if(flag2){
    client.publish("SENSORPUERTA",door_state2);  
    flag2 = false;
  }  

}
