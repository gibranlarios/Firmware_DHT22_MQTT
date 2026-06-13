#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Arduino.h>
#include <DHT.h>

struct SensorData
{
    float temperature;
    float humidity;
};

class DHTSensor
{
public:
    explicit DHTSensor(uint8_t pin);

    void begin();
    bool read(SensorData &data);

private:
    DHT _dht;
};

#endif
