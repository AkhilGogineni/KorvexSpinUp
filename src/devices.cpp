#include "main.h"


pros::Motor ldf(19, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ldm(16, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor ldb(18, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdf(12, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdm(17, pros::E_MOTOR_GEARSET_06, true , pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor rdb(14, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor temp(6, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

// pros::MotorGroup leftMotors ({ldf, ldm, ldb}); // left motor group
// pros::MotorGroup rightMotors ({rdf, rdm, rdb}); // right motor group

// okapi::Motor ldf(17,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor ldm(8,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor ldb(9,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdf(12,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdm(5,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
// okapi::Motor rdb(4,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);


// okapi::MotorGroup leftDrive({ldf,ldm,ldb});
// okapi::MotorGroup rightDrive({rdf,rdm,rdb});

okapi::Motor intake(10,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
//okapi::Motor flywheel(1,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
pros::Motor flywheel(9, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Imu imu (20);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut indexer('A', true);
pros::ADIDigitalOut endgame1('C', false);
pros::ADIDigitalOut endgame2('D', false);
pros::ADIDigitalOut angler('B', false);


