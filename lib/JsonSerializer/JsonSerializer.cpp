#include "JsonSerializer.h"

#include <ArduinoJson.h>

String JsonSerializer::telemetry(const String &deviceId, const char *deviceName, const String &macAddress, const SensorData &data)
{
    JsonDocument doc;
    String payload;

    doc["deviceId"] = deviceId;
    doc["deviceName"] = deviceName;
    doc["macAddress"] = macAddress;
    doc["temperature"] = data.temperature;
    doc["humidity"] = data.humidity;

    serializeJson(doc, payload);
    return payload;
}

String JsonSerializer::event(const String &deviceId, const char *deviceName, const String &macAddress, const DeviceEvent &event)
{
    JsonDocument doc;
    String payload;

    doc["deviceId"] = deviceId;
    doc["deviceName"] = deviceName;
    doc["macAddress"] = macAddress;
    doc["eventType"] = event.eventType;
    doc["message"] = event.message;
    doc["uptimeMs"] = event.uptimeMs;

    serializeJson(doc, payload);
    return payload;
}
