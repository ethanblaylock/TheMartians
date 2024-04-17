#ifndef DRIVE_H
#define	DRIVE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "helper.h"
#include <stdbool.h>

#define MOTOR_PWM_1 4 // Left
#define MOTOR_DIR_1 12

#define MOTOR_PWM_2 5 // Right
#define MOTOR_DIR_2 13

#define PWM_FREQUENCY 1000
#define CANYON_FREQUENCY 700
#define CANYON_TURN_FREQUENCY 300
#define TURN_PWM_FREQUENCY 600
#define COLLECT_FREQUENCY 500

#define MAX_PWM_FREQUENCY 2400
#define MIN_PWM_FREQUENCY 200

#define MAX_SPEED_MODIFIER 2

#define STEPS_PER_INCH 18.182
#define STEPS_PER_ROTATION 498

#define STEP_MODE 0.5

#define PWM_INCREMENT 1

static int total_steps2 = 0;
static int total_steps3 = 0;
static int steps_needed = 0;
static int steps_needed2 = 0;
static int steps_needed3 = 0;
static bool drive_completed = false;
static bool stripe_flag = false;
static bool high_to_low = true;
static int stripe_count = 0;
static int stripe_steps = 0;
static bool turn_flag = false;
static int straight_steps = 0;
static double motor_speed_modifier_high = 1.0;
static double motor_speed_modifier_low = 1.0;
static double motor_speed_modifier = 1.0;
static bool collect_flag = false;
static int end_detection = 0;
static bool service_flag = false;
static int end_steps = 0;
static bool end_flag = false;


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
    _OC3IE = 0;
    if (current_state == NAVIGATE_CANYON) {
        if (chosen_frequency > CANYON_TURN_FREQUENCY) {
            chosen_frequency = chosen_frequency - PWM_INCREMENT;
        }
        
    }
    if (current_state == COLLECT_SAMPLE || current_state == RETURN_SAMPLE || current_state == DATA_TRANSMISSION || current_state == SERVICE_EQUIPMENT) {
        if (chosen_frequency > COLLECT_FREQUENCY) {
            chosen_frequency = chosen_frequency - PWM_INCREMENT;
        }
    }
    steps_needed = round((degrees/360.0)*STEPS_PER_ROTATION/STEP_MODE);
    steps_needed2 = steps_needed;
    steps_needed3 = steps_needed;
    driveMotor(MOTOR_PWM_1, chosen_frequency, MOTOR_DIR_1, direction);
    driveMotor(MOTOR_PWM_2, chosen_frequency, MOTOR_DIR_2, direction ^ 1);
}

/**
 * Calculate the steps needed to drive the robot and drives straight
 * @param distance The distance in inches to be traveled
 * @param direction Forward or Reverse
 */
void driveStraight (double distance, int direction) {
    _OC3IE = 0;
    if (chosen_frequency < PWM_FREQUENCY && current_state != NAVIGATE_CANYON) {
        chosen_frequency = chosen_frequency + PWM_INCREMENT;
    }
    if (current_state == NAVIGATE_CANYON) {
        if (slow_down_flag) {
            if (chosen_frequency > CANYON_TURN_FREQUENCY) {
                chosen_frequency = chosen_frequency - 4*PWM_INCREMENT;
            }
        } else {
            if (chosen_frequency < CANYON_FREQUENCY) {
                chosen_frequency = chosen_frequency + PWM_INCREMENT;
            } 
            else if (chosen_frequency > CANYON_FREQUENCY) {
                chosen_frequency = chosen_frequency - PWM_INCREMENT;
            }
        }
    }
    if (current_state == COLLECT_SAMPLE || current_state == RETURN_SAMPLE || current_state == DATA_TRANSMISSION || current_state == SERVICE_EQUIPMENT) {
        if (!collect_flag) {
            if (chosen_frequency > COLLECT_FREQUENCY) {
                chosen_frequency = chosen_frequency - 4*PWM_INCREMENT;
            }
            else if (chosen_frequency < COLLECT_FREQUENCY) {
                chosen_frequency = chosen_frequency + 2*PWM_INCREMENT;
            }
        } else {
            if (chosen_frequency > CANYON_TURN_FREQUENCY) {
                chosen_frequency = chosen_frequency - 12*PWM_INCREMENT;
            }
        }
    }
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    steps_needed2 = steps_needed;
    steps_needed3 = steps_needed;
    driveMotor(MOTOR_PWM_1, chosen_frequency, MOTOR_DIR_1, direction);
    driveMotor(MOTOR_PWM_2, chosen_frequency, MOTOR_DIR_2, direction);
}

void driveSlow (double distance, int direction) {
    _OC3IE = 0;
    chosen_frequency = 400;
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    steps_needed2 = steps_needed;
    steps_needed3 = steps_needed;
    driveMotor(MOTOR_PWM_1, chosen_frequency, MOTOR_DIR_1, direction);
    driveMotor(MOTOR_PWM_2, chosen_frequency, MOTOR_DIR_2, direction);
}
/**
 * Drive the two motors at different speeds to correct when off the line
 * @param distance The readjustment distance
 * @param direction Forward or reverse
 * @param speed_modifier the speed modifier at which one motor is sped up and the other slowed down
 */
void driveDifferentialy (double distance, int direction, double speed_modifier, bool straight) {
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    if (straight) {
        if (chosen_frequency < PWM_FREQUENCY) {
            chosen_frequency = chosen_frequency + PWM_INCREMENT;
        }
    } else {
        if (chosen_frequency > TURN_PWM_FREQUENCY) {
        chosen_frequency = chosen_frequency - speed_modifier*PWM_INCREMENT;
    }
    }
    
    motor_speed_modifier = speed_modifier;
    if (_OC3IE == 0) {
       _OC3IE = 1; 
       _OC3IF = 0;
    }
    if (chosen_frequency/motor_speed_modifier_low < MIN_PWM_FREQUENCY) {
        motor_speed_modifier_low = chosen_frequency/MIN_PWM_FREQUENCY;
    }
    else if (chosen_frequency*motor_speed_modifier_high > MAX_PWM_FREQUENCY) {
        motor_speed_modifier_high = MAX_PWM_FREQUENCY/chosen_frequency;
    }
    
    if (direction == LEFT) {
        steps_needed2 = steps_needed/motor_speed_modifier_low;
        steps_needed3 = steps_needed*motor_speed_modifier_high;
        driveMotor(MOTOR_PWM_2, chosen_frequency*motor_speed_modifier_high, MOTOR_DIR_2, FORWARD);
        driveMotor(MOTOR_PWM_1, chosen_frequency/motor_speed_modifier_low, MOTOR_DIR_1, FORWARD);
    }
    else if (direction == RIGHT) {
        steps_needed2 = steps_needed*motor_speed_modifier_high;
        steps_needed3 = steps_needed/motor_speed_modifier_low;
        driveMotor(MOTOR_PWM_1, chosen_frequency*motor_speed_modifier_high, MOTOR_DIR_1, FORWARD);
        driveMotor(MOTOR_PWM_2, chosen_frequency/motor_speed_modifier_low, MOTOR_DIR_2, FORWARD);
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
        drive_completed = true;
        total_steps2 = 0;
    }
    if (stripe_flag) {
        stripe_steps++;
    }
    if (end_flag) {
        end_steps++;
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
        drive_completed = true;
        total_steps3 = 0;
    }
}

/*
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    _CNIF = 0;
    stripe_count++;
    if (!stripe_flag) {
        stripe_flag = true;
    }
}
*/
 
void _ISR _ADC1Interrupt(void) {
    _AD1IF = 0;
    if (ADC1BUF14 < 2048 && current_state == FOLLOW_LINE) {
        end_flag = true;
        end_steps = 0;
        end_detection++;
    } 
    if (ADC1BUF15 > 1200) {
        _AD1IE = 0;
        current_state = SERVICE_EQUIPMENT;
    }
    if (ADC1BUF4 < 2048) {
        if (high_to_low) {
            stripe_count++;
            high_to_low = false;
        }
        stripe_flag = true;
        stripe_steps = 0;
    }
    else {
        high_to_low = true;
    }
    
}

void shootGun(void) {
    /*
    double current_angle1 = 1500;
    double current_ir = 0;
    double max_ir = 0;
    double max_ir_angle = 0;
    CLKDIVbits.RCDIV = 1;
    stopRobot();
    if (OC1CON1bits.OCM == 0) { /* If PWM is off, turn it on 
        OC1CON1bits.OCM = 0b110;Trying to turn PWM on while it is on repeatedly at a high frequency make it do weird things 
    }
    OC1RS = 39999; // Period
    OC1R = 1500; // Duty cycle
    while (current_angle1 < 5000) {
        OC1R = current_angle1;
        current_ir = 1;
        if (current_ir > max_ir) {
            max_ir = current_ir;
            max_ir_angle = current_angle1;
        }
        current_angle1 = current_angle1 + 10;
    }
    while (1) {
        OC1R = 5000;
    }
    */
}

void _ISR _T1Interrupt(void) {
    _T1IF = 0;
    timer_complete = true;
    if (service_flag) {
        OC1CON1bits.OCM = 0;/* Trying to turn PWM on while it is on repeatedly at a high frequency make it do weird things */
        service_flag = false;
    }
    stopTimers();
}


#endif	//DRIVE_H
