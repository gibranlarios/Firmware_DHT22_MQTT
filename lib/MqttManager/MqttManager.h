#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

using MqttConnectionCallback = void (*)(bool connected);

class MqttManager
{
public:
    MqttManager(
        const char *host,
        uint16_t port,
        const char *username,
        const char *password,
        const char *baseTopic,
        unsigned long reconnectIntervalMs);

    bool begin();
    void loop();
    bool isConnected();
    bool publish(const String &topic, const String &payload);
    void setConnectionCallback(MqttConnectionCallback callback);

    String telemetryTopic() const;
    String statusTopic() const;
    String configTopic() const;

private:
    bool connect();
    String buildTopic(const char *suffix) const;
    void updateConnectionState(bool connected);

    WiFiClientSecure _secureClient;
    PubSubClient _mqttClient;
    const char *_host;
    uint16_t _port;
    const char *_username;
    const char *_password;
    const char *_baseTopic;
    unsigned long _lastReconnectAttempt;
    MqttConnectionCallback _connectionCallback;
    bool _lastConnected;
    unsigned long _reconnectIntervalMs;
};

#endif
