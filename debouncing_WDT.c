
    //***************************************************************************************
    //  Description:  Software to demonstrate interrupt based digital input on push button on P5.5
    //  Lab Question 4. Change the above code so that LED 1 toggles when switch S2(P5.5) is released (instead of when S1 is pressed) using an interrupt
    //  Implemented on MSP-EXP430FR5994
    //  Debouncing by WDT interrupt
    //****************************************************************************
    #include <msp430.h>
    void initGPIOReg(void);
    void main(void)
    {

        // Stop WDT
        WDTCTL = WDTPW + WDTHOLD;   // stop watchdog clk
        PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
        volatile long int i=0;
        initGPIOReg();



        // Enable interrupts globally. This sets GIE bit
        _EINT();

        // Polling loop with software delay
        for(;;)  // infinite loop, equivalent to while(1)
        {
        // Toggle LED 2
        P1OUT ^= BIT1;
        // Spin round for n cycles
        for(i=0;i<30000;i++){;} //do nothing
        }
    }

    // Port 5 Interrupt Service Routine (ISR)
    #pragma vector=PORT5_VECTOR  //attaches function my_Port5_ISR to P6 interrupt
    __interrupt void my_Port5_ISR(void)
    {
     // toggle LED 1 (P1.0)
     P1OUT ^= BIT0;
     // WDT source by ACLK, 16ms
     WDTCTL = WDT_ADLY_1000;
     // Enable WDT interrupt
     SFRIE1 |= WDTIE;
     // P5.6 Interrupt Disable
     P5IE &= ~BIT6;
     // interrupt serviced, clear P5.6 interrupt flag
     P5IFG &= ~BIT6;
    }

#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    // P5.5 Interrupt Enable
    P5IE |= BIT6;
    // Clear watchdog interrupt flag
    SFRIFG1 &= ~WDTIFG;
    SFRIE1 &= ~WDTIE;
    WDTCTL = WDTPW + WDTHOLD;   // stop watchdog clk
    //during the interval any button press is ignored, clear P5.6 interrupt flag
    P5IFG &= ~BIT6;
}



void initGPIOReg(void)
{
    // Configure input/output pins
            // Setup P1.0 as an output pin (for LED1)
            P1SEL0 = P1SEL0 & ~BIT0;  //GPIO
            P1SEL1 = P1SEL1 & ~BIT0;  //GPIO
            P1DIR = P1DIR | BIT0;   //output direction
            // Setup P1.1 as an output pin (for LED2)
            P1SEL0 = P1SEL0 & ~BIT1;  //GPIO
            P1SEL1 = P1SEL1 & ~BIT1;  //GPIO
            P1DIR = P1DIR | BIT1;  //output
            // Setup P5.6 as an input port (for SW1)
            P5SEL0 = P5SEL0 & ~BIT6;  //GPIO
            P5SEL1 = P5SEL1 & ~BIT6; //GPIO
            P5DIR = P5DIR & ~BIT6; //input
            // Enable the internal pull resistor and set as a pullup
            P5REN = P5REN | BIT6;  //enable internal pull resistor
            P5OUT = P5OUT | BIT6;  //selects to pull UP

            // Set-up the interrupt for P5.5 (SW2)
            // P5.5 Hi/lo edge (rising edge)
            P5IES &= ~BIT6;
            // P5.5 IFG cleared
            P5IFG &= ~BIT6;
            // P5.5 Interrupt Enable
            P5IE |= BIT6;
}
