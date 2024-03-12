#ifndef DRIVE_H
#define	DRIVE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "helper.h"

#define MOTOR_PWM_1 4
#define MOTOR_DIR_1 12

#define MOTOT_PWM_2 5
#define MOTOR_DIR_2 13

#define PWM_FREQUENCY 400

#define STEPS_PER_INCH 18.182
#define STEPS_PER_ROTATION 484

#define STEP_MODE 0.5

static int total_steps2 = 0;
static int total_steps3 = 0;
static int steps_needed = 0;
static int steps_needed2 = 0;
static int steps_needed3 = 0;
static int current_state = 0;

enum pinState {HIGH = 1, LOW = 0};
enum circle {CLOCKWISE = 1, COUNTERCLOCKWISE = 0};
enum direciotn {FORWARD = 0, REVERSE = 1};
enum leftRight {LEFT = 0, RIGHT = 1};


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
}

/**
 * Calculate the number of steps needed and turns that many steps
 * @param degrees The number of degrees to be turned
 * @param direction Clockwise or CounterClockwise
 */
void turnRobot(double degrees, int direction) {
    steps_needed = round((degrees/360.0)*STEPS_PER_ROTATION/STEP_MODE);
    steps_needed2 = steps_needed;
    steps_needed3 = steps_needed;
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
    steps_needed2 = steps_needed;
    steps_needed3 = steps_needed;
    driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, direction);
    driveMotor(MOTOT_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, direction);
}

/**
 * Drive the two motors at different speeds to correct when off the line
 * @param distance The readjustment distance
 * @param direction Forward or reverse
 * @param speed_modifier the speed modifier at which one motor is sped up and the other slowed down
 */
void driveDifferentialy (double distance, int direction, double speed_modifier) {
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    
    if (PWM_FREQUENCY/speed_modifier <= 200) {
        speed_modifier = 3;
    }
    
    if (direction == LEFT) {
        steps_needed2 = steps_needed/speed_modifier;
        steps_needed3 = steps_needed*speed_modifier;
        driveMotor(MOTOT_PWM_2, PWM_FREQUENCY*speed_modifier, MOTOR_DIR_2, REVERSE);
        driveMotor(MOTOR_PWM_1, PWM_FREQUENCY/speed_modifier, MOTOR_DIR_1, REVERSE);
    }
    else if (direction == RIGHT) {
        steps_needed2 = steps_needed*speed_modifier;
        steps_needed3 = steps_needed/speed_modifier;
        driveMotor(MOTOR_PWM_1, PWM_FREQUENCY*speed_modifier, MOTOR_DIR_1, REVERSE);
        driveMotor(MOTOT_PWM_2, PWM_FREQUENCY/speed_modifier, MOTOR_DIR_2, REVERSE);
    }
    
    
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
 * if so it stops the robot
 */
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void) {
    _OC2IF = 0; // Clear flag
    total_steps2++;
    if (total_steps2 > steps_needed2) {
        stopRobot();
        total_steps2 = 0;
    }
    
}

/**
 * This is the interrupt for OC3 which is pin 5
 * It counts steps and checks if we have hit the number of steps needed,
 * if so it stops the robot
 */
void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(void) {
    _OC3IF = 0; // Clear flag
    total_steps3++;
    if (total_steps3 > steps_needed3) {
        stopRobot();
        total_steps3 = 0;
    }
}


#endif	//DRIVE_H
