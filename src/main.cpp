#include "main.h"
#include "lemlib/api.hpp"
#include <fstream>

using namespace std;



// // drivetrain
// lemlib::Drivetrain_t drivetrain {
// 	&leftMotors,
// 	&rightMotors,
// 	10,
// 	3.25,
// 	360,
// };

// // lateral motion controller
// lemlib::ChassisController_t lateralController {
// 	10,
// 	30,
// 	1,
// 	100,
// 	3,
// 	500,
// 	1
// };

// // angular motion controller
// lemlib::ChassisController_t angularController {
// 	2,
// 	10,
// 	1,
// 	100,
// 	3,
// 	500,
// 	1
// };

// // sensors for odometry
// lemlib::OdomSensors_t sensors {
// 	nullptr,
// 	nullptr,
// 	nullptr,
// 	nullptr,
// 	&imu
// };


// lemlib::Chassis lemChassis(drivetrain, lateralController, angularController, sensors);

enum class autonStates { // the possible auton selections
	off,
	RedLeft,
	RedRight,
	BlueLeft,
	BlueRight,
	RedSoloWP,
	BlueSoloWP,
	Skills,
	test
};

bool driverDisabled = false;

autonStates autonSelection = autonStates::off;

static lv_res_t RedLeftBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::RedLeft;
	std::cout << pros::millis() << "RedLeft" << std::endl;
	return LV_RES_OK;
}

static lv_res_t RedRightBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::RedRight;
	std::cout << pros::millis() << "RedRight" << std::endl;
	return LV_RES_OK;
}

static lv_res_t BlueLeftBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::BlueLeft;
	std::cout << pros::millis() << "BlueLeft" << std::endl;
	return LV_RES_OK;
}

static lv_res_t BlueRightBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::BlueRight;
	std::cout << pros::millis() << "BlueRight" << std::endl;
	return LV_RES_OK;
}

static lv_res_t RedSoloWPBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::RedSoloWP;
	std::cout << pros::millis() << "RedSoloWP" << std::endl;
	return LV_RES_OK;
}

static lv_res_t BlueSoloWPBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::BlueSoloWP;
	std::cout << pros::millis() << "BlueSoloWP" << std::endl;
	return LV_RES_OK;
}

static lv_res_t SkillsBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::Skills;
	std::cout << pros::millis() << "Skills" << std::endl;
	return LV_RES_OK;
}

static lv_res_t ResetBtnAction(lv_obj_t *btn) {
	imu.reset();

	ldf.tare_position();
	ldm.tare_position();
	ldb.tare_position();
	rdf.tare_position();
	rdm.tare_position();
	rdb.tare_position();

	while (imu.is_calibrating() and pros::millis() < 5000)
	{
		pros::delay(10);
	}
	if (pros::millis() < 5000) std::cout << pros::millis() << ": finished calibrating!" << std::endl;
	return LV_RES_OK;
}

static lv_res_t noAutonBtnAction(lv_obj_t *btn) {
	autonSelection = autonStates::off;
	std::cout << pros::millis() << "None" << std::endl;
	return LV_RES_OK;
}

// Chassis constructor
Drive chassis (
  // Left Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  {-19, 16, -18}


  // Right Chassis Ports (negative port will reverse it!)
  //   the first port is the sensored port (when trackers are not used!)
  ,{12, -17, 14}

  // IMU Port

  ,20

  // Wheel Diameter (Remember, 4" wheels are actually 4.125!)
  //    (or tracking wheel diameter)
  ,2.6

  // Cartridge RPM
  //   (or tick per rotation if using tracking wheels)
  ,600

  // External Gear Ratio (MUST BE DECIMAL)
  //    (or gear ratio of tracking wheel)
  // eg. if your drive is 84:36 where the 36t is powered, your RATIO would be 2.333.
  // eg. if your drive is 36:60 where the 60t is powered, your RATIO would be 0.6.
  ,1.666

  // Uncomment if using tracking wheels
  /*
  Left Tracking Wheel Ports (negative port will reverse it!)
  ,{1, 2} // 3 wire encoder


  Right Tracking Wheel Ports (negative port will reverse it!)
  ,{-3, -4} // 3 wire encoder
 
  */

  // Uncomment if tracking wheels are plugged into a 3 wire expander
  // 3 Wire Port Expander Smart Port
  // ,1
);


void initialize() {
  	imu.reset();
	//petentiometer.tare_rotation();
	std::cout << pros::millis() << ": calibrating imu..." << std::endl;

	lv_theme_t *th = lv_theme_alien_init(360, NULL); //Set a HUE value and keep font default RED
	lv_theme_set_current(th);

	// create a tab view object
	std::cout << pros::millis() << ": creating gui..." << std::endl;
	lv_obj_t *tabview = lv_tabview_create(lv_scr_act(), NULL);

	//button layout
	lv_obj_t *RedTab = lv_tabview_add_tab(tabview, "Red");
	lv_obj_t *BlueTab = lv_tabview_add_tab(tabview, "Blue");
	lv_obj_t *SkillsTab = lv_tabview_add_tab(tabview, "Skills");
	lv_obj_t *OffTab = lv_tabview_add_tab(tabview, "Turn Off");
	

	

	// Red tab
	lv_obj_t *RedLeftBtn = lv_btn_create(RedTab, NULL);
	lv_obj_t *labelRedLeft = lv_label_create(RedLeftBtn, NULL);

	lv_obj_t *RedRightBtn = lv_btn_create(RedTab, NULL);
	lv_obj_t *labelRedRight = lv_label_create(RedRightBtn, NULL);

	lv_obj_t *RedSoloWPBtn = lv_btn_create(RedTab, NULL);
	lv_obj_t *labelRedSolo = lv_label_create(RedSoloWPBtn, NULL);

	lv_label_set_text(labelRedLeft, "RedLeft");
	lv_btn_set_action(RedLeftBtn, LV_BTN_ACTION_CLICK, RedLeftBtnAction);
	lv_obj_set_size(RedLeftBtn, 150, 50);
	lv_btnm_set_toggle(RedLeftBtn, true, 1);
	lv_obj_set_pos(RedLeftBtn, 0, 0);
	lv_obj_align(RedLeftBtn, NULL, LV_ALIGN_CENTER, -150, -5);

	lv_label_set_text(labelRedRight, "RedRight");
	lv_btn_set_action(RedRightBtn, LV_BTN_ACTION_CLICK, RedRightBtnAction);
	lv_obj_set_size(RedRightBtn, 150, 50);
	lv_btnm_set_toggle(RedRightBtn, true, 1);
	lv_obj_set_pos(RedRightBtn, 0, 0);
	lv_obj_align(RedRightBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(labelRedSolo, "RedSoloWP");
	lv_btn_set_action(RedSoloWPBtn, LV_BTN_ACTION_CLICK, RedSoloWPBtnAction);
	lv_obj_set_size(RedSoloWPBtn, 150, 50);
	lv_btnm_set_toggle(RedSoloWPBtn, true, 1);
	lv_obj_set_pos(RedSoloWPBtn, 0, 0);
	lv_obj_align(RedSoloWPBtn, NULL, LV_ALIGN_CENTER, 150, 12.5);
	

	// Blue tab
	lv_obj_t *BlueLeftBtn = lv_btn_create(BlueTab, NULL);
	lv_obj_t *labelBlueLeft = lv_label_create(BlueLeftBtn, NULL);

	lv_obj_t *BlueRightBtn = lv_btn_create(BlueTab, NULL);
	lv_obj_t *labelBlueRight = lv_label_create(BlueRightBtn, NULL);

	lv_obj_t *BlueSoloWPBtn = lv_btn_create(BlueTab, NULL);
	lv_obj_t *labelBlueSolo = lv_label_create(BlueSoloWPBtn, NULL);

	lv_label_set_text(labelBlueLeft, "BlueLeft");
	lv_btn_set_action(BlueLeftBtn, LV_BTN_ACTION_CLICK, BlueLeftBtnAction);
	lv_obj_set_size(BlueLeftBtn, 150, 50);
	lv_btnm_set_toggle(BlueLeftBtn, true, 1);
	lv_obj_set_pos(BlueLeftBtn, 0, 0);
	lv_obj_align(BlueLeftBtn, NULL, LV_ALIGN_CENTER, -150, -5);

	lv_label_set_text(labelBlueRight, "BlueRight");
	lv_btn_set_action(BlueRightBtn, LV_BTN_ACTION_CLICK, BlueRightBtnAction);
	lv_obj_set_size(BlueRightBtn, 150, 50);
	lv_btnm_set_toggle(BlueRightBtn, true, 1);
	lv_obj_set_pos(BlueRightBtn, 0, 0);
	lv_obj_align(BlueRightBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	lv_label_set_text(labelBlueSolo, "BlueSoloWP");
	lv_btn_set_action(BlueSoloWPBtn, LV_BTN_ACTION_CLICK, BlueSoloWPBtnAction);
	lv_obj_set_size(BlueSoloWPBtn, 150, 50);
	lv_btnm_set_toggle(BlueSoloWPBtn, true, 1);
	lv_obj_set_pos(BlueSoloWPBtn, 0, 0);
	lv_obj_align(BlueSoloWPBtn, NULL, LV_ALIGN_CENTER, 150, 12.5);
	

	//DiagonalDouble tab
	lv_obj_t *OffBtn = lv_btn_create(OffTab, NULL);
	lv_obj_t *Offlabel = lv_label_create(OffBtn, NULL);
	lv_label_set_text(Offlabel, "Turn Off");
	lv_btn_set_action(OffBtn, LV_BTN_ACTION_CLICK, noAutonBtnAction);
	lv_obj_set_size(OffBtn, 450, 50);
	lv_btnm_set_toggle(OffBtn, true, 1);
	lv_obj_set_pos(OffBtn, 0, 100);
	lv_obj_align(OffBtn, NULL, LV_ALIGN_CENTER, 0, 0);


	// skills tab
	lv_obj_t *SkillsBtn = lv_btn_create(SkillsTab, NULL);
	lv_obj_t *skillslabel = lv_label_create(SkillsBtn, NULL);

	lv_label_set_text(skillslabel, "Skills");
	lv_btn_set_action(SkillsBtn, LV_BTN_ACTION_CLICK, SkillsBtnAction);
	lv_obj_set_size(SkillsBtn, 450, 50);
	lv_btnm_set_toggle(SkillsBtn, true, 1);
	lv_obj_set_pos(SkillsBtn, 0, 100);
	lv_obj_align(SkillsBtn, NULL, LV_ALIGN_CENTER, 0, 0);

	//reset btn
	lv_obj_t *ResetBtn = lv_btn_create(tabview, NULL);
	lv_obj_t *ResetLabel = lv_label_create(ResetBtn, NULL);

	lv_label_set_text(ResetLabel, "Reset");
	lv_btn_set_action(ResetBtn, LV_BTN_ACTION_CLICK, ResetBtnAction);
	lv_obj_set_size(ResetBtn, 250, 50);
	lv_btnm_set_toggle(ResetBtn, true, 1);
	lv_obj_set_pos(ResetBtn, 0, 100);
	lv_obj_align(ResetBtn, NULL, LV_ALIGN_CENTER, 0, 50);

	std::cout << pros::millis() << ": generating paths..." << std::endl;
	//generatePaths();
	std::cout << pros::millis() << ": finished generating paths..." << std::endl;
	
	std::cout << pros::millis() << ": finished creating gui!" << std::endl;
	while (imu.is_calibrating() and pros::millis() < 5000)
	{
		pros::delay(10);
	}
	if (pros::millis() < 5000) std::cout << pros::millis() << ": finished calibrating!" << std::endl;
	else std::cout << pros::millis() << ": calibration failed, moving on" << std::endl;

	std::cout << pros::millis() << ": flywheel: " << flywheel.get_temperature() << std::endl;
	std::cout << pros::millis() << ": intake: " << intake.getTemperature() << std::endl;
	std::cout << pros::millis() << ": chassisLF: " << ldf.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLM: " << ldm.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisLB: " << ldb.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRF: " << rdf.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRM: " << rdm.get_temperature() << std::endl;
	std::cout << pros::millis() << ": chassisRB: " << rdb.get_temperature() << std::endl;
  
  
	// Initialize chassis and auton selector
	default_constants();
	modified_exit_condition();
	skills_constants();

	pros::ADIDigitalOut indexer('A', true);
	pros::ADIDigitalOut endgame1('C', false);
	pros::ADIDigitalOut angler('B', false);
	chassis.initialize();
	//lemChassis.calibrate();
}

void disabled() {
	pros::ADIDigitalOut indexer('A', true);
	pros::ADIDigitalOut endgame1('C', false);
	pros::ADIDigitalOut angler('B', false);

}

void competition_initialize() {
  // . . .
}


pros::Task tflywheel(threadingFlywheel, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tchassis(threadingChassis, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tintake(threadingIntake, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tendgame(threadingEndgameMech, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tflywheelP(flywheelP, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");
pros::Task tindexing(threadingIndexing, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "");

void autonomous() {
	
	driverDisabled = true;
	if(driverDisabled) {
		tflywheel.suspend();
		tchassis.suspend();
		tintake.suspend();
		tendgame.suspend();
		tflywheelP.suspend();
		tindexing.suspend();
	}

	
	chassis.reset_pid_targets(); // Resets PID targets to 0
	chassis.reset_gyro(); // Reset gyro position to 0
	chassis.reset_drive_sensor(); // Reset drive sensors to 0

	setChassisBreakMode(pros::E_MOTOR_BRAKE_HOLD);

	intake.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);	
	flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	

	
	if(autonSelection == autonStates::off) {
			autonSelection = autonStates::RedSoloWP;
	}		
	switch(autonSelection) {
		case autonStates::RedLeft:
			RedLeft();
			break;
		case autonStates::RedRight:
			RedRight();
			break;
		case autonStates::BlueLeft:
			BlueLeft();
			break;
		case autonStates::BlueRight:
			BlueRight();
			break;
		case autonStates::RedSoloWP:
			RedSoloWP();
			break;
		case autonStates::BlueSoloWP:
			BlueSoloWP();
			break;
		case autonStates::Skills:
			skills();
			//drive_example();
			// turn_example();
			// swing_example();
			break;
		case autonStates::test:
			drive_example();
			//turn_example();
			//swing_example();
			break;
		default:
			break;
	}
}



void opcontrol() {

	
	flywheel.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	intake.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	chassis.set_drive_brake(MOTOR_BRAKE_COAST);
	driverDisabled = false;
	if(!driverDisabled) {
		tflywheel.resume();
		tchassis.resume();
		tintake.resume();
		tendgame.resume();
		tflywheelP.resume();
		tindexing.resume();


		std::cout << pros::millis() << ": lf: " << ldf.get_position() << std::endl;
		std::cout << pros::millis() << ": rf: " << rdf.get_position() << std::endl;
	} 
}