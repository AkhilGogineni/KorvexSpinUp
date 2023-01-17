#include "main.h"


/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// r
/////


const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED  = 100;
const int SWING_SPEED = 90;



///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants() {
  chassis.set_slew_min_power(60, 60);
  chassis.set_slew_distance(9, 9);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.8, 0.3, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.8, 0.3, 5, 0);
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
   chassis.set_swing_pid(ez::LEFT_SWING, 55, TURN_SPEED);
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
  set_flywheel_speed(3600); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(6, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(1200);
  chassis.set_turn_pid(-6.5, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(600);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(800,450);
  wait(1500);
  intake.moveRelative(1000,450);
  wait(400);
  wait(500);
  stopShoot();
  set_flywheel_speed(3450);
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  spinIntake();
  chassis.set_turn_pid(50, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(58, 80, true);
  chassis.wait_until(30);
  wait(400);
  chassis.set_max_speed(30);
  chassis.wait_drive();
  chassis.set_drive_pid(-6, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(500);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(875,450);
  wait(1500);
  intake.moveRelative(800,450);
  wait(1500);
  intake.moveRelative(1000,450);
  wait(400);
  wait(300);
  stopShoot();
}

void RedRight() {
  set_flywheel_speed(3590);
  spinIntake();
  chassis.set_drive_pid(15, 60, false);
  chassis.wait_drive();
  chassis.set_turn_pid(20, TURN_SPEED);
  chassis.wait_drive();
  wait(1200);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(875,450);
  wait(1500);
  intake.moveRelative(1000,600);
  wait(400);
  wait(300);
  stopShoot();
  wait(200);
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-20, DRIVE_SPEED);
  chassis.wait_drive();
  wait(200);
  chassis.set_swing_pid(ez::RIGHT_SWING, -45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-2, 60, false);
  chassis.wait_drive();
  chassis.set_swing_pid(ez::RIGHT_SWING, 0, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-2, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
}
void BlueLeft() {  
  set_flywheel_speed(3600); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(6, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(1200);
  chassis.set_turn_pid(-6.5, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(600);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(800,450);
  wait(1500);
  intake.moveRelative(1000,450);
  wait(400);
  wait(500);
  stopShoot();
  set_flywheel_speed(3450);
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  spinIntake();
  chassis.set_turn_pid(50, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(58, 80, true);
  chassis.wait_until(30);
  wait(400);
  chassis.set_max_speed(30);
  chassis.wait_drive();
  chassis.set_drive_pid(-6, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(500);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(875,450);
  wait(1500);
  intake.moveRelative(800,450);
  wait(1500);
  intake.moveRelative(1000,450);
  wait(400);
  wait(300);
  stopShoot();
}

void BlueRight() {
  set_flywheel_speed(3590);
  spinIntake();
  chassis.set_drive_pid(15, 60, false);
  chassis.wait_drive();
  chassis.set_turn_pid(20, TURN_SPEED);
  chassis.wait_drive();
  wait(1200);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(875,450);
  wait(1500);
  intake.moveRelative(1000,600);
  wait(400);
  wait(300);
  stopShoot();
  wait(200);
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-20, DRIVE_SPEED);
  chassis.wait_drive();
  wait(200);
  chassis.set_swing_pid(ez::RIGHT_SWING, -45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-2, 60, false);
  chassis.wait_drive();
  chassis.set_swing_pid(ez::RIGHT_SWING, 0, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-2, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
}

void RedSoloWP() {
  set_flywheel_speed(3410); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(3, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(54, DRIVE_SPEED, true);
  chassis.wait_until(30);
  chassis.set_max_speed(40);
  chassis.wait_drive();
  chassis.set_drive_pid(-2, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(500);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(200);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(825,450);
  wait(1500);
  intake.moveRelative(1000,600);
  wait(400);
  wait(300);
  stopShoot();
  wait(200);
  chassis.set_drive_pid(-4, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(79, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(180, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  
  chassis.set_drive_pid(-5, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-4.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
}

void BlueSoloWP() {  
  set_flywheel_speed(3410); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(3, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(54, DRIVE_SPEED, true);
  chassis.wait_until(30);
  chassis.set_max_speed(40);
  chassis.wait_drive();
  chassis.set_drive_pid(-2, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(500);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(200);
  indexer.set_value(true);
  intake.moveRelative(900,450);
  wait(1500);
  intake.moveRelative(825,450);
  wait(1500);
  intake.moveRelative(1000,600);
  wait(400);
  wait(300);
  stopShoot();
  wait(200);
  chassis.set_drive_pid(-4, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(79, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(180, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  
  chassis.set_drive_pid(-5, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-4.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
}



void skills() { 
  set_flywheel_speed(2900); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(300);
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(14, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  spinRollers(); //rollers
  chassis.set_drive_pid(-14, DRIVE_SPEED, false);
  chassis.wait_drive();
  chassis.set_drive_pid(-4, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(300);
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(63, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  chassis.set_turn_pid(9, TURN_SPEED);
  chassis.wait_drive();
  indexer.set_value(true);
  intake.moveVelocity(450);
  wait(1400);
  stopShoot();
  wait(300);
  spinIntake();
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-10, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(135, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(25, 90, true);
  chassis.wait_drive();
  wait(300);
  set_flywheel_speed(3150); //start flywheel
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(45, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_drive_pid(-14, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-46, TURN_SPEED);
  chassis.wait_drive();
  wait(500);
  indexer.set_value(true);
  intake.moveRelative(875,450);
  wait(700);
  intake.moveRelative(875,450);
  wait(700);
  intake.moveRelative(900,450);
  wait(700);
  stopShoot();
  spinIntake();
  set_flywheel_speed(2900); 
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(48, 80, true);
  chassis.wait_until(30);
  chassis.set_max_speed(35);
  chassis.wait_drive();
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(10, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(44, DRIVE_SPEED, true);
  chassis.wait_drive();
  stopIntake();
  chassis.set_turn_pid(-95, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  intake.moveVelocity(450);
  shoot();
  wait(1400);
  stopShoot();
  wait(200);
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(-54, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-180, TURN_SPEED);
  chassis.wait_drive();
  spinRollers(); //rollers
  chassis.set_drive_pid(-1.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(200);
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-225, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(14, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  spinRollers(); //rollers
  chassis.set_drive_pid(-14, DRIVE_SPEED, false);
  chassis.wait_drive();
  chassis.set_drive_pid(-4, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(200);
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end

  //excess
  // chassis.set_turn_pid(-135, TURN_SPEED);
  // chassis.wait_drive();
  // set_flywheel_speed(3200); //start flywheel
  // chassis.set_drive_pid(46, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_drive_pid(-6, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_turn_pid(-225, TURN_SPEED);
  // chassis.wait_drive();
  // wait(500);
  // indexer.set_value(true);
  // intake.moveRelative(900,450);
  // wait(200);
  // intake.moveRelative(900,450);
  // wait(200);
  // intake.moveRelative(900,600);
  // wait(400);
  // stopShoot();
  // chassis.set_turn_pid(-135, TURN_SPEED);
  // chassis.wait_drive();
  // chassis.set_drive_pid(54, DRIVE_SPEED, true);
  // chassis.wait_until(30);
  // chassis.set_max_speed(40);
  // chassis.wait_drive();
  // chassis.set_turn_pid(-180, TURN_SPEED);
  // chassis.wait_drive();
  // chassis.set_drive_pid(15, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_turn_pid(-270, TURN_SPEED); //turn to disk
  // chassis.wait_drive();
  // chassis.set_drive_pid(48, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_turn_pid(-279, TURN_SPEED); //turn to disk
  // chassis.wait_drive();
  // intake.moveVelocity(450);
  // wait(1400);
  // stopShoot();
  // wait(200);
  // chassis.set_turn_pid(-270, TURN_SPEED); //turn to disk
  // chassis.wait_drive();
  // chassis.set_drive_pid(-68, DRIVE_SPEED, true);
  // chassis.wait_drive();
  // chassis.set_turn_pid(-315, TURN_SPEED);
  // chassis.wait_drive();
  // endgame();
}


