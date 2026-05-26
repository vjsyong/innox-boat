#include "boat_controller.h"
#include "ap_manager.h"
#include "html.h"

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// ---------------------------------------------------------------------------
// Internal globals (hidden from the student)
// ---------------------------------------------------------------------------
static APManager         apManager;
static WebServer         httpServer(80);
static WebSocketsServer  wsServer(81);

// ---------------------------------------------------------------------------
// WebSocket event → calls onMotorCommand()
// ---------------------------------------------------------------------------
static void onWsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t len) {
  if (type == WStype_TEXT) {
    // Expected:  {"motor":"a","speed":127}
    //            speed range: -255 .. +255
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, (const char *)payload);

    if (err) {
      Serial.printf("[WS] JSON parse error: %s\n", err.c_str());
      return;
    }

    const char *motorStr = doc["motor"];
    int         speed    = doc["speed"] | 0;

    if (!motorStr || (motorStr[0] != 'a' && motorStr[0] != 'b')) {
      Serial.println("[WS] Invalid motor");
      return;
    }

    speed = constrain(speed, -255, 255);
    onMotorCommand(motorStr[0], speed);
  }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
void setupBoatController() {
  // --- WiFi AP -------------------------------------------------------------
  apManager.setMode(WIFI_AP);
  apManager.begin();

  // --- HTTP server — serve the control page --------------------------------
  httpServer.on("/", []() {
    httpServer.send(200, "text/html", INDEX_HTML);
  });
  httpServer.begin();

  // --- WebSocket server ----------------------------------------------------
  wsServer.begin();
  wsServer.onEvent(onWsEvent);

  Serial.printf("[Boat] Ready — connect to '%s' and browse to %s\n",
                apManager.getSsid(), apManager.getIp().toString().c_str());
}

void loopBoatController() {
  httpServer.handleClient();
  wsServer.loop();
}
