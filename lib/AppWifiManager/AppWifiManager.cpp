#include "AppWifiManager.h"

#include <WiFi.h>

AppWifiManager::AppWifiManager(unsigned long reconnectIntervalMs)
    : _reconnectIntervalMs(reconnectIntervalMs)
{
}

bool AppWifiManager::begin()
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);

    _wifiManager.setConfigPortalBlocking(true);
    _wifiManager.setConnectTimeout(30);
    _wifiManager.setConfigPortalTimeout(180);

    Serial.println("[WiFi] Connecting with WiFiManager...");

    const bool connected = _wifiManager.autoConnect("ESP32-DHT22-Setup");
    _lastConnected = connected;
    _lastReconnectAttempt = millis();

    if (connected)
    {
        Serial.print("[WiFi] Connected. IP: ");
        Serial.println(getIPAddress());
    }
    else
    {
        Serial.println("[WiFi] Connection failed or setup portal timed out.");
    }

    return connected;
}

void AppWifiManager::loop()
{
    _wifiManager.process();

    const bool connected = isConnected();
    if (connected != _lastConnected)
    {
        _lastConnected = connected;

        if (_connectionCallback != nullptr)
        {
            _connectionCallback(connected);
        }
    }

    if (connected)
    {
        return;
    }

    const unsigned long now = millis();
    if (now - _lastReconnectAttempt >= _reconnectIntervalMs)
    {
        _lastReconnectAttempt = now;
        Serial.println("[WiFi] Attempting reconnection...");
        WiFi.reconnect();
    }
}

bool AppWifiManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

String AppWifiManager::getIPAddress()
{
    if (!isConnected())
    {
        return "";
    }

    return WiFi.localIP().toString();
}

void AppWifiManager::setConnectionCallback(WifiConnectionCallback callback)
{
    _connectionCallback = callback;
}
