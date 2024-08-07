# ESP32_DS18B20_Wi-Fi_MQTT_TEMPERATURE_LOGGER

- A simple Wi-Fi based temperature logger for Steady-State or Dynamic thermal testing needs.
- MQTT / TCP Based Wi-Fi Communication Protocol Default is set to MQTT as it is most lightweight.
- Can be used with multiple OneWire DS18B20 temperature sensors and are automatically detected through the OneWire Protocol.
- Uses JSON Format for Serial Printing of temperature readings from OneWire DS18B20 sensors.
- The code already sets the ADC resolution of temperature sensors to 12-bit highest available (+/- 0.125 degrees celsius).
- Allows simple publishing of combined sensor data to an MQTT Topic "ESP32/ds18b20/temperature".
- Hardware Required: ESP32, 2.4-Ghz WiFi Network, 4.7k ohm resistor, DS18B20 Temperature Sensors, Micro-USB Power & Cable.

# ESP32 Pins Used For Connecting DS18B20 Sensors
Make sure to add a 4.7k ohm resistor between VCC and OneWire Connection to the DS18B20 Sensors !
- 3.3V == VCC
- GND  == GND
- (DQ) OneWire == Pin 25 (Pin Set by Default on ESP32)

![image](https://github.com/user-attachments/assets/f8ad2150-0c79-449b-992c-1d508d654830)
