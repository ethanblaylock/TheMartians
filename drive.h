#ifndef DRIVE_H
#define	DRIVE_H

#include "xc.h" // include processor files - each processor file is guarded.  
#include "math.h"

#pragma config FNOSC = FRC

#define FOCS 8000000
#define FCY FOCS/2


#define MOTOR_PWM_1 4
#define MOTOR_DIR_1 12

#define MOTOT_PWM_2 5
#define MOTOR_DIR_2 13

#define PWM_FREQUENCY 400

#define STEPS_PER_INCH 18.182
#define STEPS_PER_ROTATION 484

#define STEP_MODE 0.5

#define TOTAL_STATES 6

static int total_steps = 0;
static int steps_needed = 0;
static int current_state = 0;

/**
 * Creates a timer given the following parameters.
 * @param timerNumber Specifies which of 5 timers is to be used.
 * @param periodValue Specifies the period for the timer to reset
 * @param prescale Defaults to 1. Accepts 1,8,64, or 256 as a prescale.
 */
void createTimer(int timerNumber, int periodValue, int prescale) {
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

/**
 * Configure a certain PWM to start running
 * @param ocNum the specific OC to turn on
 * @param period the period in Hz
 * @param dutyCycle the duty cycle as a fraction of 1
 */
void configurePWM(int pinNum, double period, double dutyCycle) {
    int value = round((1/period)*FCY) - 1;
    int dutyValue = round(dutyCycle*value);
    
    switch(pinNum) {
        case 14:
            OC1CON1 = 0;
            OC1CON2 = 0;
            OC1CON1bits.OCTSEL = 0b111; // System (peripheral) clock as timing source
            OC1CON2bits.SYNCSEL = 0x1F; // Select OC1 as synchronization source                       
            OC1CON2bits.OCTRIG = 0;     // Synchronizes with OC1 source instead of
            OC1CON1bits.OCM = 0b110;                                             
            OC1RS = value; // Period
            OC1R = dutyValue; // Duty cycle 
            break;
        case 4: 
            OC2RS = value; // Period
            OC2R = dutyValue; // Duty cycle 
            break;
        case 5:
            OC3RS = value; // Period
            OC3R = dutyValue; // Duty cycle 
            break;
    
    }
}

enum pinState {HIGH = 1, LOW = 0};
enum circle {CLOCKWISE = 1, COUNTERCLOCKWISE = 0};
enum direciotn {FORWARD = 0, REVERSE = 1};
enum state {ONE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5};

/**
 * Method to move to the next state 
 */
void incrementState() {
    if (current_state != TOTAL_STATES) {
        current_state++;
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

/**
 * Configures the given PWM and sets the proper output pin to drive the motor
 * @param motorPWMPin Which PWM is used
 * @param frequency The PWM frequency
 * @param motorDirPin The pinNum for the direction
 * @param direction High or Low
 */
void driveMotor(int motorPWMPin, int frequency, int motorDirPin, int direction) {
    configurePWM(motorPWMPin, frequency, 0.5);
    setOutputPin(motorDirPin, direction);
}

/**
 * Calculate the number of steps needed and turns that many steps
 * @param degrees The number of degrees to be turned
 * @param direction Clockwise or CounterClockwise
 */
void turnRobot(double degrees, int direction) {
    steps_needed = round((degrees/360.0)*STEPS_PER_ROTATION/STEP_MODE);
    
    driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, direction);
    driveMotor(MOTOT_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, direction ^ 1);
}

/**
 * Calculate the steps needed to drive the robot and drives straight
 * @param distance The distance in inches to be traveled
 * @param direction Forward or Reverse
 */
void driveStraight (double distance, int direction) {
    steps_needed = round(distance*STEPS_PER_INCH/STEP_MODE);
    
    driveMotor(MOTOR_PWM_1, PWM_FREQUENCY, MOTOR_DIR_1, direction);
    driveMotor(MOTOT_PWM_2, PWM_FREQUENCY, MOTOR_DIR_2, direction);
}

/**
 * Stops the robot by turning off the output compare modules
 */
void stopRobot() {
    OC2CON1bits.OCM = 0; 
    OC3CON1bits.OCM = 0; 
}

/**
 * This is the interrupt for OC2 which is pin 4
 * It counts steps and checks if we have hit the number of steps needed,
 * if so it moves to the next state
 */
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void) {
    _OC2IF = 0; // Clear flag
    total_steps++;
    if (total_steps > steps_needed) {
        incrementState();
        total_steps = 0;
    }
    
}

/*
void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(void) {
    _OC3IF = 0; // Clear flag
    total_steps3++;
    if (total_steps3 > steps_needed) {
        
        stopRobot();
        total_steps3 = 0;
    }
}
 */

/**
 * Sets up the robot with the proper registers
 */
void setup() {
    CLKDIVbits.RCDIV = 0;
            
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111; // System (peripheral) clock as timing source
    OC2CON2bits.SYNCSEL = 0x1F; // Select OC1 as synchronization source                       
    OC2CON2bits.OCTRIG = 0;     // Synchronizes with OC1 source instead of
    OC2CON1bits.OCM = 0b110;
    
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111; // System (peripheral) clock as timing source
    OC3CON2bits.SYNCSEL = 0x1F; // Select OC1 as synchronization source                       
    OC3CON2bits.OCTRIG = 0;     // Synchronizes with OC1 source instead of
    OC3CON1bits.OCM = 0b110;                                             
            
    _OC2IP = 4;
    _OC2IE = 1;
    _OC2IF = 0;
    
    /*
    _OC3IP = 4;
    _OC3IE = 1;
    _OC3IF = 0;
     */
    
    _TRISB9 = 0;
    _TRISB8 = 0;
    
    current_state = 1;
}



#endif	//DRIVE_H
