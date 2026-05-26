#include "ap_manager.h"


// ---------------------------------------------------------------------------
// DO NOT TOUCH THIS FILE! — it's a simple wrapper around WiFi.softAP() to manage our AP

// THIS IS OUT OF SCOPE FOR STUDENTS
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Constructor — generates an SSID from the MAC and defaults to open AP on ch 0
// ---------------------------------------------------------------------------
APManager::APManager()
  : _channel(0)
  , _wifiMode(WIFI_AP)
{
  _password[0] = '\0';
  generateSsidFromMac(_ssid, sizeof(_ssid));
}

// ---------------------------------------------------------------------------
// Configuration setters
// ---------------------------------------------------------------------------
void APManager::setSsid(const char *ssid) {
  strncpy(_ssid, ssid, sizeof(_ssid) - 1);
  _ssid[sizeof(_ssid) - 1] = '\0';
}

void APManager::setPassword(const char *password) {
  strncpy(_password, password, sizeof(_password) - 1);
  _password[sizeof(_password) - 1] = '\0';
}

void APManager::setChannel(uint8_t channel) {
  _channel = channel;
}

void APManager::setMode(wifi_mode_t mode) {
  _wifiMode = mode;
}

// ---------------------------------------------------------------------------
// findBestChannel — scan 2.4 GHz channels 1-13, return the least congested
// ---------------------------------------------------------------------------
int APManager::findBestChannel() {
  static constexpr int NUM_CHANNELS = 13;
  int channelCounts[NUM_CHANNELS] = {0};

  Serial.println("[AP] Scanning WiFi channels for least congestion...");

  // Perform a full scan — 200ms per channel is plenty
  int n = WiFi.scanNetworks(false, true, false, 200);

  if (n == WIFI_SCAN_FAILED) {
    Serial.println("[AP] WiFi scan failed — falling back to channel 6");
    WiFi.scanDelete();
    return 6;
  }

  // Tally networks per channel
  for (int i = 0; i < n; i++) {
    int8_t ch = WiFi.channel(i);
    if (ch >= 1 && ch <= NUM_CHANNELS) {
      channelCounts[ch - 1]++;
    }
  }

  // Pick the channel with the fewest networks (ties → lowest channel)
  int bestChannel = 1;
  int minCount = channelCounts[0];
  for (int i = 1; i < NUM_CHANNELS; i++) {
    if (channelCounts[i] < minCount) {
      minCount = channelCounts[i];
      bestChannel = i + 1;
    }
  }

  WiFi.scanDelete();

#ifdef DEBUG_AP_SCAN
  Serial.println("[AP] Channel scan results:");
  for (int i = 0; i < NUM_CHANNELS; i++) {
    Serial.printf("  ch %2d: %d networks\n", i + 1, channelCounts[i]);
  }
  Serial.printf("[AP] Best channel: %d (least congested)\n", bestChannel);
#endif

  return bestChannel;
}

// ---------------------------------------------------------------------------
// begin — auto-select channel if 0, then start the AP
// ---------------------------------------------------------------------------
bool APManager::begin() {
  // MUST set WiFi mode before scanning — otherwise scan hangs on ESP32
  WiFi.mode(_wifiMode);
  delay(100);

  // Auto-select channel if not explicitly set
  if (_channel == 0) {
    int ch = findBestChannel();
    _channel = (uint8_t)ch;
  }

  bool ok;
  if (_password[0] == '\0') {
    // Open AP (no password)
    ok = WiFi.softAP(_ssid, nullptr, _channel);
  } else {
    ok = WiFi.softAP(_ssid, _password, _channel);
  }

  Serial.printf("[AP] SSID: \"%s\"  Channel: %u  IP: %s  %s\n",
                _ssid, _channel,
                WiFi.softAPIP().toString().c_str(),
                ok ? "OK" : "FAILED");

  return ok;
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------
const char *APManager::getSsid() const {
  return _ssid;
}

uint8_t APManager::getChannel() const {
  return _channel;
}

IPAddress APManager::getIp() const {
  return WiFi.softAPIP();
}

// ---------------------------------------------------------------------------
// Static: generate an SSID like "InnoX-Boat-XXXX" from the MAC address
// ---------------------------------------------------------------------------
void APManager::generateSsidFromMac(char *buf, size_t len) {
  uint64_t mac = ESP.getEfuseMac();
  uint16_t chipId = (uint16_t)(mac >> 24);
  snprintf(buf, len, "InnoX-Boat-%04X", chipId);
}
