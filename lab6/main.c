#include "cyBot_Scan.h"
#include "cyBot_uart.h"
//#include "Timer.h"
#include "lcd.h"
#include "stdio.h"
#include "objects.h"
#include "movement.h"
//#include "open_interface.h"
//#include <math.h>
//#include "button.h"
#include "cyBot_uart.h"
//#include "uart.h"
//#include "button.h"

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart-interrupt.h"


/*
Author: Muhammad Raham Saleem, Connor Crimmins
Section: C
Date: 2/24/2026

For Lab 3. Starter code for main CyBot scan calibration and sending strings between PuTTY and CyBot instead of just char's.
*/
void cyBot_sendString(char str[]);
void cyBot_sendFloat(float fl);
char uart_receive_nonblocking(void);
// calibration

//int main(void){
//    // initialization
//    timer_init();
//    lcd_init();
//    cyBOT_init_Scan(0b0111);
//    cyBot_uart_init();
//
//    cyBOT_SERVO_cal();
//    return 0;
//}


//lab 6
int main(void){
    // initialization
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
//    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
//    oi_init(sensor_data); // do this only once at start of main()
    uart_interrupt_init();



//variables

    // manual calibration of left/right values
    right_calibration_value = 238000;
    left_calibration_value = 1183000;

    cyBOT_Scan_t scan;


    while(1){
        if(command_flag){
            command_flag = 0;

            if(command_byte == 'g'){   // start scan
                int i=0;
                for(i = 0; i < 182; i++){
                    cyBOT_Scan(i, &scan);

                    if(command_flag){   // check if 's' pressed
                        command_flag = 0;
                        if(command_byte == 's')
                            break;  // stop scanning
                    }
                }
            }
        }
    }
}
