#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include "stdio.h"
#include "objects.h"

/*
Author: Muhammad Raham Saleem. Reid Post
Section: C
Date: 2/9/2026

For Lab 3. Starter code for main CyBot scan calibration and sending strings between PuTTY and CyBot instead of just char's.
*/
void cyBot_sendString(char str[]);
void cyBot_sendFloat(float fl);
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



int main(void){
    // initialization
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    cyBot_uart_init();





//    while(1){
//      char x = cyBot_getByte();
//      lcd_printf("%c", x);
//
//      char str[] = {};
//      // sets length of string, sets str to desired string
//      sprintf(str, "Got %c", x);
//      cyBot_sendString(str);
//    }


    // manual calibration of left/right values
    right_calibration_value = 253750;
    left_calibration_value = 1240750;

    cyBOT_Scan_t scan;


while(1){
    char c = cyBot_getByte(); //blocks until key is pressed
    lcd_printf("Got a %c", c); //show recieved char on LCD
    cyBot_sendString("Got a key \n\r"); //sends message back to putty
    struct object obj[3];
    if(c=='m'){
        //if m is pressed then scan is performed
        //scans the arm in 2 degree increments and writes and prints data
        int i=0;
        int objCounter=0;

            float currDist=250;
            float prevDist=250;
        for(i=60;i<120;i++){
//            if(i==90){
//                lcd_printf("at 90");
//            }
            cyBOT_Scan(i, &scan);

            prevDist=currDist;
            currDist = scan.sound_dist;
            if(currDist < 250){

            //if(prevDist-currDist>30){
                obj[objCounter].startAngle=i;

//                lcd_printf("object detected");
                cyBot_sendString("\n object detected at ");
                cyBot_sendFloat(currDist);
                cyBot_sendString("\n\r");
                cyBot_sendString("Angle detected at \n\r");
                cyBot_sendFloat(i);
//                char angle[] = {};
//                angle = i;

            //}
                if(prevDist-currDist<-30){
                    cyBot_sendString("object ended \n\r");
                    obj[objCounter].endAngle=i;


                    objCounter++;
                }
                if(objCounter>0){
                cyBot_sendString("Object ");
                cyBot_sendFloat(objCounter);
                cyBot_sendString("\nstartAngle:");
                cyBot_sendFloat(obj[objCounter-1].startAngle);
                cyBot_sendString("\n\r");
                cyBot_sendString("endAngle:");
                cyBot_sendFloat(obj[objCounter-1].endAngle);
                cyBot_sendString("\n\r");
                char scanDistance[32] = {};
                sprintf(scanDistance, "%f\n\r", scan.sound_dist);
                 cyBot_sendString(scanDistance);
                 //            char angle[] ={};
//            sprintf(angle, "%i\t",i);
//            cyBot_sendString(angle);
//
//            char scanDistance[] = {};
//            sprintf(scanDistance, "%f\n\r", scan.sound_dist);
//                    cyBot_sendString(scanDistance);
        }
            lcd_printf("angle: %d\t\nIR: %d\tDist: %.2f",i , scan.IR_raw_val, scan.sound_dist);
    }
}
}
}


}

//    int i;
//    cyBOT_Scan_t scan;
//    cyBot_sendString("Degrees\tPING Distance (cm)\n");

////     sweep scans (0-180 degrees) and prints angle and distance from detected objects
//    for(i = 0; i < 180; i+=2){
//        cyBOT_Scan(i, &scan);
//
//        char angle[] = {};
//        sprintf(angle, "%i\t", i);
//        cyBot_sendString(angle);
//
//        char scanDistance[] = {};
//        sprintf(scanDistance, "%f\n\r", scan.sound_dist);
//        cyBot_sendString(scanDistance);
//    }
//}



void cyBot_sendString(char str[]) {
    int len = strlen(str);
    int j;

    // sends one char at a time
    for(j = 0; j < len; j++){
        cyBot_sendByte(str[j]);
    }
}


    void cyBot_sendFloat(float value) {
        char buffer[32];  // enough space for most floats

        // Convert float to string (2 decimal places — change as needed)
        snprintf(buffer, sizeof(buffer), "%.2f", value);

        cyBot_sendString(buffer);
    }
