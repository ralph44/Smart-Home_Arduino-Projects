#include <Arduino.h>

#include <Ticker.h>
Ticker ticker;

#define NB_TRYWIFI    10




#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

const char* cfg_wifi_ssid = "";
const char* cfg_wifi_pwd = "";

const char* mqtt_server = "";
const unsigned int mqtt_port = ;
const char* mqtt_user =   "";
const char* mqtt_pass =   "";


WiFiClient espClient;
PubSubClient client(espClient);

void tick()
{
  //toggle state
  int state = digitalRead(LED_BUILTIN);  // get the current state of GPIO1 pin
  digitalWrite(LED_BUILTIN, !state);     // set pin to the opposite state
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("TestMQTT");

  
  // Fait clignoter la LED intégré durant la connexion au réseau WiFi - Blink Bleu Led during WiFi connexion
  pinMode(LED_BUILTIN, OUTPUT);
  ticker.attach(0.5, tick);

  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg_wifi_ssid, cfg_wifi_pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    
    if (client.connect(WiFi.macAddress().c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe("/display/#");
      client.publish("DeepSleepDemo/PIR", "1");
      delay(3000);
      client.publish("DeepSleepDemo/PIR", "0");
      ticker.detach();
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("I go to sleep now");
      delay(3000);
      ESP.deepSleep(0);
      Serial.print("I go to sleep now");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop() {


}
