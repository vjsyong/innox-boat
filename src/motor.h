#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Motor PWM Pins — Seeed XIAO ESP32C3
// ---------------------------------------------------------------------------
#define MOTOR_A1 5
#define MOTOR_A2 6
#define MOTOR_B1 8
#define MOTOR_B2 7

#define LED_A 20
#define LED_B 21

// ---------------------------------------------------------------------------
// Motor class
//  Controls one DC motor via two PWM pins (H-bridge).
//  forward(speed)  → pin1 PWM, pin2 LOW
//  backward(speed) → pin1 LOW, pin2 PWM
//  stop()          → both pins LOW
// ---------------------------------------------------------------------------
class Motor {
private:
  int pin1;
  int pin2;
  int channel1;
  int channel2;
  int currentSpeed;

  static constexpr int PWM_FREQ = 5000;
  static constexpr int PWM_RES  = 8;

public:
  Motor(int p1, int p2)
    : pin1(p1), pin2(p2), currentSpeed(0) {
    static int nextChannel = 0;
    channel1 = nextChannel++;
    channel2 = nextChannel++;
    ledcSetup(channel1, PWM_FREQ, PWM_RES);
    ledcSetup(channel2, PWM_FREQ, PWM_RES);
    ledcAttachPin(pin1, channel1);
    ledcAttachPin(pin2, channel2);
  }

  void forward(int speed = 255) {
    currentSpeed = constrain(speed, 0, 255);
    ledcWrite(channel1, currentSpeed);
    ledcWrite(channel2, 0);
  }

  void backward(int speed = 255) {
    currentSpeed = constrain(speed, 0, 255);
    ledcWrite(channel1, 0);
    ledcWrite(channel2, currentSpeed);
  }

  void stop() {
    currentSpeed = 0;
    ledcWrite(channel1, 0);
    ledcWrite(channel2, 0);
  }

  int getSpeed() const { return currentSpeed; }
};

#endif // MOTOR_H
