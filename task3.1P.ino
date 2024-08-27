#include <WiFiNINA.h>
#define LIGHT_SENSOR 4
char ssid[] = "Palak";
char pass[] = "12345678";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";

String light_state = "Off";
int light_value = 0;
String last_light = "Off";

bool connectToWiFi(char ssid[], char pass[]) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
      if (i > 6) {
        Serial.print("Wifi Connection Failed!");
        return false;
        break;
      }
      i++;
    }
    Serial.println("\nConnected.");
    return true;
  }
  return true; 
}
int readLightSensor() {
  light_value = digitalRead(LIGHT_SENSOR);
  return light_value;
}


void sendWebhook(String state) {
  if (!client.connect(HOST_NAME, 80)) {
    Serial.println("Connection failed!");
    return;
  }

    Serial.println("Connected to server");
    client.println("GET trigger/house/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ HTTP/1.1");
    client.println("Host: maker.ifttt.com"  );
    client.println("Connection: close");
    client.println(); 


  client.stop();
  Serial.println("Sent notification to IFTTT");
}

void setup() {

  Serial.begin(115200);
  connectToWiFi(ssid,pass);
  while (!Serial);


  pinMode(LIGHT_SENSOR,INPUT);
}



void loop() {

  int current_light = readLightSensor();

  if (current_light > 0) {
    light_state = "On";
  } else {
    light_state = "Off";
  }

  // Send notification only if state changes or 'c' is pressed
  if (light_state != last_light || Serial.read() == 'c') {
    sendWebhook(light_state);
    last_light = light_state;
  }
  delay(1000); // Adjust delay based on desired update frequency

}
