#include "main.h"
#include <fstream>

using namespace std;


const int chass_power_threshold = 3;

void flywheelPID(double target) {
  // Constants
  double kP = 0.3;
  double kV = .0354; 
  double threshold = 150;

  double error = 0;
  double prevError = 0;

  double output = 0;

  while (true) {

    // Proportional
    error = target - flywheel.get_actual_velocity()*6;

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

    flywheel.move(output);

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

void shoot() {
  spinIntake();
  indexer.set_value(true);
}

void stopShoot() {
  stopIntake();
  indexer.set_value(false);
}

void wait(int sec) {
  pros::delay(sec);
}

void endgame() {
  endgame1.set_value(true);
  endgame2.set_value(true);
}


void setChassisBreakMode(const pros::motor_brake_mode_e_t mode) {
    ldf.set_brake_mode(mode);
    ldm.set_brake_mode(mode);
    ldb.set_brake_mode(mode);
    rdf.set_brake_mode(mode);
    rdm.set_brake_mode(mode);
    rdb.set_brake_mode(mode);
}


void driveP(int targetLeft, int targetRight, int voltageMax=115) {
  ldb.tare_position(); // reset base encoders
  rdb.tare_position();

  bool debugLog = false; //debuging

	float kp = 0.15;
	float acc = 5;
	float kpTurn = 0.7;
	float accTurn = 4;

	// the untouchables
	float voltageLeft = 0;
	float voltageRight = 0;
	int errorLeft;
	int errorRight;
	int voltageCap = 0;
	int signLeft;
	int signRight;
	int errorCurrent = 0;
	int errorLast = 0;
	int sameErrCycles = 0;
	int same0ErrCycles = 0;
	int startTime = pros::millis();
	targetLeft = targetLeft + ldb.get_position();
	targetRight = targetRight + rdb.get_position();

	while(autonomous){
    std::cout << pros::millis() << (ldb.get_position() + rdb.get_position())/2 << std::endl;
		errorLeft = targetLeft - ldb.get_position(); // error is target minus actual value
    errorRight = targetRight - rdb.get_position();
		errorCurrent = (abs(errorRight) + abs(errorLeft)) / 2;

		signLeft = errorLeft / abs(errorLeft); // + or - 1
		signRight = errorRight / abs(errorRight);

		if(signLeft == signRight){
			voltageLeft = errorLeft * kp; // intended voltage is error times constant
			voltageRight = errorRight * kp;
			voltageCap = voltageCap + acc;  // slew rate
		}
		else{
			voltageLeft = errorLeft * kpTurn; // same logic with different turn value
			voltageRight = errorRight * kpTurn;
			voltageCap = voltageCap + accTurn;  // turn slew rate
		}
		
		if(voltageCap > voltageMax) voltageCap = voltageMax; // voltageCap cannot exceed 115

		if(abs(voltageLeft) > voltageCap) voltageLeft = voltageCap * signLeft; // limit the voltage
		if(abs(voltageRight) > voltageCap) voltageRight = voltageCap * signRight;// ditto

		// set the motors to the intended speed
		//chassis->getModel()->tank(voltageLeft/127, voltageRight/127);
        ldf.move(voltageLeft); 
        ldm.move(voltageLeft);
        ldb.move(voltageLeft);
        rdf.move(voltageRight);
        rdm.move(voltageRight);
        rdb.move(voltageRight);

		// timeout utility
		if (errorLast == errorCurrent) {
			if (errorCurrent <= 2) same0ErrCycles +=1; // less than 2 ticks is "0" error
			sameErrCycles += 1;
		}
		else {
			sameErrCycles = 0;
			same0ErrCycles = 0;
		}

		// exit paramaters
		if ((errorLast < 5 and errorCurrent < 5) or sameErrCycles >= 20) { // allowing for smol error or exit if we stay the same err for .4 second
			ldf.move_velocity(0); 
      ldm.move_velocity(0);
		  ldb.move_velocity(0);
		  rdf.move_velocity(0);
      rdm.move_velocity(0);
		  rdb.move_velocity(0);
			std::cout << pros::millis() << "task complete with error " << errorCurrent << " in " << (pros::millis() - startTime) << "ms" << std::endl;
			return;
		}
		
		// debug
		if (debugLog) {
			std::cout << pros::millis() << "error  " << errorCurrent << std::endl;
			std::cout << pros::millis() << "errorLeft  " << errorLeft << std::endl;
			std::cout << pros::millis() << "errorRight  " << errorRight << std::endl;
			std::cout << pros::millis() << "voltageLeft  " << voltageLeft << std::endl;
			std::cout << pros::millis() << "voltageRight  " << voltageRight << std::endl;
		}

		// nothing goes after this
		errorLast = errorCurrent;
		pros::delay(20);
	}
}

void chassisDriveDistance(int unit, int speed) {
    int direction = (speed/-speed)*-1;
    ldf.tare_position();
    ldf.tare_position();
    chassisDrive(speed * direction, speed * direction);
    pros::delay(200);
    while (true) {
        chassisDrive(speed * direction, speed * direction);
        pros::delay(20);
        auto pos = averageChassisMotorPosition();
        if ((abs(pos) - unit) >= 400) {
            break;
        }
    }
    while (true) {
        chassisDrive(30 * direction, 30 * direction);
        pros::delay(20);
        auto pos = averageChassisMotorPosition();
        if (abs(pos) >= unit) {
            break;
        }
    }
    chassisDrive(0, 0);
}


void chassisDrive(int left_power, int right_power) {
    if (abs(left_power) < chass_power_threshold) {
        left_power = 0;
    }
    if (abs(right_power) < chass_power_threshold) {
        right_power = 0;
    }

    ldf.move(left_power);
    ldm.move(left_power);
    ldb.move(left_power);
    rdf.move(right_power);
    rdm.move(right_power);
    rdb.move(right_power);
}

double averageChassisMotorPosition() {
    auto l_pos = ldf.get_position();
    auto r_pos = ldf.get_position(); 

        // cout << "left position: " << l_pos
        //     << ", right position: " << r_pos
        //     << endl;

    return (l_pos + r_pos) / 2;
}