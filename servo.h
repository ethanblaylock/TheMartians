#ifndef SERVO_H
#define	SERVO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "helper.h"

#define SERVO_PWM_PIN 14
#define SERVO_PWM_FREQUENCY 50

//duty cycle between .025 and .125 for 0 to 180 degrees

void driveServo(double angle) {
    double dutyCycle = .025 + angle*.1/180.0;
    configurePWM(SERVO_PWM_PIN, SERVO_PWM_FREQUENCY, dutyCycle);
}

#endif	/* SERVO_H */

