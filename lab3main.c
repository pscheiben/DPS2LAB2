//******************************************************************************
//   Description: Blink the LED on Port 1.0 using Watchdog interrupt
//   Implemented on FR5994
//******************************************************************************


#include <msp430.h>
void InitIOpins();
void main(void)
{
    // Stop WDT
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
    InitIOpins();

        // WDT source by ACLK, 1s
        WDTCTL = WDT_ADLY_1000;

        // Enable WDT interrupt
        SFRIE1 |= WDTIE;

        // Enable interrupts globally
        __enable_interrupt();

        // Go to sleep: Enter LPM0, enable interrupts
        __bis_SR_register(LPM0_bits + GIE);

        // This line is needed by the debugger
        __no_operation();
    }


#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    // Toggle P1.0
    P1OUT ^= BIT0;
    // Clear watchdog interrupt flag
    SFRIFG1 &= ~WDTIFG;
}

void InitIOpins()
{
  // Setup P1.0 and P1.1 as output pins
  P1SEL0 = P1SEL0 & ~(BIT1+BIT0);
  P1SEL1 = P1SEL1 & ~(BIT1+BIT0);
  P1DIR = P1DIR | (BIT1+BIT0);
  P1OUT = P1OUT & ~(BIT1+BIT0); //turn off LED1 and LED2
}

