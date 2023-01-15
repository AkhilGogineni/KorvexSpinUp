#include "main.h"
using namespace okapi;


extern void flywheelPID(double target);

extern void set_flywheel_speed(int speed);

extern void spinIntake();
extern void outtake();
extern void spinRollers();
extern void stopIntake();
extern void shoot();
extern void stopShoot();
extern void wait(int sec);
extern void endgame();
extern void shoot(int target, int shots);
extern void setChassisBreakMode(const pros::motor_brake_mode_e_t mode);
extern void driveP(int targetLeft, int targetRight, int voltageMax);
extern void chassisDrive(int left_power, int right_power);
extern void chassisDriveDistance(int unit, int speed);
extern double averageChassisMotorPosition();
extern void shoott(int target, int shots);