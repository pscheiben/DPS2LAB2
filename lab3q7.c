//******************************************************************************
//   DEAD-MAN mechanism
//   Description: SW2 pressed to initiate train movement (LED2)
//   Stops the train (LED2) unless SW1 “dead-man” is pressed before 16s,
//   keep pressing and releasing SW1 repeatedly to keep train going
//   Author: Mario Mata
//******************************************************************************

//   Implemented on FR5994
//   Peter Scheibenhoffer

//******************************************************************************


#include <msp430.h>

#define MY_WDT_CONFIG WDTSSEL0+BIT1+BIT0  //ACLK source, 16s interval

void InitIOpins();
void main(void)
{
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
    InitIOpins();

    //wait here until SW2 pressed to enable system
       while(P5IN & BIT5); //empty loop, just active wait for S2

          //Activate LED2 (allowing the train movement)
          P1OUT = P1OUT | BIT1; //turn on LED2
          //Configure the Watchdog to 16-second reboot unless avoided
          WDTCTL = WDTPW + MY_WDT_CONFIG; /* check define for MY_WDT_CONFIG */
          // P5.6 (SW1) Interrupt Enable
          P5IE |= BIT6;
          // Enable interrupts globally
          __enable_interrupt();
          // Go to sleep: Enter LPM0, enable interrupts
          __bis_SR_register(LPM0_bits + GIE);
          // This line is needed by the debugger
          __no_operation();
      }

#pragma vector=PORT5_VECTOR  //attaches function my_Port5_ISR to P5 interrupt
__interrupt void my_Port5_ISR(void)
{
    // P5.6 IFG cleared
    P5IFG &= ~BIT6;
    P1OUT = P1OUT ^ BIT0; //toggle LED1
    // Clear watchdog count (avoid reboot!), keep WDT config
    // COMPLETE HERE THE MISSING INSTRUCTION
    WDTCTL = WDTPW + MY_WDT_CONFIG + WDTCNTCL;




}

void InitIOpins()
{
  // Setup P1.0 and P1.1 as output pins
  P1SEL0 = P1SEL0 & ~(BIT0+BIT1);
  P1SEL1 = P1SEL1 & ~(BIT0+BIT1);
  P1DIR = P1DIR | (BIT0+BIT1);
  P1OUT = BIT0; //turn on LED1 only
  // Setup P5.6 and P5.5 as an input port (for SW1 and 2)
  P5SEL0 = P5SEL0 & ~(BIT6+BIT5); //GPIO
  P5SEL1 = P5SEL1 & ~(BIT6+BIT5); //GPIO
  P5DIR = P5DIR & ~(BIT6+BIT5); //input
  // Enable the internal pull resistor and set as a pullup
  P5REN = P5REN | (BIT6+BIT5);  //enable internal pull resistor
  P5OUT = P5OUT | (BIT6+BIT5);  //selects to pull UP

  // Set-up the interrupt for P5.6 (SW1 only)
  // P5.6 Hi/lo edge (falling edge, pressing the switch)
  P5IES |= BIT6;
  // P2.6 IFG cleared
  P5IFG &= ~BIT6;
}



