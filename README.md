# ESP32_DS18B20_Wi-Fi_MQTT_TEMPERATURE_LOGGER

- A simple Wi-Fi based temperature logger for Steady-State or Dynamic thermal testing needs.
- MQTT / TCP Based Wi-Fi Communication Protocol Default is set to MQTT as it is most lightweight.
- Can be used with multiple OneWire DS18B20 temperature sensors and are automatically detected through the OneWire Protocol.
- Uses JSON Format for Serial Printing of temperature readings from OneWire DS18B20 sensors.
- The code already sets the ADC resolution of temperature sensors to 12-bit highest available (+/- 0.125 degrees celsius).
- Allows simple publishing of combined sensor data to an MQTT Topic "ESP32/ds18b20/temperature".
- Hardware Required: ESP32, 2.4-Ghz WiFi Network, 4.7k ohm resistor, DS18B20 Temperature Sensors, Micro-USB Power & Cable.
- Can be easily integrated to a "NODE-RED" dashboard.

# ESP32 Pins Used For Connecting DS18B20 Sensors
Make sure to add a 4.7k ohm resistor between VCC and OneWire Connection to the DS18B20 Sensors !
- 3.3V == VCC
- GND  == GND
- (DQ) OneWire or Data == Pin 25 (Pin Set by Default on ESP32)

![image](https://github.com/user-attachments/assets/f8ad2150-0c79-449b-992c-1d508d654830)

### Non-Commercial Use

You are free to use, modify, and distribute this project for non-commercial purposes as long as you provide appropriate credit and link to the original repository.

### Commercial Use

If you wish to use this project for commercial purposes, including but not limited to incorporation into products or services for sale, you must contact the author at arshiakeshvariasl@gmail.com to obtain permission and discuss licensing terms.

### Disclaimer

This project is provided "as-is" without warranty of any kind. The author is not liable for any damages arising from the use of this project.

### Additional Notes
Should you want to use/modify/edit/reference it for commercial, corporate and company needs you must gain full permission of usage and discuss licensing from myself. Otherwise, Feel free to play around with the code to make it custom to your own requirements. 
  
You are welcome to contact me should you have any general and technical questions.
