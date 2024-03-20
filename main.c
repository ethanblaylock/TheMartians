/*
 * File:   main.c
 * Author: ethanblaylock
 *
 * Created on February 28, 2024, 5:10 PM
 */

#include "xc.h"
#include "line.h"
#include "canyon.h"

int main(void) {
    // States : FOLLOW_LINE, NAVIGATE_CANYON
    setup();

    while(1) {
        // Figures out what to do based on current state
        if (current_state == FOLLOW_LINE) {
            followLine();
            _LATB15 = 0;
        }
        else if (current_state == NAVIGATE_CANYON) {
            navigateCanyon();
            _LATB15 = 1;
        }
    }

    return 0;
}