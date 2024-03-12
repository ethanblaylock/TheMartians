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
    enum line_state current_state = NAVIGATE_CANYON; // Sets current_state
    while(1) {
        // Figures out what to do based on current state
        if (current_state == FOLLOW_LINE) {
            followLine();
        }
        else if (current_state == NAVIGATE_CANYON) {
            navigateCanyon();
        }
    }

    return 0;
}