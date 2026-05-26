#include "boat_controller.h"
#include "motor.h"

// ---------------------------------------------------------------------------
// Motor instances
// ---------------------------------------------------------------------------
Motor motorA(MOTOR_A1, MOTOR_A2);
Motor motorB(MOTOR_B1, MOTOR_B2);

// ---------------------------------------------------------------------------
// ✏️  STUDENT TASK: Map a signed speed (-255..+255) to motor PWM control.
//
//     motor  : 'a' or 'b'
//     speed  : -255 = full backward  →  0 = stop  →  +255 = full forward
//
//     Available methods on Motor:
//       forward(int speed)   — pin1 PWM, pin2 LOW  (speed: 0-255)
//       backward(int speed)  — pin1 LOW, pin2 PWM  (speed: 0-255)
//       stop()               — both pins LOW
//       getSpeed()           — returns last set speed
//
//     LED pins: LED_A (20), LED_B (21)
//     Use digitalWrite(pin, HIGH/LOW) to light the corresponding LED.
// ---------------------------------------------------------------------------
void onMotorCommand(char motor, int speed) {
  // === ✏️  YOUR CODE HERE ===

  Motor *m = (motor == 'b') ? &motorB : &motorA;
  int ledPin = (motor == 'b') ? LED_B : LED_A;

  if (speed > 0) {
    m->forward(speed);
    digitalWrite(ledPin, HIGH);
    Serial.printf("Motor %c → FWD  speed=%d\n", motor, speed);
  } else if (speed < 0) {
    m->backward(-speed);
    digitalWrite(ledPin, HIGH);
    Serial.printf("Motor %c → BWD  speed=%d\n", motor, -speed);
  } else {
    m->stop();
    digitalWrite(ledPin, LOW);
    Serial.printf("Motor %c → STOP\n", motor);
  }
}

// ---------------------------------------------------------------------------
// Setup & Loop
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Wait for USB CDC serial to enumerate (ESP32-C3 quirk)
  pinMode(LED_A, OUTPUT);
  digitalWrite(LED_A, HIGH);   // LED on = waiting for serial
  for (int i = 0; i < 20; i++) {
    delay(250);
    if (Serial) break;         // USB CDC connected
  }

  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_B, LOW);

  Serial.println("\n\n=== InnoX Boat Controller ===");
  Serial.flush();

  setupBoatController();

  digitalWrite(LED_A, LOW);
}

void loop() {
  loopBoatController();
}