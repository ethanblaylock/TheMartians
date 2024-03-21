/*
 * File:   main.c
 * Author: ethanblaylock
 *
 * Created on February 28, 2024, 5:10 PM
 */

#include "xc.h"
#include "line.h"
#include "canyon.h"
#include "start.h"

int main(void) {
    // States : FOLLOW_LINE, NAVIGATE_CANYON
    setup();

    while(1) {
        
        switch(stripes_detected) {
            case 0:
                _LATB7 = 0;
                _LATB15 = 0;
                _LATB14 = 0;
                break;
            case 1:
                _LATB7 = 1;
                _LATB15 = 0;
                _LATB14 = 0;
                break;
            case 2:
                _LATB7 = 0;
                _LATB15 = 1;
                _LATB14 = 0;
                break;
            case 3:
                _LATB7 = 1;
                _LATB15 = 1;
                _LATB14 = 0;
                break;
            case 4:
                _LATB7 = 0;
                _LATB15 = 0;
                _LATB14 = 1;
                break;
            case 5:
                _LATB7 = 1;
                _LATB15 = 0;
                _LATB14 = 1;
                break;
            case 6:
                _LATB7 = 0;
                _LATB15 = 1;
                _LATB14 = 1;
                break;
            case 7:
                _LATB7 = 1;
                _LATB15 = 1;
                _LATB14 = 1;
                break;
                
        }
        // Figures out what to do based on current state
        if (current_state == START) {
            start();
        }
        else if (current_state == FOLLOW_LINE) {
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