
#ifndef CANYON_H
#define	CANYON_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define VCC 3.3
#define MAX_RANGE 118.11

#define FRONT_SONAR ADC1BUF11
#define RIGHT_SONAR ADC1BUF12

#define TILE_LENGTH 23 //inches
#define MAX_WALL_DISTANCE 9
#define OPEN_DISTANCE 15
#define STRAIGHT_DISTANCE 1 // Inches
#define DISTANCE_FROM_SONAR_TO_CENTER 3.5 // inches
#define HYSTERESIS_DISTANCE 15
#define CANYON_SPEED_MODIFIER 1.2

static int canyon_state = 0;

double getDistance(double voltage) {
    voltage = (voltage/4095.0)*VCC;
    double distance = MAX_RANGE * (voltage / VCC);
    return distance;
}

void makeLeftCanyonTurn() {
    turnRobot(90, CLOCKWISE);
    while (!drive_completed) {}
    driveStraight(TILE_LENGTH/2, REVERSE);
    while (!drive_completed) {}
}

void makeRightCanyonTurn() {
    turnRobot (90, COUNTERCLOCKWISE);
    while (!drive_completed) {}
    driveStraight(TILE_LENGTH/2, REVERSE);
    while (!drive_completed) {}
}


void navigateCanyon(void) {
    if (getDistance(FRONT_SONAR) < MAX_WALL_DISTANCE) {
        if (getDistance(RIGHT_SONAR) > OPEN_DISTANCE) {
            makeRightCanyonTurn();
        }
        else {
            makeLeftCanyonTurn();
        }
    } else {
        driveStraight(STRAIGHT_DISTANCE, REVERSE);
    }
    
    // State transition logic
    if (LEFT_QRD <= QRD_THRESHOLD || RIGHT_QRD <= QRD_THRESHOLD) {
        getBackOnLine();
    }
}

void getBackOnLine(void) {
    if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) {
        driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, FORWARD);
    }
    else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
        driveMotor(MOTOR_PWM_2, PWM_FREQUENCY, MOTOR_DIR_1, FORWARD);
    }
}


#endif	/* CANYON_H */

