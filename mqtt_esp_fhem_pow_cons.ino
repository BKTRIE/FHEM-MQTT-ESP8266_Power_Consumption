/*
  Basic ESP8266 MQTT example etwas angedeutscht
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define PIN1     12   // Pin schaltet LED für Zähler 1 ein
#define PIN2     14   // Pin schaltet LED für Zähler 2 ein


const char* ssid              = "ssid";
const char* password          = "passwd";
const char* clientName        = "StromMess";
const char* mqtt_server       = "192.168.x.y";
const char* start             = "fhem/anmeldung/";
const char* targetSetStrom1   = "fhem/keller/strom/zaehler1";
const char* targetSetStrom2   = "fhem/keller/strom/zaehler2";
const char* anmeldeKanal      = "fhem/keller/debug/set";
const char* targetSetDebug    = "fhem/keller/debug/";
long        verzoegern        = 0;
int schwellWert1              = 850;
int schwellWert2              = 950;
int timer                     = 30;

boolean debug, trigger1, trigger2, zeahler1 = false;

IPAddress ip(192, 168, x, z); //statische IP
IPAddress gateway(192, 168, x, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiClient ESPClient;
PubSubClient client(ESPClient);

void setup() {
  WiFi.mode(WIFI_STA);
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  Serial.begin(115200);
  WiFiLogin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void WiFiLogin() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, using IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* kanal, byte* nachrichtInBytes, unsigned int length) {
  String nachricht = "";
  for (int i = 0; i < length; i++) {
    nachricht += (char)nachrichtInBytes[i];
  }
  Serial.println(kanal);
  Serial.println(nachricht);
  String  stringKanal         = kanal;
  String  stringAnmeldeKanal1 = anmeldeKanal;
  if (stringKanal == stringAnmeldeKanal1) {
    debug = (nachricht == "on");
    if (debug)
      timer = 500;
    else
      timer = 30;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Warten auf Anmeldung...");
    if (client.connect(clientName)) {
      Serial.println("angemeldet");
      client.publish(start, clientName);
      client.subscribe(anmeldeKanal);
    } else {
      Serial.print("Fehler, rc=");
      Serial.print(client.state());
      Serial.println(" erneut versuchen in 5 Sekunden");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long jetzt = millis();
  if (jetzt - verzoegern > timer) {
    if (zeahler1) {
      digitalWrite(PIN1 , HIGH);
      digitalWrite(PIN2 , LOW);
    }
    else {
      digitalWrite(PIN1 , LOW);
      digitalWrite(PIN2 , HIGH);
    }
    delay(1);
    int wert = analogRead(A0);
    digitalWrite(PIN1 , LOW);
    digitalWrite(PIN2 , LOW);
    char sendInt[4];
    String(wert).toCharArray(sendInt, 5);
    if (debug)
      client.publish(targetSetDebug, sendInt);
    if (zeahler1) {
      if (wert < schwellWert1 && !trigger1) {
        trigger1 = true;
        client.publish(targetSetStrom1, "on");
      }
      if (wert > schwellWert2 && trigger1) {
        trigger1 = false;
        client.publish(targetSetStrom1, "off");
      }
    }
    else {
      if (wert < schwellWert1 && !trigger2) {
        trigger2 = true;
        client.publish(targetSetStrom2, "on");
      }
      if (wert > schwellWert2 && trigger2) {
        trigger2 = false;
        client.publish(targetSetStrom2, "off");
      }
    }
    zeahler1 = !zeahler1;
    verzoegern = jetzt;
  }
}
