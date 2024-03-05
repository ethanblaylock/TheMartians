#ifndef DRIVE_H
#define	DRIVE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "helper.h"
#include "stdbool.h"

#define MOTOR_PWM_1 4
#define MOTOR_DIR_1 12

#define MOTOT_PWM_2 5
#define MOTOR_DIR_2 13

#define PWM_FREQUENCY 400

#define STEPS_PER_INCH 18.182
#define STEPS_PER_ROTATION 484

#define STEP_MODE 0.5

#define TOTAL_STATES 6

static int total_steps = 0;
static int steps_needed = 0;
static int current_state = 0;
static bool readjustment_complete = false; // Tracks wheter the motor has completed its steps

enum pinState {HIGH = 1, LOW = 0};
enum circle {CLOCKWISE = 1, COUNTERCLOCKWISE = 0};
enum direciotn {FORWARD = 0, REVERSE = 1};
enum state {ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5};

/**
 * Method to move to the next state 
 */
void incrementState() {
    if (current_state != TOTAL_STATES) {
        current_state++;
    }
}

/**
 * Configures the given PWM and sets the proper output pin to drive the motor
 * @param motorPWMPin Which PWM is used
 * @param frequency The PWM frequency
 * @param motorDirPin The pinNum for the direction
 * @param direction High or Low
 */
void driveMotor(int motorPWMPin, int frequency, int motorDirPin, int direction) {
    configurePWM(motorPWMPin, frequency, 0.5);
    setOutputPin(motorDirPin, direction);
    readjustment_complete = false;
}

/**
 * Calculate the number of steps needed and turns that many steps
 * @param degrees The number of degrees to be turned
 * @param direction Clockwise or CounterClockwise
 */
void turnRobot(double degrees, int direction) {
    steps_needed = round((degrees/360.0)*STEPS_PER_ROTATION/STEP_MODE);
    
    driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, direction);
    driveMotor(MOTOT_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, direction ^ 1);
}

/**
 * Calculate the steps needed to drive the robot and drives straight
 * @param distance The distance in inches to be traveled
 * @param direction Forward or Reverse
 */
void driveStraight (double distance, int direction) {
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    
    driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, direction);
    driveMotor(MOTOT_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, direction);
}

/**
 * Stops the robot by turning off the output compare modules
 */
void stopRobot() {
    OC2CON1bits.OCM = 0; 
    OC3CON1bits.OCM = 0; 
}

/**
 * This is the interrupt for OC2 which is pin 4
 * It counts steps and checks if we have hit the number of steps needed,
 * if so it moves to the next state
 */
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void) {
    _OC2IF = 0; // Clear flag
    total_steps++;
    if (total_steps > steps_needed) {
        incrementState();
        stopRobot();
        total_steps = 0;
        readjustment_complete = true;
    }
    
}

/*
void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(void) {
    _OC3IF = 0; // Clear flag
    total_steps3++;
    if (total_steps3 > steps_needed) {
        
        stopRobot();
        total_steps3 = 0;
    }
}
 */

#endif	//DRIVE_H
