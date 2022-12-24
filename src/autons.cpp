#include "main.h"


/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// r
/////


const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;



///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 0, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}



void exit_condition_defaults() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition() {
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}



///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches

  std::cout << pros::millis() << "running!" << std::endl;
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  
  chassis.wait_drive();

  // chassis.set_drive_pid(-12, DRIVE_SPEED);
  // chassis.wait_drive();

  // chassis.set_drive_pid(-12, DRIVE_SPEED);
  // chassis.wait_drive();
}



///
// Turn Example
///
void turn_example() {
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at


  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
}



///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_drive();
}



///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // wait_until will wait until the robot gets to a desired position


  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_until(-6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();
}



///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive


  chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(12);

  chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}



///
// Auto that tests everything
///
void combining_movements() {
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_swing_pid(ez::RIGHT_SWING, -45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_drive();
}



///
// Interference example
///
void tug (int attempts) {
  for (int i=0; i<attempts-1; i++) {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered) {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees. 
// If interfered, robot will drive forward and then attempt to drive backwards. 
void interfered_example() {
 chassis.set_drive_pid(24, DRIVE_SPEED, true);
 chassis.wait_drive();

 if (chassis.interfered) {
   tug(3);
   return;
 }

 chassis.set_turn_pid(90, TURN_SPEED);
 chassis.wait_drive();
}


void RedLeft() {
  wait(1400);
  spinRollers();
  chassis.set_drive_pid(-3, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(2, DRIVE_SPEED, true);
  chassis.wait_drive();
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(2, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(1300);
  chassis.set_turn_pid(-8, TURN_SPEED);
  chassis.wait_drive();
  //outtake();
  wait(500);
  //shoot(550, 3);
  tripleShot();
  // wait(300);
  // set_flywheel_speed(500);
  // spinRollers();
  // chassis.set_drive_pid(-3, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_turn_pid(45, TURN_SPEED);
  // chassis.wait_drive();
  // chassis.set_drive_pid(15, 45, true);
  // chassis.wait_drive();
  // wait(400);
  // chassis.set_turn_pid(-30, TURN_SPEED);
  // chassis.wait_drive();
  // wait(400);
  // shoot(550, 3);
}

void RedRight() {
  spinRollers();
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, false);
  chassis.wait_drive();  
  stopIntake();
  chassis.set_drive_pid(2, DRIVE_SPEED, true);
  chassis.wait_drive();
  
}

void BlueLeft() {  
  set_flywheel_speed(500);
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  stopIntake();
  chassis.set_drive_pid(12, DRIVE_SPEED, true);
  chassis.wait_until(9);
  spinIntake();
  chassis.wait_drive();
  chassis.set_turn_pid(-20, TURN_SPEED);
  chassis.wait_drive();
  tripleShot();
  stopIntake();
  set_flywheel_speed(0);
  wait(300);
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(36, DRIVE_SPEED, true);
  chassis.wait_drive();
  set_flywheel_speed(500);
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(12, DRIVE_SPEED, true);
  chassis.wait_drive();
  tripleShot();
}

void BlueRight() {

}

void RedSoloWP() {
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  stopIntake();
  chassis.set_drive_pid(12, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  spinIntake();
  set_flywheel_speed(500);
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-40, TURN_SPEED);
  chassis.wait_drive();
  tripleShot();  
  wait(200);
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(48, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED);
  chassis.wait_drive();
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  stopIntake();
}

void BlueSoloWP() {  
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  stopIntake();
  chassis.set_drive_pid(12, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();
  //startflywheel
  chassis.set_turn_pid(-40, TURN_SPEED);
  chassis.wait_drive();
  tripleShot();  
  wait(200);
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(48, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED);
  chassis.wait_drive();
  spinRollers();
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  stopIntake();
}

// void skills() {
//   spinRollers();
//   chassis.set_drive_pid(-3, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   stopIntake();
//   chassis.set_drive_pid(2, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-45, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(5, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   spinRollers();
//   chassis.set_turn_pid(90, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(-3, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   stopIntake();
//   chassis.set_drive_pid(2, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(0, TURN_SPEED);
//   spinIntake();
//   chassis.set_drive_pid(36, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   tripleShot();
//   chassis.set_drive_pid(-32, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(45, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(36, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-45, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(12, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   tripleShot();
//   chassis.set_drive_pid(-12, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-135, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(-36, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-180, TURN_SPEED);
//   spinRollers();
//   chassis.set_drive_pid(-3, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   stopIntake();
//   chassis.set_drive_pid(2, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-225, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(5, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   spinRollers();
//   chassis.set_turn_pid(-270, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(-3, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   stopIntake();
//   chassis.set_drive_pid(2, DRIVE_SPEED, true);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-180, TURN_SPEED);
//   chassis.wait_drive();
//   chassis.set_drive_pid(-3, DRIVE_SPEED, false);
//   chassis.wait_drive();
//   chassis.set_turn_pid(-135, TURN_SPEED);
//   chassis.wait_drive();
//   endgame();
// }

void skills() {
  spinRollers();
  
  chassis.set_drive_pid(-3, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(5, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  endgame();
}

