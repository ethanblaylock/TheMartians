
#ifndef LINE_H
#define	LINE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "drive.h"

#define QRD_THRESHOLD 2048 // 4096 is max, so 2048 means half of reference volatage, ie. 2048/4096 * 3.3V = 1.65 V

#define LEFT_QRD ADC1BUF0
#define RIGHT_QRD ADC1BUF1
#define STRIPE_QRD ADC1BUF4


#define STRAIGHT_DISTANCE 1 // Inches

#define ADJUSTMENT_DEGREES 2.5 // Degrees
#define ADJUSTMENT_DISTANCE 1 // Inches

#define DISTANCE_BETWEEN_QRD 2.25 // Inches

#define SPEED_MODIFIER_INCREMENT 0.025

static double speed_modifier = 1; // A multiplier for speed
static int line_state = 1; // A variable for keeping track of what state of line following the robot is on

/**
 * Uses QRD Sensor and variably increase the speed of the wheels to compensate
 * for larger readjustments
 */
void followLine(void) {
    if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) {/* Both see black */
        speed_modifier = 1.0;
        driveStraight(STRAIGHT_DISTANCE, FORWARD);
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
        // Turn into lander or out of canyon
        stopRobot();
        line_state = 3;
    }
    
    
     // State transition logic
    if (stripe_steps > 10*STEPS_PER_INCH) {
         stripe_flag = false;
         stripe_steps = 0;
         if (stripe_count == 2) {
         }
         else if (stripe_count == 3) {
         }
         else if (stripe_count == 4) {
            has_turned = false;
            current_state = NAVIGATE_CANYON;
         }
         stripe_count = 0;
     }
     
}
     
#endif	/* LINE_H */

