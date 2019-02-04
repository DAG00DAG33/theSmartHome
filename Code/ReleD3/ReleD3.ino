#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Define NodeMCU D3 pin  connect to LED
#define LUZ_1 0
#define LUZ_2 2
#define BUTTON_1 3
#define BUTTON_2 1

int luz1;
int luz2;
int button1=0;
int button2=0;

// Update these with values suitable for your network.
const char* ssid = "MIWIFI_2G_mSdJ";
const char* password = "9csKN6Px";
const char* mqtt_server = "m21.cloudmqtt.com";
//const char* mqtt_server = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(100);
  // We start by connecting to a WiFi network
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }
  randomSeed(micros());
  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length)
{
  //Serial.print("Command from MQTT broker is : [");
  //Serial.print(topic);
  if (strcmp(topic, "luz_1") == 0) {
    int p = (char)payload[0] - '0';
    //Serial.print(p);
    if (p == 0){
      digitalWrite(LUZ_1, LOW);
      luz1=0;
    }else if (p == 1){
      digitalWrite(LUZ_1, HIGH);
      luz1=1;
    }
    //Serial.println();
  }else if (strcmp(topic, "luz_2") == 0) {
    int p = (char)payload[0] - '0';
    //Serial.print(p);
    if (p == 0){
      digitalWrite(LUZ_2, LOW);
      luz2=0;
    }else if (p == 1){
      digitalWrite(LUZ_2, HIGH);
      luz2=1;
    }
    //Serial.println();
  }

} //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to ---------------------------------------------------
    //PONER DATOS
    if (client.connect("DAG00DAG33", "gmctxgsj", "xqvrR_PofPOG"))
      //if (client.connect(clientId.c_str()))-----------------------------------------------
    {
      //Serial.println("connected");
      //once connected to MQTT broker, subscribe command if any
      //SUBSCRIBIRSE TOPICS
      client.subscribe("luz_1");
      client.subscribe("luz_2");
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    } 
  }
} //end reconnect()

void setup() {
  //Serial.begin(115200);
  setup_wifi();
  //PONER PUERTO
  client.setServer(mqtt_server, 12101);
  client.setCallback(callback);
  pinMode(LUZ_1, OUTPUT);
  pinMode(LUZ_2, OUTPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_2, INPUT);
  digitalWrite(LUZ_1, HIGH);
  digitalWrite(LUZ_2, HIGH);
}

void loop() {

  
  if (!client.connected()) {
    reconnect();
  }
  if ((digitalRead(BUTTON_1)==0)&&(button1==1)){
    if(luz1==1){
      client.publish("luz_1", "0");
    }else if(luz1==0){
      client.publish("luz_1", "1");
    }
    button1=0;
  }
  if ((digitalRead(BUTTON_2)==0)&&(button2==1)){
    if(luz2==1){
      client.publish("luz_2", "0");
    }else if(luz2==0){
      client.publish("luz_2", "1");
    }
    button2=0;
  }
  if(digitalRead(BUTTON_1)==1){
    button1=1;
  }
  if(digitalRead(BUTTON_2)==1){
    button2=1;
  }
  
  client.loop();

}
