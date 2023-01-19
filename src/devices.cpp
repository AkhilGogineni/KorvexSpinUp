#include "main.h"


pros::Motor ldf(17, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ldm(8, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ldb(9, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdf(12, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdm(5, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdb(4, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);


// okapi::Motor ldf(17,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor ldm(8,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor ldb(9,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdf(12,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdm(5,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdb(4,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);


// okapi::MotorGroup leftDrive({ldf,ldm,ldb});
// okapi::MotorGroup rightDrive({rdf,rdm,rdb});

okapi::Motor intake(11,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
//okapi::Motor flywheel(1,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
pros::Motor flywheel(2, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Imu imu (10);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut indexer('A', false);
pros::ADIDigitalOut endgame1('B', false);
pros::ADIDigitalOut endgame2('D', false);
pros::ADIDigitalOut angler('C', false);
