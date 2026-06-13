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
