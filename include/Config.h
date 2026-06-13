#ifndef CONFIG_H
#define CONFIG_H

// Device identity
// Edit these values to identify this device in MQTT payloads and dashboards.
#define DEVICE_NAME "ESP32 DHT22-Test"

// Sensor
// DHT22 data pin connected to ESP32 GPIO15.
#define DHT_PIN 15

// MQTT broker
// Edit these values with your HiveMQ Cloud broker information.
#define MQTT_HOST "8c9db9d7afa847bf80a2754aa740d0b2.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USERNAME "gibranlarios"
#define MQTT_PASSWORD "esp32GLV"

// MQTT topics
// Edit MQTT_BASE_TOPIC only if you want to change your topic convention.
// Derived topics:
//   devices/esp32-test/telemetry
//   devices/esp32-test/status
//   devices/esp32-test/config
#define MQTT_BASE_TOPIC "devices/esp32-test"

// Telemetry
// Publishing interval in milliseconds.
#define TELEMETRY_INTERVAL_MS 10000UL

#endif
