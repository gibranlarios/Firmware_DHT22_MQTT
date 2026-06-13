#include "AppWifiManager.h"

#include <WiFi.h>

bool AppWifiManager::begin()
{
    WiFi.mode(WIFI_STA);

    _wifiManager.setConfigPortalBlocking(true);
    _wifiManager.setConnectTimeout(30);
    _wifiManager.setConfigPortalTimeout(180);

    Serial.println("[WiFi] Connecting with WiFiManager...");

    const bool connected = _wifiManager.autoConnect("ESP32-DHT22-Setup");

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
