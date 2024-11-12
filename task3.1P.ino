#include <WiFiNINA.h>  // Include the WiFiNINA library to enable WiFi functionality

#define LIGHT_SENSOR 4  // Define the pin connected to the light sensor

// WiFi credentials
char ssid[] = "Palak"; 
char pass[] = "12345678";  

WiFiClient client;  

// IFTTT webhook details
char   HOST_NAME[] = "maker.ifttt.com"; 

String light_state = "Off";  
int light_value = 0;  
String last_light = "Off";  


// Function to connect to WiFi
bool connectToWiFi(char ssid[], char pass[]) {
  // Check if WiFi is already connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    int i = 0;
    
    // Try to connect to WiFi, with a maximum of 7 attempts (35 seconds)
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  
      Serial.print(".");  
      delay(5000);  
      
      if (i > 6) {  
        Serial.print("Wifi Connection Failed!");
        return false;
      }
      i++;
    }
    
    // Once connected, print a success message
    Serial.println("\nConnected.");
    return true;  
  }
  return true;  
}


// Function to read the light sensor value
int readLightSensor() {
  light_value = digitalRead(LIGHT_SENSOR);  
  return light_value;  
}


// Function to send a notification to IFTTT via a webhook
void sendWebhook(String state) {
  // Connect to the IFTTT webhook server (port 80 for HTTP)
  if (!client.connect(HOST_NAME, 80)) {
    Serial.println("Connection failed!");  // If connection fails, print an error message
    return;  // Exit the function
  }

  // If connected to the server, print success message
  Serial.println("Connected to server");

  // Send HTTP GET request to trigger the IFTTT webhook
  client.println("GET trigger/house/with/key/dCzgH4VqCd8yZ_jYfZseMYt86OHL13Bdzn5tjtvUlSZ HTTP/1.1");
  client.println("Host: maker.ifttt.com");  
  client.println("Connection: close"); 
  client.println();  // Send a blank line to end the headers

  // Stop the client connection after sending the request
  client.stop();
  
  // Print a confirmation message
  Serial.println("Sent notification to IFTTT");
}


// Setup function to initialize the system
void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  connectToWiFi(ssid, pass);  // Call the WiFi connection function
  while (!Serial);  // Wait for the serial monitor to be ready

  pinMode(LIGHT_SENSOR, INPUT);  // Set the light sensor pin as an input
}


// Main loop function
void loop() {
  int current_light = readLightSensor();  // Read the current light sensor value

  // Determine if the light is on or off based on the sensor reading
  if (current_light > 0) {
    light_state = "On";  
  } else {
    light_state = "Off"; 
  }

  // Only send the notification if the light state has changed or if the user presses the 'c' key in the Serial Monitor
  if (light_state != last_light || Serial.read() == 'c') {
    sendWebhook(light_state);  // Send a notification to IFTTT
    last_light = light_state;  // Update the last recorded light state
  }

  // Wait 1 second before checking the light sensor again
  delay(1000);
}
