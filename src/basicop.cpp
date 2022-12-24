#include "main.h"
#include <fstream>

using namespace std;

void flywheelPID(double target) {
  // Constants
  double kP = 0.5;//.1
  double kI = 0;
  double kD = 0;
  double kV = 127.0/3600;

  double error = 0;
  double errorInertial = 0;
  double totalError = 0;
  double prevError = 0;

  double derivative;

  //reset sensors


  while (true) {

    // Proportional
    error = target - flywheel.getActualVelocity();

    // Integral
    totalError += error;

    // Derivative
    derivative = error - prevError;

    // Set speed of flywhell
    double motorPower = (kP *error) + (kI * totalError) + (kD * derivative);

    // Sets the speed of the flywheel

    if(motorPower > 127){
      motorPower = 127;
    }
    else if(motorPower < 0){
      motorPower = 0; 
    }

    flywheel.moveVoltage((target*kV + motorPower)/127*12000);
    //flywheel.moveVoltage(120);
    printf("flywheel: %d", flywheel.getActualVelocity());
    printf("\n");

    prevError = error;

    pros::delay(10);

  }

}

void flywheelPID2(double target) {
  // Constants
  double kP = 0.3;//.1
  double kV = .0354;
  double threshold = 150;

  double error = 0;
  double prevError = 0;

  double output = 0;

  //reset sensors


  while (true) {

    // Proportional
    error = target - flywheel.getActualVelocity()*6;

    // Set speed of flywheel
    if (error > threshold){
      output = 127;
    }
    else if (error < -threshold){
      output = 0;
    }
    else{
      output = (kV * target) + (kP * error) ;
    }

    // Sets the speed of the flywheel
    
    if(output > 127){
      output = 127;
    }
    else if(output < 0){
      output = 0; 
    }

    flywheel.moveVelocity(output);

    //printf("flywheel: %f", flywheel.get_actual_velocity()*6);
    //printf("\n");

    prevError = error;
    
    pros::delay(10);

  }
  
}


void set_flywheel_speed(int speed) {
  static std::unique_ptr<pros::Task> pidTask {};
  if (pidTask != nullptr) { pidTask->remove(); }
  pidTask = (speed == -1) ? nullptr : std::make_unique<pros::Task>([=]{ flywheelPID(speed); });
}

void spinIntake() {
  intake.moveVelocity(600);
}

void outtake() {
  intake.moveVelocity(-600);
}

void spinRollers() {
  intake.moveVelocity(400);
}

void stopIntake() {
  intake.moveVelocity(0);
}

void tripleShot() {
  indexer.set_value(0);
  pros::delay(150);
  indexer.set_value(1);
  pros::delay(2000);
  indexer.set_value(0);
  pros::delay(150);
  indexer.set_value(1);
}

void wait(int sec) {
  pros::delay(sec);
}

void endgame() {
  piston1.set_value(false);
  piston2.set_value(false);
}

void shoot(int target, int shots){
    int counter = 0;
    while(counter < shots){
  
        if(flywheel.getActualVelocity()*6 > (target - 30)){
            printf("flywheel: %f", flywheel.getActualVelocity()*6);
            indexer.set_value(0);
            pros::delay(200);
            indexer.set_value(1);
            counter++;
        }
        pros::delay(10);
    }
}