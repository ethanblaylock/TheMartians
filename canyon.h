
#ifndef CANYON_H
#define	CANYON_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define VCC 3.3
#define MAX_RANGE 118.11 // Inches

#define FRONT_SONAR ADC1BUF11
#define RIGHT_SONAR ADC1BUF12

#define TILE_LENGTH 23 //inches
#define MAX_WALL_DISTANCE 8
#define SLOW_DOWN_DISTANCE 15
#define OPEN_DISTANCE 20
#define STRAIGHT_DISTANCE 1 // Inches
#define DISTANCE_FROM_SONAR_TO_CENTER 3.5 // inches
#define HYSTERESIS_DISTANCE 15
#define CANYON_SPEED_MODIFIER 1.2

/**
 * Turns the voltage form the sonar into a distance in inches
 * @param voltage the raw voltage from the sonar
 * @return The distance in inches
 */
double getDistance(double voltage) {
    voltage = (voltage/4095.0)*VCC;
    double distance = MAX_RANGE * (voltage / VCC);
    return distance;
}

/**
 * Turns the robot and drive forward to not interfere with the sensors
 */
void makeLeftCanyonTurn() {
    stopRobot();
    drive_completed = false;
    turnRobot(90, COUNTERCLOCKWISE);
    while (!drive_completed) {}
    slow_down_flag = false;
    drive_completed = false;
    driveStraight(TILE_LENGTH/2, FORWARD);
    while (!drive_completed) {
        driveStraight(TILE_LENGTH/2, FORWARD);
    }
    has_turned = true;
}

/**
 * Turns the robot and drive forward to not interfere with the sensors
 */
void makeRightCanyonTurn() {
    stopRobot();
    drive_completed = false;
    turnRobot (90, CLOCKWISE);
    while (!drive_completed) {}
    slow_down_flag = false;
    drive_completed = false;
    driveStraight(TILE_LENGTH/2, FORWARD);
    while (!drive_completed) {
        driveStraight(TILE_LENGTH/2, FORWARD);
    }
    has_turned = true;
}

/**
 * Logic to navigate the canyon
 * Turns the robot once the front end gets too close to a wall
 */
void navigateCanyon(void) {
    if (getDistance(FRONT_SONAR) < SLOW_DOWN_DISTANCE) {
        slow_down_flag = true;
    } else {
        slow_down_flag = false;
    }
    
    if (getDistance(FRONT_SONAR) < MAX_WALL_DISTANCE) {

        if (getDistance(RIGHT_SONAR) > OPEN_DISTANCE) {
            makeLeftCanyonTurn();
        }
        else {
            makeRightCanyonTurn();
        }
    } else {
        driveStraight(STRAIGHT_DISTANCE, FORWARD);
    } 
     
    
    // State transition logic
    if (LEFT_QRD <= QRD_THRESHOLD || RIGHT_QRD <= QRD_THRESHOLD) {
        if (has_turned) {
            getBackOnLine();
        }
    }
}

/**
 * These are the preprogrammed coordinates to get the robot back to the line following state
 */
void getBackOnLine(void) {
    while (current_state == NAVIGATE_CANYON) {
        OC1RS = 39999; // Period
        OC1R = servo_frequency; // Duty cycle
        if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) {
            stopRobot();
            driveMotor(MOTOR_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, FORWARD);
        }
        else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
            stopRobot();
            driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, FORWARD);
        }
        else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
            drive_completed = false;
            driveStraight(5, FORWARD);
            while(!drive_completed) {
                if (chosen_frequency > 400) {
                    chosen_frequency = chosen_frequency - 4;
                }
                driveStraight(5, FORWARD);
            }
            drive_completed = false;
            if (getDistance(RIGHT_SONAR) < OPEN_DISTANCE) {
                turnRobot(90, CLOCKWISE);
            } else {
                turnRobot(90, COUNTERCLOCKWISE);
            }
            while(!drive_completed) {}
            stripe_steps = 0;
            stripe_count = 0;
            stripe_flag = false;
            current_state = FOLLOW_LINE;
            _AD1IE = 1;
        }
    }
}


#endif	/* CANYON_H */

