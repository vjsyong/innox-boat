#ifndef BOAT_CONTROLLER_H
#define BOAT_CONTROLLER_H

// ===========================================================================
//  BoatController  —  Edutech API for the InnoX Boat
//
//  Call these two functions from your main .ino / .cpp file:
//      setupBoatController();   // once in setup()
//      loopBoatController();    // every iteration of loop()
//
//  The system will loop-print the connection banner on Serial until a client
//  connects to the web page, so you never miss the SSID/IP.
//
//  ✏️  YOU MUST DEFINE these two functions:
//
//      onMotorCommand(char motor, int speed)
//          Called when the user drags a throttle slider.
//          motor : 'a' or 'b'
//          speed : -255 (full backward) … 0 (stop) … +255 (full forward)
//
//      onStartMotors()
//          Called once when the "START" button is pressed on the web page.
//          Play a chime sequence here, then set motorsEnabled = true.
// ===========================================================================

void setupBoatController();
void loopBoatController();

bool isClientConnected();

// ✏️  ← DELETE these declarations and let the STUDENT write them
void onMotorCommand(char motor, int speed);
void onStartMotors();

#endif // BOAT_CONTROLLER_H
