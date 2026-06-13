#include <Arduino.h>

#include <AppWifiManager.h>
#include <Config.h>
#include <DHTSensor.h>
#include <JsonSerializer.h>
#include <MqttManager.h>

AppWifiManager wifi;
DHTSensor sensor(DHT_PIN);
MqttManager mqtt(MQTT_HOST, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD, MQTT_BASE_TOPIC);

unsigned long lastTelemetryAt = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("[App] Starting ESP32 IoT base...");

    sensor.begin();
    wifi.begin();
    mqtt.begin();
}

void loop()
{
    wifi.loop();
    mqtt.loop();

    const unsigned long now = millis();
    if (now - lastTelemetryAt < TELEMETRY_INTERVAL_MS)
    {
        return;
    }

    lastTelemetryAt = now;

    if (!wifi.isConnected())
    {
        Serial.println("[App] Telemetry skipped. WiFi is not connected.");
        return;
    }

    if (!mqtt.isConnected())
    {
        Serial.println("[App] Telemetry skipped. MQTT is not connected.");
        return;
    }

    SensorData data;
    if (!sensor.read(data))
    {
        Serial.println("[DHT22] Failed to read sensor data.");
        return;
    }

    const String payload = JsonSerializer::telemetry(DEVICE_ID, DEVICE_NAME, data);
    mqtt.publish(mqtt.telemetryTopic(), payload);
}
