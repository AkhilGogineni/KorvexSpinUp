#include "main.h"
#include <fstream>

using namespace std;

bool pneumatics = true;
bool intakeSpin = false;
bool braketype = false;
bool angle = true;

void threadingChassis(void* p) {
    while(true) {
        if(abs(master.get_analog(ANALOG_LEFT_Y)) > 5) {
           ldf.move(master.get_analog(ANALOG_LEFT_Y));
           ldm.move(master.get_analog(ANALOG_LEFT_Y));
           ldb.move(master.get_analog(ANALOG_LEFT_Y));
          //intake.moveVelocity(master.get_analog(ANALOG_LEFT_Y));

        }
        else {
            ldf.move_voltage(0);
            ldm.move_voltage(0);
            ldb.move_voltage(0);
        }
        if(abs(master.get_analog(ANALOG_RIGHT_Y)) > 5) {
            rdf.move(master.get_analog(ANALOG_RIGHT_Y));
            rdm.move(master.get_analog(ANALOG_RIGHT_Y));
            rdb.move(master.get_analog(ANALOG_RIGHT_Y));
        }
        else {
            rdf.move_voltage(0);
            rdm.move_voltage(0);
            rdb.move_voltage(0);

        }
        pros::delay(10);

        std::cout << ": temp: " << intake.getActualVelocity() << std::endl;
    }
}

void threadingIntake(void* p) {
    
    while(true) {

        if(master.get_digital(DIGITAL_L1)) {
            spinIntake();
        } else {
            stopIntake();
        }
        if(master.get_digital(DIGITAL_Y)) {
            intake.moveVoltage(-12000);
        }
        if(master.get_digital(DIGITAL_R1)) {
            intake.moveVelocity(-450);
        }



        std::cout << ": temp: " << temp.get_position() << std::endl;
        pros::delay(10);
    }
}

void threadingEndgameMech(void* p) {
    while(true) {
        if(master.get_digital(DIGITAL_A) && master.get_digital(DIGITAL_LEFT)) {
            endgame();
        }
        pros::delay(10);
    }
}



void threadingFlywheel(void *p) {
    bool indexing = false;
        
    while(true) {
        if(master.get_digital_new_press(DIGITAL_L2)) {
            indexing = !indexing;
        } 

        if(indexing) {
            indexer.set_value(0);
        } else {
            indexer.set_value(1);
        }
        
        pros::delay(10);
    }

}

void threadingIndexing(void *p) {
    while(true) {       
        if(master.get_digital_new_press(DIGITAL_R2)) {
            angle = !angle;
        } 

        if(angle) {
            angler.set_value(0);
        } else {
            angler.set_value(1);
        }
        pros::delay(10);
    }
}


void flywheelP(void *p) {
    while(true) {
        if(master.get_digital_new_press(DIGITAL_RIGHT)) {
            set_flywheel_speed(2800);
        } else if(master.get_digital_new_press(DIGITAL_X)) {
            set_flywheel_speed(0);
        } else if(master.get_digital_new_press(DIGITAL_DOWN)) {
            set_flywheel_speed(2700);
        } else if(master.get_digital_new_press(DIGITAL_B)) {
            set_flywheel_speed(2900);
        }
        pros::delay(10);

    }
}

void threadingAngler(void *p) {
    while(true) {
        pros::delay(10);
    }
}


