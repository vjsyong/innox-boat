#include "boat_controller.h"
#include "motor.h"
#include "chime.h"

// ---------------------------------------------------------------------------
// Motor instances
// ---------------------------------------------------------------------------
Motor motorA(MOTOR_A1, MOTOR_A2);
Motor motorB(MOTOR_B1, MOTOR_B2);

// Non-blocking chime player — auto-advances through a note sequence
ChimePlayer chime;

// ===========================================================================
//  ✏️  TASK 1: Play a chime when START is pressed
// ===========================================================================
void onStartMotors() {
  // ✏️  Write your chime here. See README.md for step-by-step examples.
  //     Queue notes with chime.add() / chime.startStep(), then chime.play().

  Serial.println("[Motor] Chime started — motors enabled");
}

// ===========================================================================
//  ✏️  TASK 2: Control motor speed & direction from throttle sliders
// ===========================================================================
void onMotorCommand(char motor, int speed) {
  // These are given to you — no pointers needed!
  Motor *m   = (motor == 'b') ? &motorB : &motorA;
  int ledPin = (motor == 'b') ? LED_B  : LED_A;

  // ✏️  Fill in the logic below.  See README.md for step-by-step examples.
  //     speed > 0  →  m->forward(speed)   + LED on
  //     speed < 0  →  m->backward(-speed)  + LED on
  //     speed == 0 →  m->stop()            + LED off

  if (speed > 0) {

  } else if (speed < 0) {

  } else {

  }
}

// ---------------------------------------------------------------------------
// Startup chime — plays a short descending harmony on boot
// ---------------------------------------------------------------------------
void playStartupChime() {
  chime.startStep();
  chime.addToStep(&motorA, NOTE_G4, 50);     // G4
  chime.addToStep(&motorB, NOTE_E4, 50);     // E4  —  minor 3rd below
  chime.endStep(200);
  chime.startStep();
  chime.addToStep(&motorA, NOTE_F4, 50);     // F4
  chime.addToStep(&motorB, NOTE_D4, 50);     // D4  —  minor 3rd below
  chime.endStep(200);
  chime.startStep();
  chime.addToStep(&motorA, NOTE_E4, 50);     // E4
  chime.addToStep(&motorB, NOTE_C4, 50);     // C4  —  major 3rd below
  chime.endStep(600);
  chime.startStep();
  chime.addToStep(&motorA, NOTE_G4, 50);     // G4
  chime.addToStep(&motorB, NOTE_E4, 50);     // E4  —  minor 3rd below
  chime.endStep(600);
  chime.startStep();
  chime.addToStep(&motorA, NOTE_C5, 50);     // C5
  chime.addToStep(&motorB, NOTE_G4, 50);     // G4  —  perfect 4th below
  chime.endStep(600);
  chime.play();
}

// ---------------------------------------------------------------------------
// Setup & Loop
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_A, LOW);    // both LEDs start OFF
  digitalWrite(LED_B, LOW);

  // Brief USB CDC wait (250 ms is usually enough on CDC-with-wait-loop)
  for (int i = 0; i < 4; i++) {
    delay(50);
    if (Serial) break;
  }

  Serial.println();
  Serial.println("========================================");
  Serial.println("       ⛵  INNOX BOAT CONTROLLER");
  Serial.println("========================================");
  Serial.flush();

  setupBoatController();

  digitalWrite(LED_A, HIGH);   // LED on during startup chime
  playStartupChime();
  digitalWrite(LED_A, LOW);    // startup chime scheduled, LED off

  // === MIDI song easter egg ===
  // Uncomment the block below to play a converted MIDI song instead of the
  // startup chime.  Regenerate src/song.h from a .mid file using:
  //   python tools/midi_to_chime.py tools/your_song.mid
  //
  // #include "song.h"
  // Motor* trackMap[] = { &motorA, &motorB };
  // chime.loadSong(&songData, trackMap);
  // chime.play();

  digitalWrite(LED_A, LOW);    // startup chime scheduled, LED off

  // The banner will keep repeating every 3 s in loopBoatController() until
  // a client connects, so you never miss the SSID/IP even with USB CDC lag.
}

void loop() {
  // Advance any playing chime notes
  motorA.update();
  motorB.update();
  chime.update();

  loopBoatController();
}