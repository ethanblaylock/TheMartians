#ifndef SERVICING_H
#define	SERVICING_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define BUMPING_DISTANCE 17
#define ALLIGNMENT_DISTANCE 7.5

#define SERVICE_QRD_THRESHOLD 2500



void serviceEquipmentGun() {
    if (OC1CON1bits.OCM == 0) { /* If PWM is off, turn it on */
        OC1CON1bits.OCM = 0b110; /* Trying to turn PWM on while it is on repeatedly at a high frequency make it do weird things */
    }
    OC1RS = 39999;
    servo_frequency = 2500; // Duty cycle
    OC1R = servo_frequency;
    end_steps = 0;
    stripe_count = 0;
    end_flag = false;
    service_flag = true;
    createTimer(1, 15000, 256);
    current_state = FOLLOW_LINE;
    _AD1IE = 1;
    _AD1IF = 0;
}





void serviceEquipment() {
    current_state = SERVICE_EQUIPMENT;
    drive_completed = false;
    driveStraight(5, FORWARD);
    collect_flag = true;
    while(!drive_completed) {
       driveStraight(5, FORWARD);
    }
    collect_flag = false;
    drive_completed = false;
    turnRobot(88, CLOCKWISE);
    while(!drive_completed) {
        turnRobot(88, CLOCKWISE);
    }
    drive_completed = false;
    driveStraight(BUMPING_DISTANCE, REVERSE);
    while(!drive_completed) {driveStraight(BUMPING_DISTANCE, REVERSE);}
    stopRobot();
    createTimer(1, 3000, 256);
    while(!timer_complete) {}
    stopTimers();
    drive_completed = false;
    driveStraight(4, FORWARD);
    while(!drive_completed) {
       driveStraight(4, FORWARD);
    }
    current_state = SERVICE_EQUIPMENT;
    _AD1IE = 0;
    _AD1IF = 0;
    while(current_state == SERVICE_EQUIPMENT) {
        end_detection = 0;
        end_steps = 0;
        end_flag = false;
        if (LEFT_QRD <= SERVICE_QRD_THRESHOLD && RIGHT_QRD >= SERVICE_QRD_THRESHOLD) {
            stopRobot();
            driveMotor(MOTOR_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, FORWARD);
        }
        else if (LEFT_QRD >= SERVICE_QRD_THRESHOLD && RIGHT_QRD <= SERVICE_QRD_THRESHOLD) {
            stopRobot();
            driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, FORWARD);
        }
        else if (LEFT_QRD <= SERVICE_QRD_THRESHOLD && RIGHT_QRD <= SERVICE_QRD_THRESHOLD) {
            end_detection = 0;
            drive_completed = false;
            collect_flag = true;
            driveStraight(5, FORWARD);
            while(!drive_completed) {}
            collect_flag = false;
            drive_completed = false;
            turnRobot(90, COUNTERCLOCKWISE);
            while(!drive_completed) {}
            end_steps = 0;
            stripe_count = 0;
            end_flag = false;
            current_state = FOLLOW_LINE;
            _AD1IE = 1;
            _AD1IF = 0;
        }
        else {
            driveStraight(ALLIGNMENT_DISTANCE, FORWARD);
        }
    }
}


#endif	/* COLLECTION_H */

