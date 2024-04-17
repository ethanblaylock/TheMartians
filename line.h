
#ifndef LINE_H
#define	LINE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "drive.h"

#define QRD_THRESHOLD 2048 // 4096 is max, so 2048 means half of reference volatage, ie. 2048/4096 * 3.3V = 1.65 V

#define LEFT_QRD ADC1BUF0
#define RIGHT_QRD ADC1BUF1
#define STRIPE_QRD ADC1BUF4
#define END_QRD ADC1BUF14 //pin8


#define STRAIGHT_DISTANCE 1 // Inches

#define ADJUSTMENT_DEGREES 2.5 // Degrees
#define ADJUSTMENT_DISTANCE 1 // Inches

#define DISTANCE_BETWEEN_QRD 2.25 // Inches

#define SPEED_MODIFIER_INCREMENT 0.08
#define SPEED_MODIFIER_INCREMENT2 0.015

static double speed_modifier = 1; // A multiplier for speed
static int line_state = 1; // A variable for keeping track of what state of line following the robot is on
static enum leftRight last_direction = LEFT;
/**
 * Uses QRD Sensor and variably increase the speed of the wheels to compensate
 * for larger readjustments
 */
void followLine(void) {
    
    if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) {/* Both see black */
        if (speed_modifier > 1) {
            speed_modifier = speed_modifier - SPEED_MODIFIER_INCREMENT; // Increase turning speed if robot is still on line
        }
        if (motor_speed_modifier_low > 1) {
            motor_speed_modifier_low = motor_speed_modifier_low - SPEED_MODIFIER_INCREMENT;
        }
        if (motor_speed_modifier_high > 1) {
            motor_speed_modifier_high = motor_speed_modifier_high - SPEED_MODIFIER_INCREMENT;
        }
        driveDifferentialy(ADJUSTMENT_DISTANCE, last_direction, speed_modifier, true);
        line_state = 0;
    }
    else if (LEFT_QRD >= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) { /* Robot is drifting to the left */
        if (line_state == 1) {
            if (speed_modifier < 2.5) {
                speed_modifier = speed_modifier + SPEED_MODIFIER_INCREMENT2; // Increase turning speed if robot is still on line
            }
            if (motor_speed_modifier_low < 2.5) {
                motor_speed_modifier_low = motor_speed_modifier_low + SPEED_MODIFIER_INCREMENT2;

            }
            if (motor_speed_modifier_high < 2.5) {
                motor_speed_modifier_high = motor_speed_modifier_high + SPEED_MODIFIER_INCREMENT2;
            }
        } else {   
            if (speed_modifier > 1) {
            speed_modifier = speed_modifier - SPEED_MODIFIER_INCREMENT; // Increase turning speed if robot is still on line
            }
            if (motor_speed_modifier_low > 1) {
                motor_speed_modifier_low = motor_speed_modifier_low - SPEED_MODIFIER_INCREMENT;
            }
            if (motor_speed_modifier_high > 1) {
                motor_speed_modifier_high = motor_speed_modifier_high - SPEED_MODIFIER_INCREMENT;
            }     
        }
        last_direction = RIGHT;
        driveDifferentialy(ADJUSTMENT_DISTANCE, RIGHT, speed_modifier, false);
        line_state = 1;
    } 
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD >= QRD_THRESHOLD) { /* Robot is drifting to the right */
        if (line_state == 2) {
            if (speed_modifier < 2.5) {
            speed_modifier = speed_modifier + SPEED_MODIFIER_INCREMENT2; // Increase turning speed if robot is still on line
            }
            if (motor_speed_modifier_low < 2.5) {
                motor_speed_modifier_low = motor_speed_modifier_low + SPEED_MODIFIER_INCREMENT2;

            }
            if (motor_speed_modifier_high < 2.5) {
                motor_speed_modifier_high = motor_speed_modifier_high + SPEED_MODIFIER_INCREMENT2;
            }
        } else {
            if (speed_modifier > 1) {
            speed_modifier = speed_modifier - SPEED_MODIFIER_INCREMENT; // Increase turning speed if robot is still on line
            }
            if (motor_speed_modifier_low > 1) {
                motor_speed_modifier_low = motor_speed_modifier_low - SPEED_MODIFIER_INCREMENT;
            }
            if (motor_speed_modifier_high > 1) {
                motor_speed_modifier_high = motor_speed_modifier_high - SPEED_MODIFIER_INCREMENT;
            }  
        }
        last_direction = LEFT;
        driveDifferentialy(ADJUSTMENT_DISTANCE, LEFT, speed_modifier, false);
        line_state = 2;
    }
    else if (LEFT_QRD <= QRD_THRESHOLD && RIGHT_QRD <= QRD_THRESHOLD) {
        // Turn into lander or out of canyon
        line_state = 3;
    }
    
    
     // State transition logic
    if (stripe_steps > 6*STEPS_PER_INCH) {
         stripe_flag = false;
         end_flag = false;
         end_steps = 0;
         stripe_steps = 0;
         
         if (stripe_count == 2) {
             speed_modifier = 1.0; // Increase turning speed if robot is still on line
         motor_speed_modifier_low = 1.0;
         motor_speed_modifier_high = 1.0;
             _AD1IE = 0;
             current_state = COLLECT_SAMPLE;
         }
         else if (stripe_count == 3) {
             speed_modifier = 1.0; // Increase turning speed if robot is still on line
         motor_speed_modifier_low = 1.0;
         motor_speed_modifier_high = 1.0;
             _AD1IE = 0;
             current_state = RETURN_SAMPLE;
         }
         else if (stripe_count >= 4) {
             speed_modifier = 1.0; // Increase turning speed if robot is still on line
         motor_speed_modifier_low = 1.0;
         motor_speed_modifier_high = 1.0;
             _AD1IE = 0;
            has_turned = false;
            createTimer(1, 3000, 256);
            while (!timer_complete) {
                OC1RS = 39999; // Period
                OC1R = servo_frequency; // Duty cycle
                followLine();
            }
            stopTimers();
            current_state = NAVIGATE_CANYON;
         }
         stripe_count = 0;
         end_detection = 0;
    }
    
    if (end_steps > 2*STEPS_PER_INCH) {
        end_flag = false;
        end_steps = 0;
        if (end_detection > 15) {
            speed_modifier = 1.0; // Increase turning speed if robot is still on line
         motor_speed_modifier_low = 1.0;
         motor_speed_modifier_high = 1.0;
            _AD1IE = 0;
            current_state = DATA_TRANSMISSION;
            
        }
        end_detection = 0;
    }

    if (!end_flag) {
        end_steps = 0;
        end_detection = 0;
    }
    
    if (!stripe_flag) {
        stripe_count = 0;
        stripe_steps = 0;
    }
     
}
     
#endif	/* LINE_H */

