/*
 * File:   main.c
 * Author: ethanblaylock
 *
 * Created on February 28, 2024, 5:10 PM
 */

#include "drive.h"
#include "xc.h"

int main(void) {
    setup();
    while(1) {
        switch (current_state) {
            case 1:
                driveStraight(36, FORWARD);
                break;
            case 2:
                turnRobot(90, CLOCKWISE);
                break;
            case 3:
                driveStraight(36, FORWARD);
                break;
            case 4:
                turnRobot(180, COUNTERCLOCKWISE);
                break;
            case 5:
                driveStraight(36, FORWARD);
                break;
            case 6:
                stopRobot();
                break;  
        }
    }

    return 0;
}