//   Description:  Software to demonstrate interrupt based digital input
//   Implemented on FR5994
//   Lab Question 6. Modify the above code to enter LPM0, then:
//   toggle LED 1 if switch s1 is pressed (this is already done)
//   but also to toggle LED2 if switch S2 is pressed. This should be performed within the same ISR. Remember to configure LED2, and S2.
//******************************************************************************
#include <msp430.h>
void main(void)
{
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
    // Setup P1.0 and P1.1 as an output port
    P1SEL0 = P1SEL0 & ~(BIT0+BIT1);
    P1SEL1 = P1SEL1 & ~(BIT0+BIT1);
    P1DIR = P1DIR | (BIT0+BIT1);
    P1OUT = P1OUT & ~(BIT0+BIT1);
    // Setup P5.6(SW1) and P5.5(SW2) as an input port
    P5SEL0 = P5SEL0 & ~(BIT6+BIT5);
    P5SEL1 = P5SEL1 & ~(BIT6+BIT5);
    P5DIR = P5DIR & ~(BIT6+BIT5);
    // Enable the pull up resistors and set as a pullup
    P5REN = P5REN | (BIT6+BIT5);
    P5OUT = P5OUT | (BIT6+BIT5);

    // P5.6 and P5.5 Hi/lo edge
    P5IES |= (BIT6+BIT5);
    // P5.6 and P5.5 IFG cleared
    P5IFG &= ~(BIT6+BIT5);
    // P5.6 and P5.5 Interrupt Enable
    P5IE |= (BIT6+BIT5);

    // Go to sleep: Enter LPM0, enable interrupts
    __bis_SR_register(LPM0_bits + GIE);

    __no_operation();    // This line is needed by the debugger
}

// Port 5 Interrupt Service Routine (ISR)
    #pragma vector=PORT5_VECTOR  //attaches function my_Port5_ISR to P5 interrupt
    __interrupt void my_Port5_ISR(void)
    {

    if(P5IFG&BIT6){
     P1OUT ^= BIT0; // toggle LED 1 (P1.0)

     P5IFG &= ~(BIT6); // interrupt serviced, clear P5.6 interrupt flag
     }
     else{
        P1OUT ^= BIT1; // toggle LED 2 (P1.1)
        P5IFG &= ~(BIT5); // interrupt serviced, clear P5.5 interrupt flag
        }
     }

