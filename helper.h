#ifndef HELPER_H
#define	HELPER_H

#include "xc.h" 
#include "math.h"
#include "setup.h"

/**
 * Creates a timer given the following parameters.
 * @param timerNumber Specifies which of 5 timers is to be used.
 * @param periodValue Specifies the period for the timer to reset
 * @param prescale Defaults to 1. Accepts 1,8,64, or 256 as a prescale.
 */
void createTimer(int timerNumber, int periodValue, int prescale) {
    timer_complete = false;
    _T1IE = 1;
    _T1IF = 0;
    int tckpsValue = 0b0;
    //Takes the parameter and turns it into the needed TCKPS bits
    switch(prescale) {
        case 0:
            tckpsValue = 0b00;
            break;
        case 8:
            tckpsValue = 0b01;
            break;
        case 64:
            tckpsValue = 0b10;
            break;
        case 256:
            tckpsValue = 0b11;
            break;
    }
    
    //Sets the appropriate timer and timer settings
    switch(timerNumber) {
        case 1:
            T1CONbits.TON = 1;
            T1CONbits.TCS = 0;
            T1CONbits.TCKPS = tckpsValue;
            PR1 = periodValue;
            TMR1 = 0;
            break;
        case 2:
            T2CONbits.TON = 1;
            T2CONbits.TCS = 0;
            T2CONbits.TCKPS = tckpsValue;
            PR2 = periodValue;
            TMR2 = 0;
            break;
        case 3:
            T3CONbits.TON = 1;
            T3CONbits.TCS = 0;
            T3CONbits.TCKPS = tckpsValue;
            PR3 = periodValue;
            TMR3 = 0;
            break;
        case 4:
            T4CONbits.TON = 1;
            T4CONbits.TCS = 0;
            T4CONbits.TCKPS = tckpsValue;
            PR4 = periodValue;
            TMR4 = 0;
            break;
        case 5:
            T5CONbits.TON = 1;
            T5CONbits.TCS = 0;
            T5CONbits.TCKPS = tckpsValue;
            PR5 = periodValue;
            TMR5 = 0;
            break;
    }
}

void stopTimers(void) {
    T1CONbits.TON = 0;
    _T1IE = 0;
    TMR1 = 0;
}
/**
 * Configure a certain PWM to start running
 * @param ocNum the specific OC to turn on
 * @param period the period in Hz
 * @param dutyCycle the duty cycle as a fraction of 1
 */
void configurePWM(int pinNum, double period, double dutyCycle) {
    int value = round((1/(period/2))*FCY) - 1;
    int dutyValue = round(dutyCycle*value);
    
    switch(pinNum) {
        case 14:
            if (OC1CON1bits.OCM == 0) { /* If PWM is off, turn it on */
                OC1CON1bits.OCM = 0b110; /* Trying to turn PWM on while it is on repeatedly at a high frequency make it do weird things */
            }
            OC1RS = value; // Period
            OC1R = dutyValue; // Duty cycle 
            break;
        case 4: 
            if (OC2CON1bits.OCM == 0) {
                OC2CON1bits.OCM = 0b110;
            }
            OC2RS = value; // Period
            OC2R = dutyValue; // Duty cycle 
            break;
        case 5:
            if (OC3CON1bits.OCM == 0) {
                OC3CON1bits.OCM = 0b110;
            }
            OC3RS = value; // Period
            OC3R = dutyValue; // Duty cycle 
            break;
    }
}

/**
 * Sets the _LAT for the pin to high or low
 * @param pinNum The number of the pin (1,2,3,4,...etc.)
 * @param pinState High or Low
 */
void setOutputPin(int pinNum, int pinState) {
    switch(pinNum) {
        case 1:
            
            break;
        case 2:
            _LATA0 = pinState;
            break;
        case 3:
            _LATA1 = pinState;
            break;
        case 6:
            _LATB2 = pinState;
            break;
        case 7:
            _LATA2 = pinState;
            break;
        case 8:
            _LATA3 = pinState;
            break;
        case 9:
            _LATB4 = pinState;
            break;
        case 10:
            _LATA4 = pinState;
            break;
        case 11:
            _LATB7 = pinState;
            break;
        case 12:
            _LATB8 = pinState;
            break;
        case 13:
            _LATB9 = pinState;
            break;
        case 15:
            _LATB12 = pinState;
            break;
        case 16:
            _LATB13 = pinState;
            break;
        case 17:
            _LATB14 = pinState;
            break;
        case 18:
            _LATB15 = pinState;
            break;
    }
}

#endif	/* HELPER_H */