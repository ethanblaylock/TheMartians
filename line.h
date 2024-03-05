
#ifndef LINE_H
#define	LINE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "drive.h"

#define QRD_THRESHOLD 2048 // 4096 is max, so 2048 means half of reference volatage, ie. 2048/4096 * 3.3V = 1.65 V

#define LEFT_QRD ADC1BUF0
#define RIGHT_QRD ADC1BUF1

#define STRAIGHT_DISTANCE 5 // Inches

#define ADJUSTMENT_DEGREES 15.0 // Degrees
#define ADJUSTMENT_DISTANCE 1 // Inches

#define DISTANCE_BETWEEN_QRD 4 // Inches

/**
 * Based off QRD sensors makes adjustments to follow white line
 */
void followLine(void) {
    if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Both see black */
        driveStraight(STRAIGHT_DISTANCE, FORWARD);
    }
    else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) { /* Robot is drifting to the left */
        turnRobot(ADJUSTMENT_DEGREES, CLOCKWISE);
    } 
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Robot is drifting to the right */
        turnRobot(ADJUSTMENT_DEGREES, COUNTERCLOCKWISE);
    }
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
        // Turn into lander or out of canyon
        stopRobot();
    }
}

/**
 * Similar to followLine but this one blocks code to ensure turns are fully executed
 */
void followLine2(void) {
    if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Both see black */
        driveStraight(STRAIGHT_DISTANCE, FORWARD);
    }
    else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) { /* Robot is drifting to the left */
        turnRobot(ADJUSTMENT_DEGREES, CLOCKWISE);
        while(!readjustment_complete) {}
        driveStraight(ADJUSTMENT_DISTANCE, FORWARD);
        while(!readjustment_complete) {}
        turnRobot(round(ADJUSTMENT_DEGREES/2), COUNTERCLOCKWISE);
    } 
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Robot is drifting to the right */
        turnRobot(ADJUSTMENT_DEGREES, COUNTERCLOCKWISE);
        while(!readjustment_complete) {}
        driveStraight(ADJUSTMENT_DISTANCE, FORWARD);
        while(!readjustment_complete) {}
        turnRobot(round(ADJUSTMENT_DEGREES/2), CLOCKWISE);
    }
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
        // Turn into lander or out of canyon
        stopRobot();
    }
}

#endif	/* LINE_H */

