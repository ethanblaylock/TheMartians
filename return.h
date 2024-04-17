#ifndef RETURN_H
#define	RETURN_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define BUMPING_DISTANCE 6
#define ALLIGNMENT_DISTANCE 8


#define COLOR_QRD ADC1BUF13
#define QRD_THRESHOLD 300

static enum circle turning_direction = CLOCKWISE;

void returnSample() {
    if (COLOR_QRD < QRD_THRESHOLD) {
        turning_direction = COUNTERCLOCKWISE;
    } else {
        turning_direction = CLOCKWISE;
    }
    drive_completed = false;
    collect_flag = true;
    driveStraight(ALLIGNMENT_DISTANCE + (turning_direction ^ 1)*1, FORWARD);
    while(!drive_completed) {
        driveStraight(ALLIGNMENT_DISTANCE + (turning_direction ^ 1)*2, FORWARD);
    }
    drive_completed = false;
    collect_flag = false;
    if (turning_direction == COUNTERCLOCKWISE) {
        turnRobot(86, turning_direction);
        while(!drive_completed) {
            turnRobot(86, turning_direction);
        }
    } else {
        turnRobot(80, turning_direction);
        while(!drive_completed) {
            turnRobot(80, turning_direction);
        }
    }
    drive_completed = false;
    driveStraight(BUMPING_DISTANCE, REVERSE);
    while(!drive_completed) {
        driveStraight(BUMPING_DISTANCE, REVERSE);
    }
    stopRobot();
    createTimer(1, 7500, 256);
    while(!timer_complete) {
    }
    stopTimers();
    drive_completed = false;
    collect_flag = true;
    driveStraight(BUMPING_DISTANCE-1.5, FORWARD);
    while(!drive_completed) {
        driveStraight(BUMPING_DISTANCE-1.5, FORWARD);
    }
    drive_completed = false;
    collect_flag = false;
    turnRobot(90, turning_direction ^ 1);
    while(!drive_completed) {
        turnRobot(90, turning_direction ^ 1);
    }
    stripe_steps = 0;
    stripe_count = 0;
    stripe_flag = false;
    current_state = FOLLOW_LINE;
    _AD1IE = 1;
    
}


#endif	/* RETURN_H */

 
