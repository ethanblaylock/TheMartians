
#ifndef START_H
#define	START_H

#include "xc.h"
#include "drive.h"
#include "math.h"

#define LEFT_QRD ADC1BUF0
#define RIGHT_QRD ADC1BUF1

#define QRD_THRESHOLD 2048
#define SPEED_MODIFIER_INCREMENT 0.01
#define STRAIGHT_DISTANCE 1
#define ADJUSTMENT_DISTANCE 1

void start() {
    if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) {/* Both see black */
        speed_modifier = 1.0;
        driveSlow(STRAIGHT_DISTANCE, FORWARD);
        line_state = 0;
    }
    else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) { /* Robot is drifting to the left */
        if (line_state == 1) {
            speed_modifier = speed_modifier + SPEED_MODIFIER_INCREMENT; // Increase turning speed if robot is still on line
        } else {
            speed_modifier = 1.0; // Resets the speed modifier
        }
        driveDifferentialy(ADJUSTMENT_DISTANCE, RIGHT, speed_modifier);
        line_state = 1;
    } 
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Robot is drifting to the right */
        if (line_state == 2) {
            speed_modifier = speed_modifier + SPEED_MODIFIER_INCREMENT; // Increase turning speed if robot is still on line
        } else {
            speed_modifier = 1.0; // Resets the speed modifier
        }
        driveDifferentialy(ADJUSTMENT_DISTANCE, LEFT, speed_modifier);
        line_state = 2;
    }
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
        driveStraight(5, FORWARD);
        while(!drive_completed) {}
        turnRobot(90, CLOCKWISE);
        while(!drive_completed) {}
        current_state = FOLLOW_LINE;
    }
}


#endif	/* START_H */

