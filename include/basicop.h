#include "main.h"
using namespace okapi;


extern void flywheelPID(double target);
extern void flywheelPID2(double target);
extern void set_flywheel_speed(int speed);

extern void spinIntake();
extern void outtake();
extern void spinRollers();
extern void stopIntake();
extern void tripleShot();
extern void wait(int sec);
extern void endgame();
void shoot(int target, int shots);