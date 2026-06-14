#ifndef APP_WIFI_MANAGER_H
#define APP_WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFiManager.h>

using WifiConnectionCallback = void (*)(bool connected);

class AppWifiManager
{
public:
    explicit AppWifiManager(unsigned long reconnectIntervalMs);

    bool begin();
    void loop();
    bool isConnected();
    String getIPAddress();
    void setConnectionCallback(WifiConnectionCallback callback);

private:
    WiFiManager _wifiManager;
    WifiConnectionCallback _connectionCallback = nullptr;
    bool _lastConnected = false;
    unsigned long _lastReconnectAttempt = 0;
    unsigned long _reconnectIntervalMs;
};

#endif
