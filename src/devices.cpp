#include "main.h"

okapi::Motor ldf(18,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor ldm(19,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor ldb(20,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rdf(6,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rdm(7,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor rdb(8,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);

okapi::MotorGroup leftDrive({ldf,ldm,ldb});
okapi::MotorGroup rightDrive({rdf,rdm,rdb});
//pros::Motor intake(1,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
//pros::Motor flywheel(7,pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

okapi::Motor intake(9,false,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);
okapi::Motor flywheel(1,true,okapi::AbstractMotor::gearset::blue,okapi::AbstractMotor::encoderUnits::counts);



//okapi::ADIEncoder left('E','F',true);
//okapi::ADIEncoder right('A','B',true);
//okapi::ADIEncoder middle('C','D',true);

pros::Imu imu (3);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::ADIDigitalOut indexer('A', true);
pros::ADIDigitalOut piston1('B', false);
pros::ADIDigitalOut piston2('C', false);
pros::ADIDigitalOut piston3('D', false);
pros::ADIDigitalOut piston4('E', false);

