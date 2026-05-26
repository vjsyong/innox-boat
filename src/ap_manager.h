#ifndef AP_MANAGER_H
#define AP_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>


// ---------------------------------------------------------------------------
// DO NOT TOUCH THIS FILE! — it's a simple wrapper around WiFi.softAP() to manage our AP

// THIS IS OUT OF SCOPE FOR STUDENTS
// ---------------------------------------------------------------------------


class APManager {
public:
  APManager();

  // Configuration
  void setSsid(const char *ssid);
  void setPassword(const char *password);
  void setChannel(uint8_t channel);   // 0 = auto-select on begin()
  void setMode(wifi_mode_t mode);     // e.g. WIFI_AP, WIFI_AP_STA

  // Actions
  int  findBestChannel();             // scan & return least-congested 2.4 GHz ch
  bool begin();                       // auto-select channel if 0, then start AP

  // Getters
  const char *getSsid() const;
  uint8_t     getChannel() const;
  IPAddress   getIp() const;

  // Static helpers
  static void generateSsidFromMac(char *buf, size_t len);

private:
  char        _ssid[32];
  char        _password[64];
  uint8_t     _channel;               // 0 = auto-select on begin()
  wifi_mode_t _wifiMode;
};

#endif // AP_MANAGER_H
