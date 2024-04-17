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
#include "collection.h"
#include "return.h"
#include "transmission.h"
#include "servicing.h"

int main(void) {
    // States : FOLLOW_LINE, NAVIGATE_CANYON
    setup();
    
    while(1) {
        _LATB7 = 1;
        OC1R = servo_frequency; // Duty cycle
        // Figures out what to do based on current state
        switch(current_state) {
            case START:
                start();
                break;
            case FOLLOW_LINE:
                followLine();
                break;
            case NAVIGATE_CANYON:
                navigateCanyon();
                break;
            case COLLECT_SAMPLE:
                collectSample();
                break;
            case RETURN_SAMPLE:
                returnSample();
                break;
            case SERVICE_EQUIPMENT:
                serviceEquipmentGun();
                break;
            case DATA_TRANSMISSION:
                transmitData();
                break;
        }
        
    }

    return 0;
}