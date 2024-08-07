/*

## Custom License Agreement

This project includes software, hardware, and system integration and is licensed under the following terms:

### License Overview

- **Non-Commercial Use**: You are free to use, modify, and distribute the software, hardware designs, and integration components for non-commercial purposes, provided that you comply with the conditions outlined below.
- **Commercial Use**: If you wish to use, modify, or distribute the software, hardware designs, or integration components for commercial purposes, including but not limited to incorporating them into products or services for sale, you must obtain explicit permission from the author.

### Terms for Non-Commercial Use

1. **Attribution**: You must provide appropriate credit, including a link to the original repository and a notice indicating any changes made.
2. **Non-Commercial Use**: The use, modification, and distribution of this project are permitted only for non-commercial purposes. Commercial use requires obtaining a separate license.

### Terms for Commercial Use

1. **Contact Requirement**: To use the software, hardware designs, or integration components for commercial purposes, you must contact the author at [arshiakeshvariasl@gmail.com] to obtain permission and discuss licensing terms.
2. **Additional Licensing**: Commercial use will be subject to additional terms agreed upon between you and the author. This may include financial arrangements or other conditions.

### Integration with Other Projects

1. **Commercial Integration**: If you wish to integrate the software, hardware designs, or other components with other commercial projects or products, you must contact the author for permission and to discuss any additional licensing terms.
2. **Non-Commercial Integration**: Integration with non-commercial projects is permitted under the same terms as outlined for non-commercial use.

### Disclaimer

This project is provided "as-is" without warranty of any kind. The author is not liable for any damages arising from the use of this project, whether commercial or non-commercial. By using this project, you acknowledge that you assume all risks associated with its use.

### How to Contact

For commercial use inquiries or any other questions regarding this license, please contact the author at [arshiakeshvariasl@gmail.com].

By using this project, you agree to abide by the terms set forth in this license.

*/

/*
  ESP32 Wi-Fi Temp Logger (Using MQTT / TCP)
  Includes ESP32 GPIO viewer for remote debugging purposes
  All temp data can easily be integrated with "NODE-RED"
  Author: Arshia Keshvari
  Current Date: 07/08/2024

  @WARNING: Should you want to use/modify/edit/reference it for commercial, corporate and company needs you 
  must gain full permission of usage and discuss licensing from myself. Otherwise, Feel free to play around 
  with the code to make it custom to your own requirements. 
  
  You are welcome to contact me should you have any general and technical questions.
*/

#include <DallasTemperature.h>
#include <OneWire.h> 
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <gpio_viewer.h> 

#define ONE_WIRE_BUS 25                           // Digital pin DS18B20 is connected to.
#define TEMPERATURE_PRECISION 12                  // Set sensor precision.  Valid options 8,10,11,12 Lower is faster but less precise

// Add your MQTT Broker IP address:
const char* mqtt_server = "xxx.xxx.xxx.xxx";

const char* ssid = "xxxx";      // SSID of the wireless network
const char* password = "xxxx";  // Password of the wireless network

// Uncomment and use only if you intend to have TCP Direct Communication with a Device see espClient segments if you wish to use uncomment and comment MQTT segments
//const char* laptop_ip = "xxx.xxx.xxx.xxx"; // IP for websocket
//const int laptop_port = 54321;             // Choose a port number for websocket

int updatePeriod = 10;                            //delay in seconds

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
GPIOViewer gpio_viewer;

String combinedSensorData = "{ \"SensorData\": ["; // Define a JSON-like string to hold the combined sensor data
int numberOfDevices;                               // Number of temperature devices found
DeviceAddress tempDeviceAddress;                   // We'll use this variable to store a found device address
WiFiClient espClient;                              // Create a WiFiClient instance
PubSubClient mqttClient(espClient);                // Create a MQTTClient instance using the WifiClient Instance

void setup() {
  Serial.begin(115200);

  wifi_setup();
  sensors.begin();
  mqttClient.setServer(mqtt_server, 9883);  // Set MQTT Port of your MQTT Broker (Server)
  //mqttClient.setCallback(callback);

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    Serial.print("Found device ");
    Serial.print(i, DEC);
    Serial.print(" with address: ");
    printAddress(tempDeviceAddress);
    Serial.println();
    
    Serial.print("Setting resolution to ");
    Serial.println(TEMPERATURE_PRECISION, DEC);
    
    // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions from 8-bit to 12-bit)
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    
    Serial.print("Resolution actually set to: ");
    Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
    Serial.println();
  }else{
    Serial.print("Found ghost device at ");
    Serial.print(i, DEC);
    Serial.print(" but could not detect address. Check power and cabling");
  }
  }
  gpio_viewer.begin();
}

void loop() {
  
  // Connect to the laptop if not already connected
  // if (!espClient.connected() && espClient.connect(laptop_ip, laptop_port)) {
  //   Serial.println("Connected to laptop");
  // }

  handleMQTTConnection();

  sensors.requestTemperatures();

  // Start the JSON-like message
  combinedSensorData = "{ \"SensorData\": [";

  // Itterate through each sensor and turn the readings into JSON format
  for (int i=0; i<=(numberOfDevices - 1); i++){
    float temp=sensors.getTempCByIndex(i);

    // Add sensor data to the JSON message
    combinedSensorData += "{ \"Sensor\": " + String(i) + ", \"Temperature\": " + String(temp) + " }";
    
    // Add a comma if it's not the last sensor
    if (i < (numberOfDevices - 1)) {
      combinedSensorData += ", ";
    }
  }

  // Close the JSON-like message
  combinedSensorData += "]}";
  Serial.println(combinedSensorData);

  // Publish combined sensor data to the "temperature" topic
  mqttClient.publish("ESP32/ds18b20/temperature", combinedSensorData.c_str());

  // // Send data to laptop
  // if (espClient.connected()) {
  //   espClient.println(combinedSensorData);
  // }

  // // Disconnect from the laptop after sending data
  // if (espClient.connected()) {
  //   espClient.stop();
  //   Serial.println("Disconnected from laptop");
  // }

  delay(updatePeriod * 1000);
}

void handleMQTTConnection() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();
}

// WiFi Setup Function
void wifi_setup(){
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // The ClientID is very important as it allows the broker to see what device is talking kinda like a mac address
    if (mqttClient.connect("ESP32Client", "public", "password")) {  // Remover User and Password if not required by your broker
      Serial.println("connected");
      mqttClient.subscribe("ESP32/ds18b20/temperature", 2); // Topic we want to subscribe to and the QOS ranking of MQTT
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

