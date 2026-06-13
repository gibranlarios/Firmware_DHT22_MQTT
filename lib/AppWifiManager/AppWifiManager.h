#ifndef APP_WIFI_MANAGER_H
#define APP_WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFiManager.h>

class AppWifiManager
{
public:
    bool begin();
    void loop();
    bool isConnected();
    String getIPAddress();

private:
    WiFiManager _wifiManager;
};

#endif
