#include "MqttManager.h"

#include <WiFi.h>

namespace
{
const unsigned long MQTT_RECONNECT_INTERVAL_MS = 5000UL;
const char *STATUS_ONLINE = "online";
const char *STATUS_OFFLINE = "offline";
}

MqttManager::MqttManager(const char *host, uint16_t port, const char *username, const char *password, const char *baseTopic)
    : _mqttClient(_secureClient),
      _host(host),
      _port(port),
      _username(username),
      _password(password),
      _baseTopic(baseTopic),
      _lastReconnectAttempt(0)
{
}

bool MqttManager::begin()
{
    _secureClient.setInsecure();
    _mqttClient.setServer(_host, _port);

    return connect();
}

void MqttManager::loop()
{
    if (_mqttClient.connected())
    {
        _mqttClient.loop();
        return;
    }

    const unsigned long now = millis();
    if (now - _lastReconnectAttempt >= MQTT_RECONNECT_INTERVAL_MS)
    {
        _lastReconnectAttempt = now;
        connect();
    }
}

bool MqttManager::isConnected()
{
    return _mqttClient.connected();
}

bool MqttManager::publish(const String &topic, const String &payload)
{
    if (!isConnected())
    {
        Serial.println("[MQTT] Publish skipped. MQTT is not connected.");
        return false;
    }

    const bool sent = _mqttClient.publish(topic.c_str(), payload.c_str());

    Serial.print("[MQTT] Publish ");
    Serial.print(sent ? "OK" : "failed");
    Serial.print(" -> ");
    Serial.println(topic);

    return sent;
}

String MqttManager::telemetryTopic() const
{
    return buildTopic("telemetry");
}

String MqttManager::statusTopic() const
{
    return buildTopic("status");
}

String MqttManager::configTopic() const
{
    return buildTopic("config");
}

bool MqttManager::connect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[MQTT] Waiting for WiFi before connecting.");
        return false;
    }

    if (_host == nullptr || strlen(_host) == 0)
    {
        Serial.println("[MQTT] MQTT_HOST is empty. Edit include/Config.h.");
        return false;
    }

    const String clientId = String("esp32-") + String((uint32_t)ESP.getEfuseMac(), HEX);
    const String willTopic = statusTopic();

    Serial.print("[MQTT] Connecting to ");
    Serial.print(_host);
    Serial.print(":");
    Serial.println(_port);

    const bool connected = _mqttClient.connect(
        clientId.c_str(),
        _username,
        _password,
        willTopic.c_str(),
        1,
        true,
        STATUS_OFFLINE);

    if (!connected)
    {
        Serial.print("[MQTT] Connection failed. State: ");
        Serial.println(_mqttClient.state());
        return false;
    }

    Serial.println("[MQTT] Connected.");
    _mqttClient.publish(willTopic.c_str(), STATUS_ONLINE, true);
    Serial.print("[MQTT] Status online -> ");
    Serial.println(willTopic);

    return true;
}

String MqttManager::buildTopic(const char *suffix) const
{
    String topic(_baseTopic);
    topic += "/";
    topic += suffix;
    return topic;
}
