#ifndef JSON_SERIALIZER_H
#define JSON_SERIALIZER_H

#include <Arduino.h>
#include <DHTSensor.h>

class JsonSerializer
{
public:
    static String telemetry(const char *deviceId, const char *deviceName, const SensorData &data);
};

#endif
