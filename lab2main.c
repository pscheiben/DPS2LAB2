
    //***************************************************************************************
    //  Description:  Software to demonstrate interrupt based digital input on  push buttons on P5.6                
    //****************************************************************************
    #include <msp430.h>

    void main(void)
    {
        volatile long int i=0;

        // Stop WDT
        WDTCTL = WDTPW + WDTHOLD;   //stop watchdog clk
        PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode

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

        // Set-up the interrupt for P5.6 (SW1)
        // P5.6 Hi/lo edge (falling edge)
        P5IES |= BIT6;
        // P5.6 IFG cleared
        P5IFG &= ~BIT6;
        // P5.6 Interrupt Enable
        P5IE |= BIT6;

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
    #pragma vector=PORT5_VECTOR  //attaches function my_Port5_ISR to P5 interrupt
    __interrupt void my_Port5_ISR(void)
    {
     // toggle LED 1 (P1.0)
     P1OUT ^= BIT0;
     // interrupt serviced, clear P5.6 interrupt flag
     P5IFG &= ~BIT6;
    }

