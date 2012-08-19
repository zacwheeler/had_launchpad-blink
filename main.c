/*
Copyright (c) 2010 - Mike Szczys

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include <msp430.h>
#include <signal.h>


#define     LED0                  BIT0
#define     LED1                  BIT6
#define     LED_DIR               P1DIR
#define     LED_OUT               P1OUT


void initLEDs(void) {
  LED_DIR |= LED0 | LED1;	//Set LED pins as outputs
  LED_OUT |= LED0 | LED1;	//Turn on both LEDs
}


int main(void) {

  WDTCTL = WDTPW | WDTHOLD;	// Stop WDT
  /*Halt the watchdog timer
      According to the datasheet the watchdog timer
      starts automatically after powerup. It must be
      configured or halted at the beginning of code
      execution to avoid a system reset. Furthermore,
      the watchdog timer register (WDTCTL) is
      password protected, and requires the upper byte
      during write operations to be 0x5A, which is the
      value associated with WDTPW.*/

  initLEDs();		//Setup LEDs

  BCSCTL3 |= LFXT1S_2;	//Set ACLK to use internal VLO (12 kHz clock)

  TACTL = TASSEL_1 | MC_1;	//Set TimerA to use auxiliary clock in UP mode
  TACCTL0 = CCIE;	//Enable the interrupt for TACCR0 match
  TACCR0 = 11999;	/*Set TACCR0 which also starts the timer. At
				12 kHz, counting to 12000 should output
				an LED change every 1 second. Try this
				out and see how inaccurate the VLO can be */
			
  WRITE_SR(GIE);	//Enable global interrupts

  while(1) {
	//Loop forever, interrupts take care of the rest
  }
}

interrupt(TIMER0_A0_VECTOR) TIMER0A0_ISR(void) {
  LED_OUT ^= (LED0 | LED1);	//Toggle both LEDs
}

