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
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.8, 0.7, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.8, 0.7, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void skills_constants() {
  chassis.set_slew_min_power(100, 100);
  chassis.set_slew_distance(5, 5);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.17, 0, 1, 0.3);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.17, 0, 1, 0.3);
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
   chassis.set_drive_pid(24, DRIVE_SPEED);
   chassis.wait_drive();
   wait(300);
   chassis.set_drive_pid(-24, DRIVE_SPEED);
   chassis.wait_drive();
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
  chassis.set_drive_pid(-2.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(6, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(300);
  chassis.set_turn_pid(-7.25, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(400);
  indexer.set_value(true);
  intake.moveRelative(925,450);
  wait(1300);
  intake.moveRelative(875,450);
  wait(1300);
  intake.moveRelative(1000,450);
  wait(400);
  wait(300);
  stopShoot();
  set_flywheel_speed(3420);
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  spinIntake();
  chassis.set_turn_pid(50, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(60, 80, true);
  chassis.wait_until(30);
  wait(250);
  chassis.set_max_speed(30);
  chassis.wait_drive();
  chassis.set_drive_pid(-8, 127);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(100);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(850,450);
  wait(600);
  intake.moveRelative(800,450);
  wait(900);
  intake.moveRelative(1000,450);
  wait(400);
  wait(100);
  stopShoot();
}

void RedRight() {
  set_flywheel_speed(3530);
  spinIntake();
  chassis.set_drive_pid(15, 60, false);
  chassis.wait_drive();
  chassis.set_turn_pid(20, TURN_SPEED);
  chassis.wait_drive();
  wait(400);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  spinIntake();
  wait(200);
  set_flywheel_speed(3470);
  chassis.set_drive_pid(11, 80);
  chassis.wait_drive();
  wait(200);
  chassis.set_drive_pid(-11, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(40, 80);
  chassis.wait_drive();
  
  chassis.set_turn_pid(43, TURN_SPEED);
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-62, DRIVE_SPEED);
  chassis.wait_drive();
  wait(200);
  chassis.set_swing_pid(ez::RIGHT_SWING, -45, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-4, 100, false);
  chassis.wait_drive();
  chassis.set_swing_pid(ez::RIGHT_SWING, 0, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-2, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end


  // set_flywheel_speed(3400);
  // spinIntake();
  // chassis.set_drive_pid(44, DRIVE_SPEED, false);
  // chassis.wait_drive();
  // chassis.set_turn_pid(38, TURN_SPEED);
  // chassis.wait_drive();
  // chassis.set_drive_pid(9, 60, false);
  // chassis.wait_drive();
  // wait(200);
  // indexer.set_value(true);
  // wait(200);
  // intake.moveRelative(875,450);
  // wait(500);
  // intake.moveRelative(800,450);
  // wait(500);
  // intake.moveRelative(1000,450);
  // wait(400);
  // wait(300);
  // stopShoot();
  // chassis.set_swing_pid(ez::RIGHT_SWING, 135, TURN_SPEED);
  // // chassis.wait_drive();
  
}
void BlueLeft() {  
  set_flywheel_speed(3600); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-2.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(-45, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(6, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(300);
  chassis.set_turn_pid(-7.25, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(400);
  indexer.set_value(true);
  intake.moveRelative(925,450);
  wait(1300);
  intake.moveRelative(875,450);
  wait(1300);
  intake.moveRelative(1000,450);
  wait(400);
  wait(300);
  stopShoot();
  set_flywheel_speed(3420);
  chassis.set_drive_pid(-5, DRIVE_SPEED, true);
  chassis.wait_drive();
  spinIntake();
  chassis.set_turn_pid(50, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(60, 80, true);
  chassis.wait_until(30);
  wait(250);
  chassis.set_max_speed(30);
  chassis.wait_drive();
  chassis.set_drive_pid(-8, 127);
  chassis.wait_drive();
  chassis.set_turn_pid(-36.5, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(100);
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(850,450);
  wait(600);
  intake.moveRelative(800,450);
  wait(900);
  intake.moveRelative(1000,450);
  wait(400);
  wait(100);
  stopShoot();
}

void BlueRight() {
  set_flywheel_speed(3530);
  spinIntake();
  chassis.set_drive_pid(15, 60, false);
  chassis.wait_drive();
  chassis.set_turn_pid(20, TURN_SPEED);
  chassis.wait_drive();
  wait(400);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  spinIntake();
  wait(200);
  set_flywheel_speed(3470);
  chassis.set_drive_pid(11, 80);
  chassis.wait_drive();
  wait(200);
  chassis.set_drive_pid(-11, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(40, 80);
  chassis.wait_drive();
  
  chassis.set_turn_pid(43, TURN_SPEED);
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-62, DRIVE_SPEED);
  chassis.wait_drive();
  wait(200);
  chassis.set_swing_pid(ez::RIGHT_SWING, -45, DRIVE_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-4, 100, false);
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
  set_flywheel_speed(3370); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-2.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(50, 100, true);
  chassis.wait_until(30);
  chassis.set_max_speed(30);
  chassis.wait_drive();

  chassis.set_turn_pid(-34, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  wait(200);

  set_flywheel_speed(3600);
  chassis.set_drive_pid(-2, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(78, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(180, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  
  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-8.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();

  chassis.set_drive_pid(10, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(300);
  chassis.set_turn_pid(273, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(100);
  indexer.set_value(true);
  intake.moveRelative(750,450);
  wait(1000);
  intake.moveRelative(750,450);
  wait(1000);
  intake.moveRelative(1000,450);
  wait(300);
  stopShoot();
}

void BlueSoloWP() {  
  set_flywheel_speed(3370); //start flywheel
  spinRollers(); //rollers
  chassis.set_drive_pid(-2.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();
  chassis.set_drive_pid(4, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  chassis.set_drive_pid(50, 100, true);
  chassis.wait_until(30);
  chassis.set_max_speed(30);
  chassis.wait_drive();

  chassis.set_turn_pid(-34, TURN_SPEED); //turn to goal + shoot
  chassis.wait_drive();
  wait(50);
  indexer.set_value(true);
  wait(200);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(750,450);
  wait(800);
  intake.moveRelative(1000,450);
  wait(500);
  stopShoot();
  wait(200);

  set_flywheel_speed(3600);
  chassis.set_drive_pid(-2, DRIVE_SPEED, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  spinIntake();
  chassis.set_drive_pid(78, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_turn_pid(180, TURN_SPEED); //turn to stack of disks
  chassis.wait_drive();
  
  chassis.set_drive_pid(-10, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(270, TURN_SPEED);
  chassis.wait_drive();

  spinRollers(); //rollers
  chassis.set_drive_pid(-8.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  stopIntake();

  chassis.set_drive_pid(10, DRIVE_SPEED, true);
  chassis.wait_drive();
  wait(300);
  chassis.set_turn_pid(273, TURN_SPEED); //turn to high goal
  chassis.wait_drive();
  wait(100);
  indexer.set_value(true);
  intake.moveRelative(750,450);
  wait(1000);
  intake.moveRelative(750,450);
  wait(1000);
  intake.moveRelative(1000,450);
  wait(300);
  stopShoot();
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
  chassis.set_drive_pid(-16.5, DRIVE_SPEED, false);
  chassis.wait_drive();
  wait(300);
  stopIntake();
  chassis.set_drive_pid(4.5, DRIVE_SPEED, true);
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
  spinIntake(); //first volley
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-10, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(135, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(25, 90, true);
  chassis.wait_drive();
  wait(300);
  set_flywheel_speed(3220); //start flywheel
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(45, 100, true);
  chassis.wait_drive();
  chassis.set_drive_pid(-14, 60, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-40, TURN_SPEED);
  chassis.wait_drive();
  wait(500);
  indexer.set_value(true);
  intake.moveRelative(800,450);
  wait(700);
  intake.moveRelative(800,450);
  wait(700);
  intake.moveRelative(825,450);
  wait(700); //second volley
  stopShoot();
  spinIntake();
  set_flywheel_speed(2900); 
  chassis.set_drive_pid(-2, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(48, 80, true);
  chassis.wait_until(30);
  chassis.set_max_speed(35);
  chassis.wait_drive();
  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(15, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(41.5, DRIVE_SPEED, true);
  chassis.wait_drive();
  stopIntake();
  chassis.set_turn_pid(-95, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  indexer.set_value(true);
  intake.moveVelocity(450);
  wait(1400);
  stopShoot();
  wait(300); //thrid volley
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(-30, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-80, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-24, 100, true);
  chassis.wait_drive();
  // chassis.set_swing_pid(ez::LEFT_SWING, -178, SWING_SPEED);
  // chassis.wait_drive();
  chassis.set_turn_pid(-180, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  spinRollers(); //rollers
  chassis.set_drive_pid(-9.5, 80, false);
  chassis.wait_drive();
  wait(200);
  stopIntake();
  chassis.set_drive_pid(7.5, DRIVE_SPEED, true);
  chassis.wait_drive(); //rollers end
  spinIntake();

  
  chassis.set_turn_pid(-225, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  chassis.set_drive_pid(28, 60, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-90, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  spinRollers(); //rollers


  chassis.set_drive_pid(-8.25, 80, false);
  chassis.wait_drive();
  wait(300);
  stopIntake();


  chassis.set_drive_pid(4.5, 60, true);
  chassis.wait_drive(); //rollers end
  spinIntake();
  chassis.set_drive_pid(21, 60, true);
  chassis.wait_drive(); //rollers end
  wait(200);
  chassis.set_drive_pid(-19.5, 60, true);
  chassis.wait_drive(); //rollers end

  chassis.set_turn_pid(-180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(54, 100, true);
  chassis.wait_drive(); //rollers end
  chassis.set_turn_pid(-166, TURN_SPEED);
  chassis.wait_drive();
  indexer.set_value(true);
  intake.moveVelocity(450);
  wait(1400);
  stopShoot();
  wait(200);
  spinIntake(); //fourth volley

  chassis.set_turn_pid(-175, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(-11, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(27, 90, true);
  chassis.wait_drive();
  wait(300);
  set_flywheel_speed(3270); //start flywheel
  chassis.set_turn_pid(-135, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(45, 100, true);
  chassis.wait_drive();
  chassis.set_drive_pid(-14, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-219, TURN_SPEED);
  chassis.wait_drive();
  wait(500);
  indexer.set_value(true);
  intake.moveRelative(875,450);
  wait(600);
  intake.moveRelative(875,450);
  wait(600);
  intake.moveRelative(900,450);
  wait(600); //fifth volley
  stopShoot();
  spinIntake();


  set_flywheel_speed(3600); 
  chassis.set_drive_pid(-4, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-135, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(48, 80, true);
  chassis.wait_until(30);
  chassis.set_max_speed(35);
  chassis.wait_drive();
  chassis.set_turn_pid(-180, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(11, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-270, TURN_SPEED); //turn to disk
  chassis.wait_drive();
  indexer.set_value(true);
  chassis.set_drive_pid(-36, 100, true);
  chassis.wait_drive();
  chassis.set_turn_pid(-315, TURN_SPEED);
  chassis.wait_drive();
  chassis.set_drive_pid(4, 100, true);
  chassis.wait_drive();
  endgame();

}


