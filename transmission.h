#ifndef TRANSMISSION_H
#define	TRANSMISSION_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "servo.h"
#include "drive.h"

#define MAX_ANGLE 5000
#define MIN_ANGLE 0

#define ANGLE_INCREMENT 0.1
#define ALLIGNMENT_DISTANCE 1
#define IR_SENSOR ADC1BUF9

static double current_angle = MAX_ANGLE;
static double max_ir_angle = MAX_ANGLE;
static double current_ir = 0.0;
static double max_ir = 0.0;

void transmitData(void) {
    _LATB7 = 0;
    drive_completed = false;
    driveStraight(2, FORWARD);
    collect_flag = true;
    while(!drive_completed) {
       driveStraight(2, FORWARD);
    }
    collect_flag = false;
    drive_completed = false;
    turnRobot(85, COUNTERCLOCKWISE);
    while(!drive_completed) {
        turnRobot(85, COUNTERCLOCKWISE);
    }
    drive_completed = false;
    driveStraight(27, REVERSE);
    while(!drive_completed) {
       driveStraight(27, REVERSE);
    }
    CLKDIVbits.RCDIV = 1;
    stopRobot();
    if (OC1CON1bits.OCM == 0) { /* If PWM is off, turn it on */
        OC1CON1bits.OCM = 0b110; /* Trying to turn PWM on while it is on repeatedly at a high frequency make it do weird things */
    }
    OC1RS = 39999;
    OC1R = 5000; // Duty cycle
    current_angle = 5000;
    while (current_angle > 1500) {
        _LATB7 = 0;
        OC1R = current_angle;
        current_ir = IR_SENSOR;
        if (current_ir > max_ir) {
            max_ir = current_ir;
            max_ir_angle = current_angle;
        }
        current_angle = current_angle - 0.1;
    }
    _OC2IE = 0;
    _OC3IE = 0;
    _AD1IE = 0;
    _T1IE = 0;
    OC1R = max_ir_angle;
    while (1) {
        _LATB7 = 0;
        OC1R = max_ir_angle;
        turnOnLaser();
    }
}

void turnOnLaser(void) {
    setOutputPin(17, 1);
}

#endif	/* TRANSMISSION_H */

