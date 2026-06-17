#include <Arduino.h>

#include <AppWifiManager.h>
#include <Config.h>
#include <DHTSensor.h>
#include <EventQueue.h>
#include <JsonSerializer.h>
#include <MqttManager.h>
#include <WiFi.h>

AppWifiManager wifi(WIFI_RECONNECT_INTERVAL_MS);
DHTSensor sensor(DHT_PIN);
MqttManager mqtt(
    MQTT_HOST,
    MQTT_PORT,
    MQTT_USERNAME,
    MQTT_PASSWORD,
    MQTT_BASE_TOPIC,
    MQTT_RECONNECT_INTERVAL_MS);
EventQueue<EVENT_QUEUE_SIZE> eventQueue;

unsigned long lastTelemetryAt = 0;
String macAddress;
String deviceId;
String telemetryTopic;
String eventsTopic;
bool sensorHealthy = true;
bool telemetryPublishHealthy = true;

String buildDeviceId(const String &mac)
{
    String id = mac;
    id.replace(":", "");
    id.toUpperCase();
    return id;
}

void handleWifiConnection(bool connected)
{
    eventQueue.enqueue(
        connected ? "wifi_reconnected" : "wifi_disconnected",
        connected ? "WiFi connection restored" : "WiFi connection lost",
        millis());
}

void handleMqttConnection(bool connected)
{
    eventQueue.enqueue(
        connected ? "mqtt_reconnected" : "mqtt_disconnected",
        connected ? "MQTT connection restored" : "MQTT connection lost",
        millis());
}

void enqueueEvent(const String &eventType, const String &message)
{
    eventQueue.enqueue(eventType, message, millis());
}

void updateSensorHealth(bool healthy)
{
    if (healthy == sensorHealthy)
    {
        return;
    }

    sensorHealthy = healthy;
    enqueueEvent(
        healthy ? "sensor_recovered" : "sensor_failed",
        healthy ? "DHT22 sensor recovered" : "DHT22 sensor read failed");
}

void updateTelemetryPublishHealth(bool healthy)
{
    if (healthy == telemetryPublishHealthy)
    {
        return;
    }

    telemetryPublishHealthy = healthy;
    enqueueEvent(
        healthy ? "telemetry_publish_recovered" : "telemetry_publish_failed",
        healthy ? "Telemetry publish recovered" : "Telemetry publish failed");
}

void publishPendingEvent()
{
    if (!mqtt.isConnected() || eventQueue.isEmpty())
    {
        return;
    }

    DeviceEvent event;
    if (!eventQueue.peek(event))
    {
        return;
    }

    const String payload = JsonSerializer::event(deviceId, DEVICE_NAME, macAddress, event);
    if (mqtt.publish(eventsTopic, payload))
    {
        eventQueue.pop();
    }
}

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("[App] Starting ESP32 IoT base...");

    sensor.begin();
    wifi.setConnectionCallback(handleWifiConnection);
    mqtt.setConnectionCallback(handleMqttConnection);
    wifi.begin();

    macAddress = WiFi.macAddress();
    deviceId = buildDeviceId(macAddress);
    telemetryTopic = String("devices/") + deviceId + "/telemetry";
    eventsTopic = String("devices/") + deviceId + "/events";

    Serial.print("Device Name: ");
    Serial.println(DEVICE_NAME);
    Serial.print("MAC Address: ");
    Serial.println(macAddress);
    Serial.print("Device ID: ");
    Serial.println(deviceId);

    mqtt.begin();
}

void loop()
{
    wifi.loop();
    mqtt.loop();
    publishPendingEvent();

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
        updateSensorHealth(false);
        return;
    }

    updateSensorHealth(true);

    const String payload = JsonSerializer::telemetry(deviceId, DEVICE_NAME, macAddress, data);
    updateTelemetryPublishHealth(mqtt.publish(telemetryTopic, payload));
}
