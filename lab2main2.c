//   Description:  Software to demonstrate interrupt based digital input
//   Implemented on FR5994
//******************************************************************************
#include <msp430.h>
void main(void)
{
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
    // Setup P1.0 as an output port
    P1SEL0 = P1SEL0 & ~BIT0;
    P1SEL1 = P1SEL1 & ~BIT0;
    P1DIR = P1DIR | BIT0;
    // Setup P5.6 as an input port
    P5SEL0 = P5SEL0 & ~BIT6;
    P5SEL1 = P5SEL1 & ~BIT6;
    P5DIR = P5DIR & ~BIT6;
    // Enable the pull up resistor and set as a pullup
    P5REN = P5REN | BIT6;
    P5OUT = P5OUT | BIT6;

    // P5.6 Hi/lo edge
    P5IES |= BIT6;
    // P5.6 IFG cleared
    P5IFG &= ~BIT6;
    // P5.6 Interrupt Enable
    P5IE |= BIT6;

    // Go to sleep: Enter LPM0, enable interrupts
    __bis_SR_register(LPM0_bits + GIE);

    __no_operation();    // This line is needed by the debugger
}

// Port 5 Interrupt Service Routine (ISR)
    #pragma vector=PORT5_VECTOR  //attaches function my_Port5_ISR to P5 interrupt
    __interrupt void my_Port5_ISR(void)
    {
     // toggle LED 1 (P1.0)
     P1OUT ^= BIT0;
     // interrupt serviced, clear P5.6 interrupt flag
     P5IFG &= ~BIT6;
    }
