#ifndef SETUP_H
#define	SETUP_H

#include "xc.h" 
#include <stdbool.h>

#pragma config FNOSC = FRC

#define FOCS 8000000
#define FCY FOCS/2

#define PWM_FREQUENCY 400

#define ANALOG_INPUTS 6
#define ANALOG_PINS 0b0011100000010011

enum line_state {START, FOLLOW_LINE, NAVIGATE_CANYON};
enum line_state current_state = START; // Sets current_state
static bool has_turned = false; // Whether the robot has turned in the canyon
static double chosen_frequency = PWM_FREQUENCY; // The frequency that drive the motors
static int stripes_detected = 0;
static bool slow_down_flag = false;

// This function configures the A/D to read from a
// single channel in auto conversion mode.
void configAD(void) {
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config

    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 1;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Does not scan inputs
                  // specified in AD1CSSx registers (instead
                  // uses channels specified by CH0SA bits in
                  // AD1CHS register) -- Selecting '0' here
                  // probably makes writing to the AD1CSSL
                  // register unnecessary.
    _SMPI = ANALOG_INPUTS - 1;    
                  // AD1CON2<6:2> -- Number of Analog signals to be sampled
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only

    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 1;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0x3F; // AD1CON3<7:0> -- A/D period TAD = 64*TCY (kind of like a prescaler for A/D, slower conversion rate, less power)

    // AD1CHS register
    _CH0NA = 0;   // AD1CHS<7:5> -- Use VDD as negative input
    //_CH0SA = 0b11110; // AD1CHS<4:0> -- Use AN10 as positive input

    // AD1CSSL register
    AD1CSSL = ANALOG_PINS;  // AD1CSSL<15:0> -- Selects
    //AD1CSSLbits.CSS11 = 1; Select specific pins or set entire register
    //AD1CSSLbits.CSS9 = 1;
    //AD1CSSLbits.CSS0 = 1;
    //AD1CSSLbits.CSS1 = 1;
    //AD1CSSLbits.CSS11 = 1;
    //AD1CSSLbits.CSS12 = 1;
                                   // which ADC pins to scan
                                   // -- see the CSCNA bits in AD1CON2

    _ADON = 1;    // AD1CON1<15> -- Turn on A/D

}

/**
 * Sets up the proper registers for the PWM
 */
void configPWM(void) {
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111; // System (peripheral) clock as timing source
    OC1CON2bits.SYNCSEL = 0x1F; // Select OC1 as synchronization source                       
    OC1CON2bits.OCTRIG = 0;     // Synchronizes with OC1 source instead of
    OC1CON1bits.OCM = 0b110; 
    
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
}

/**
 * Configures the proper interrupts
 */
void configInterrupts(void) {
    _OC2IP = 4;
    _OC2IE = 1;
    _OC2IF = 0;
    
    
    _OC3IP = 4;
    _OC3IE = 1;
    _OC3IF = 0;
    
    /*
    _CN6IE = 1;
    _CN6PUE = 0;
    _CNIP = 6;
    _CNIF = 0;
    _CNIE = 1;
    */
      
    _AD1IE = 1;
    _AD1IP = 6;
    _AD1IF = 0;
    
}


/**
 * Sets the given pins for input or output
 */
void configPins(void) {
    _TRISB9 = 0;
    _TRISB8 = 0;
    _TRISB15 = 0;
    
    _TRISB7 = 0;
    _TRISB14 = 0;
    _TRISA4 = 0;
}

/**
 * Sets up the robot with the proper registers
 */
void setup() {
    configAD();
    configPWM();
    configInterrupts();
    configPins();
    
    CLKDIVbits.RCDIV = 0;
            
    
    
}


#endif	/* SETUP_H */

