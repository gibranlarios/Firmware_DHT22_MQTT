#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class MqttManager
{
public:
    MqttManager(const char *host, uint16_t port, const char *username, const char *password, const char *baseTopic);

    bool begin();
    void loop();
    bool isConnected();
    bool publish(const String &topic, const String &payload);

    String telemetryTopic() const;
    String statusTopic() const;
    String configTopic() const;

private:
    bool connect();
    String buildTopic(const char *suffix) const;

    WiFiClientSecure _secureClient;
    PubSubClient _mqttClient;
    const char *_host;
    uint16_t _port;
    const char *_username;
    const char *_password;
    const char *_baseTopic;
    unsigned long _lastReconnectAttempt;
};

#endif
