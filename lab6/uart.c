/*
*
*   uart.c
*
*
*
*   @author
*   @date
*/

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"

void uart_init(void){
	//TODO
  //enable clock to GPIO port B
  SYSCTL_RCGCGPIO_R |= 0x02;

  //enable clock to UART1
  SYSCTL_RCGCUART_R |= 0x02;

  //wait for GPIOB and UART1 peripherals to be ready
  while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
  while ((SYSCTL_PRUART_R & 0x02) == 0) {};

  //enable alternate functions on port B pins
  GPIO_PORTB_AFSEL_R |= 0x03;

  //enable digital functionality on port B pins
  GPIO_PORTB_DEN_R |= 0x03;

  //enable UART1 Rx and Tx on port B pins
  GPIO_PORTB_PCTL_R &= ~0xFF;
  GPIO_PORTB_PCTL_R |= 0x11;
  //UARTSysClk = 16 MHz, ClkDiv = 16, Baud Rate = 115,200
//  Calculate BRD:
//  BRD=16,000,000/(16×115,200)=8.6806
//  IBRD = integer part = 8
//  FBRD = integer(0.6806 × 64 + 0.5) = integer(44.06) = 44
  //
  //calculate baud rate
//  uint16_t iBRD =  (uint16_t)(8) ; //use equations
//  uint16_t fBRD = (uint16_t)(0.6806 × 64 + 0.5) ; //use equations


  //turn off UART1 while setting it up
  UART1_CTL_R &= ~0x01;

  //set baud rate
  //note: to take effect, there must be a write to LCRH after these assignments
//  UART1_IBRD_R = iBRD;
//  UART1_FBRD_R = fBRD;
  UART1_IBRD_R = 8;
  UART1_FBRD_R = 44;

  //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
  //note: this write to LCRH must be after the BRD assignments
  UART1_LCRH_R =  0x60;

  //use system clock as source
  //note from the datasheet UARTCCC register description:
  //field is 0 (system clock) by default on reset
  //Good to be explicit in your code
  UART1_CC_R = 0x0;

  //re-enable UART1 and also enable RX, TX (three bits)
  //note from the datasheet UARTCTL register description:
  //RX and TX are enabled by default on reset
  //Good to be explicit in your code
  //Be careful to not clear RX and TX enable bits
  //(either preserve if already set or set them)
  UART1_CTL_R|= (0x01 | 0x100 | 0x200);

}

void uart_sendChar(char data){

    while(UART1_FR_R & 0x20){};   // Wait while TXFF (Transmit FIFO full)

    UART1_DR_R = data;
}

char uart_receive(void){

    while(UART1_FR_R & 0x10){};   // Wait while RXFE (Receive FIFO empty)

    return (char)(UART1_DR_R & 0xFF);
}

//char uart_nonblocking_receive(void){
//
//    while(U){};   // Wait while RXFE (Receive FIFO empty)
//
//    return (char)(UART1_DR_R & 0xFF);
//}
// Returns 1 if data is available, 0 otherwise
int uart_available(void) {
    return !(UART1_FR_R & 0x10); // Returns 1 if RXFE is 0 (not empty)
}

// Revised function
char uart_receive_nonblocking(void) {
    if(uart_available()) {
        return (char)(UART1_DR_R & 0xFF);
    }
    return 0; // Return 0 if nothing received
}
void uart_sendStr(const char *data){

    int i = 0;

    while(data[i] != '\0'){
        uart_sendChar(data[i]);
        i++;
    }
}
