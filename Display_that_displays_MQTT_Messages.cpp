#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);




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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);
}


void verifyFingerprint() {
  if(client.connected() || espClient.connected()) return;
  
  Serial.print("Checking TLS @ ");
  Serial.print(mqtt_server);
  Serial.print("...");
  
  if (!espClient.connect(mqtt_server, mqtt_port)) {
    Serial.println("Connection failed. Rebooting.");
    Serial.flush();
    ESP.restart();
  }
  if (espClient.connect(mqtt_server, mqtt_port)) {
    Serial.print("Connection secure -> .");
  } else {
    Serial.println("Connection insecure! Rebooting.");
    Serial.flush();
    ESP.restart();
  }

  espClient.stop();

  delay(100);
}

unsigned int counter = 0;



void setup() {
  Serial.begin(115200);
  Serial.println("TestMQTT");

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
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect(WiFi.macAddress().c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe("/display");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  u8g2.begin();
}





void loop() {
  client.loop();
  u8g2.setFont(u8g2_font_courB10_tf);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 20);
    u8g2.print(F("arduino-"));
    u8g2.setCursor(0, 40);
    u8g2.print(F("projekte"));
    u8g2.setCursor(0, 60);
    u8g2.print(F(".info"));
  } while ( u8g2.nextPage() );
  delay(1000);

}
