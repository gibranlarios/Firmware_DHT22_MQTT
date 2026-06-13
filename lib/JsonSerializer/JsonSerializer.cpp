#include "JsonSerializer.h"

#include <ArduinoJson.h>

String JsonSerializer::telemetry(const char *deviceId, const char *deviceName, const SensorData &data)
{
    JsonDocument doc;
    String payload;

    doc["deviceId"] = deviceId;
    doc["deviceName"] = deviceName;
    doc["temperature"] = data.temperature;
    doc["humidity"] = data.humidity;

    serializeJson(doc, payload);
    return payload;
}
