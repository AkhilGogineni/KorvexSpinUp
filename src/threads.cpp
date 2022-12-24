#include "main.h"
#include <fstream>

using namespace std;

bool pneumatics = false;
bool intakeSpin = false;
bool braketype = false;
 
void threadingChassis(void* p) {
    while(true) {
        if(abs(master.get_analog(ANALOG_LEFT_Y)) > 5) {
           leftDrive.moveVoltage(-master.get_analog(ANALOG_LEFT_Y)/127.0*12000);

        }
        else {
            leftDrive.moveVoltage(0);
        }
        if(abs(master.get_analog(ANALOG_RIGHT_Y)) > 5) {
            rightDrive.moveVoltage(-master.get_analog(ANALOG_RIGHT_Y)/127.0*12000);
        }
        else {
            rightDrive.moveVoltage(0);

        }

        // if(master.get_digital_new_press(DIGITAL_Y) && braketype == false) {
        //     chassis.set_drive_brake(MOTOR_BRAKE_HOLD);
        // } else if(master.get_digital_new_press(DIGITAL_Y) && braketype == true) {
        //     chassis.set_drive_brake(MOTOR_BRAKE_COAST);
        // }
        pros::delay(10);
    }
}

void threadingIntake(void* p) {
    while(true) {

        if(master.get_digital(DIGITAL_L1)) {
            intake.moveVelocity(600);
        } else {
            intake.moveVelocity(0);
        }
        if(master.get_digital(DIGITAL_RIGHT)) {
            intake.moveVelocity(-600);
        }

        pros::delay(10);
    }
}

void threadingEndgameMech(void* p) {
    while(true) {
        if(master.get_digital_new_press(DIGITAL_LEFT)) {
            piston1.set_value(false);
            
        } else if(master.get_digital_new_press(DIGITAL_A)) {
            piston2.set_value(false);
        }
        pros::delay(10);
    }
}



/*
void threadingFlywheel(void* p) {
    while(true) {

        //std::cout << pros::millis() << "Target: " << target << std::endl;

        float kp = 1.6;
        float ki = 0.0;
        float kd = 0.0;

        // the untouchables
        int voltageCap = 0;
        int voltageMax = 127;
        float voltage = 0;
        float errorCurrent;
        float errorLast;
        int errorCurrentInt;
        int errorLastInt;
        int sameErrCycles = 0;
        int same0ErrCycles = 0;
        int p;
        float i;
        int d;
        int sign;
        float error;

        if(master.get_digital(DIGITAL_L1)) {
            target = liftMax;
            liftUp = true;
            trigger = true;
        } else if(master.get_digital(DIGITAL_L2)) {
            target = liftMin;
            liftUp = false;
            trigger = true;
        } else if(trigger == true and (!master.get_digital(DIGITAL_L1) and !master.get_digital(DIGITAL_L2)))  {
            target = liftEncoder.get();
            trigger = false;
        }

        //while(true) {
            error = target - liftEncoder.get();
            errorCurrent = abs(error);
            errorCurrentInt = errorCurrent;
            sign = target / abs(target); // -1 or 1

            p = (error * kp);
            if (abs(error) < 10) { // if we are in range for I to be desireable
                i = ((i + error) * ki);
            }
            else
                i = 0;
            d = (error - errorLast) * kd;
            
            voltage = p + i + d;

            if(abs(voltage) > voltageMax) voltage = voltageMax * sign;

            // set the motors to the intended speed
            frontLift.move(voltage);
            // timeout utility
            if (errorLastInt == errorCurrentInt) {
                if (errorLast <= 2 and errorCurrent <= 2) { // saying that error less than 2 counts as 0
                    same0ErrCycles +=1;
                }
                sameErrCycles += 1;
            }
            else {
                sameErrCycles = 0;
                same0ErrCycles = 0;
            }

            //exit paramaters
            if (same0ErrCycles >= 5 or sameErrCycles >= 60) { // allowing for smol error or exit if we stay the same err for .6 second
                frontLift.move_velocity(0);		
                return;
            }        

            errorLast = errorCurrent;
            errorLastInt = errorLast;
            //pros::delay(10);
        //}
            pros::delay(10);
        }
  }  
*/

void threadingFlywheel(void *p) {
    
    while(true) {
        if(master.get_digital_new_press(DIGITAL_R2)) {
            indexer.set_value(0);
            pros::delay(200);
            indexer.set_value(1);
            pros::delay(200);
            indexer.set_value(0);
            pros::delay(200);
            indexer.set_value(1);
            pros::delay(200);
            indexer.set_value(0);
            pros::delay(300);
            indexer.set_value(1);
        }
        if(master.get_digital_new_press(DIGITAL_R1)) {
            indexer.set_value(0);
            pros::delay(150);
            indexer.set_value(1);
        }
        
        pros::delay(10);
    }

}

void flywheelP(void *p) {
    while(true) {
        if(master.get_digital_new_press(DIGITAL_L2)) {
            flywheelPID(400);
        } else if(master.get_digital_new_press(DIGITAL_R2)) {
            flywheelPID(460);
            pros::delay(200);
            flywheelPID(515);
            pros::delay(100);
            flywheelPID(600);
            pros::delay(1000);
            flywheelPID(350);
        } 
        if(master.get_digital_new_press(DIGITAL_X)) {
            flywheelPID(0);
        }
        pros::delay(10);

    }
}

