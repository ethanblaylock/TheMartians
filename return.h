#ifndef RETURN_H
#define	RETURN_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define BUMPING_DISTANCE 5
#define ALLIGNMENT_DISTANCE 8


#define COLOR_QRD ADC1BUF13
#define QRD_THRESHOLD 2048

static enum circle turning_direction = CLOCKWISE;

void returnSample() {
    if (COLOR_QRD < QRD_THRESHOLD) {
        turning_direction = COUNTERCLOCKWISE;
    } else {
        turning_direction = CLOCKWISE;
    }
    drive_completed = false;
    driveStraight(ALLIGNMENT_DISTANCE + (turning_direction ^ 1)*1, FORWARD);
    while(!drive_completed) {driveStraight(ALLIGNMENT_DISTANCE + (turning_direction ^ 1)*2, FORWARD);}
    drive_completed = false;
    turnRobot(90, turning_direction);
    while(!drive_completed) {}
    drive_completed = false;
    driveStraight(BUMPING_DISTANCE, REVERSE);
    while(!drive_completed) {}
    stopRobot();
    createTimer(1, 15625, 256);
    while(!timer_complete) {}
    stopTimers();
    drive_completed = false;
    driveStraight(BUMPING_DISTANCE-0.75, FORWARD);
    while(!drive_completed) {}
    drive_completed = false;
    turnRobot(90, turning_direction ^ 1);
    while(!drive_completed) {}
    stripe_steps = 0;
    stripe_count = 0;
    stripe_flag = false;
    current_state = FOLLOW_LINE;
    _AD1IE = 1;
    
}


#endif	/* RETURN_H */

 
