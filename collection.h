
#ifndef COLLECTION_H
#define	COLLECTION_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define BUMPING_DISTANCE 18
#define ALLIGNMENT_DISTANCE 7.5


void collectSample() {
    drive_completed = false;
    driveStraight(ALLIGNMENT_DISTANCE, FORWARD);
    collect_flag = true;
    while(!drive_completed) {
       driveStraight(ALLIGNMENT_DISTANCE, FORWARD);
    }
    collect_flag = false;
    drive_completed = false;
    turnRobot(86, CLOCKWISE);
    while(!drive_completed) {
        turnRobot(86, CLOCKWISE);
    }
    drive_completed = false;
    driveStraight(BUMPING_DISTANCE, REVERSE);
    while(!drive_completed) {
        driveStraight(BUMPING_DISTANCE, REVERSE);
    }
    stopRobot();
    createTimer(1, 15625, 256);
    while(!timer_complete) {
    }
    stopTimers();
    while(current_state == COLLECT_SAMPLE) {
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
            collect_flag = true;
            driveStraight(5.5, FORWARD);
            while(!drive_completed) {
            }
            collect_flag = false;
            drive_completed = false;
            turnRobot(90, COUNTERCLOCKWISE);
            while(!drive_completed) {}
            stripe_steps = 0;
            stripe_count = 0;
            stripe_flag = false;
            current_state = FOLLOW_LINE;
            _AD1IE = 1;
        }
        else {
            driveStraight(ALLIGNMENT_DISTANCE, FORWARD);
        }
    }
}


#endif	/* COLLECTION_H */

