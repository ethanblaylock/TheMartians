
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
#define MAX_WALL_DISTANCE 100
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
    turnRobot(92, CLOCKWISE);
    while (!drive_completed) {}
    driveStraight(TILE_LENGTH/2, REVERSE);
    while (!drive_completed) {}
}

void makeRightCanyonTurn() {
    turnRobot (92, COUNTERCLOCKWISE);
    while (!drive_completed) {}
    driveStraight(TILE_LENGTH/2, REVERSE);
    while (!drive_completed) {}
}

/*
void navigateCanyon(void) {
    // 23 inches between canyon walls
    if (getDistance(LEFT_SONAR) < MAX_WALL_DISTANCE && getDistance(RIGHT_SONAR) < MAX_WALL_DISTANCE) {
        if (fabs(getDistance(LEFT_SONAR) - getDistance(RIGHT_SONAR)) > HYSTERESIS_DISTANCE) {
            if (getDistance(LEFT_SONAR) > getDistance(RIGHT_SONAR)) {
                driveDifferentialy(STRAIGHT_DISTANCE, RIGHT, CANYON_SPEED_MODIFIER);
            } else {
                driveDifferentialy(STRAIGHT_DISTANCE, LEFT, CANYON_SPEED_MODIFIER);
            }
        } else {
            driveStraight(STRAIGHT_DISTANCE, REVERSE);
        }
    } 
    else if (getDistance(LEFT_SONAR) > MAX_WALL_DISTANCE) {
        makeLeftCanyonTurn();
    }
    else if (getDistance(RIGHT_SONAR) > MAX_WALL_DISTANCE) {
        makeRightCanyonTurn();
    }
    else { 
        stopRobot();
    }
}*/

void navigateCanyon(void) {
    if (getDistance(FRONT_SONAR) < 9) {
        if (getDistance(RIGHT_SONAR) > 15) {
            makeRightCanyonTurn();
        }
        else {
            makeLeftCanyonTurn();
        }
    } else {
        driveStraight(STRAIGHT_DISTANCE, REVERSE);
    }
}

#endif	/* CANYON_H */

