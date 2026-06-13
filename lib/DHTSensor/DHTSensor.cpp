#include "DHTSensor.h"

#define DHT_TYPE DHT22

DHTSensor::DHTSensor(uint8_t pin)
    : _dht(pin, DHT_TYPE)
{
}

void DHTSensor::begin()
{
    _dht.begin();
}

bool DHTSensor::read(SensorData &data)
{
    const float humidity = _dht.readHumidity();
    const float temperature = _dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        return false;
    }

    data.humidity = humidity;
    data.temperature = temperature;
    return true;
}
