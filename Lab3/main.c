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


//variables





    // manual calibration of left/right values
    right_calibration_value = 290500;
    left_calibration_value = 1209250;

    cyBOT_Scan_t scan;


while(1){
    char c = cyBot_getByte(); //blocks until key is pressed
    lcd_printf("Got a %c", c); //show recieved char on LCD
    cyBot_sendString("Got a key \n\r"); //sends message back to putty
    if(c=='m'){
        //if m is pressed then scan is performed
        //scans the arm in 2 degree increments and writes and prints data
        int i=0;

            struct object obj[10];
            float currDist=250;
            float prevDist=250;
            int objCounter = 0;
            bool inObject = false;
            float objectDistSum = 0;
            int objectScanCount = 0;

            cyBot_sendString("\n\r=== Starting Scan ===\n\r");

        for(i=60;i<120;i++){

            cyBOT_Scan(i, &scan);

            prevDist=currDist;
            currDist = scan.sound_dist;

            // DEBUG: Print every scan to see what's happening
            cyBot_sendString("Angle: ");
            cyBot_sendFloat(i);
            cyBot_sendString(" | Dist: ");
            cyBot_sendFloat(currDist);
            cyBot_sendString(" | Diff: ");
            cyBot_sendFloat(prevDist - currDist);
            cyBot_sendString(" | inObj: ");
            cyBot_sendFloat(inObject ? 1 : 0);
            cyBot_sendString("\n\r");

            // Detect START of object: distance suddenly drops
               if(!inObject && (prevDist - currDist > 50)) {
                   inObject = true;
                   obj[objCounter].startAngle = i;
                   objectDistSum = currDist;
                   objectScanCount = 1;

                   cyBot_sendString(">>> OBJECT START at angle ");
                   cyBot_sendFloat(i);
                   cyBot_sendString(" (distance drop detected)\n\r");
               }
               // Still in object: accumulate distance
               else if(inObject && currDist < 250) {
                   objectDistSum += currDist;
                   objectScanCount++;
               }
               // Detect END of object: distance suddenly increases
               if(inObject && (prevDist - currDist < -30)) {
                   obj[objCounter].endAngle = i - 2;
                   obj[objCounter].distance = objectDistSum / objectScanCount;
                   obj[objCounter].radialWidth = obj[objCounter].endAngle - obj[objCounter].startAngle;
                   obj[objCounter].middleAngle = (obj[objCounter].startAngle + obj[objCounter].endAngle) / 2;

                   if(obj[objCounter].radialWidth > 3) {
                   cyBot_sendString("<<< OBJECT END at angle ");
                   cyBot_sendFloat(i);
                   cyBot_sendString(" (distance jump detected)\n\r");

                   cyBot_sendString("\n--- Object #");
                   cyBot_sendFloat(objCounter + 1);
                   cyBot_sendString(" Summary ---\n\r");
                   cyBot_sendString("  Start Angle: ");
                   cyBot_sendFloat(obj[objCounter].startAngle);
                   cyBot_sendString("\n\r  End Angle: ");
                   cyBot_sendFloat(obj[objCounter].endAngle);
                   cyBot_sendString("\n\r  Middle Angle: ");
                   cyBot_sendFloat(obj[objCounter].middleAngle);
                   cyBot_sendString("\n\r  Distance: ");
                   cyBot_sendFloat(obj[objCounter].distance);
                   cyBot_sendString(" cm\n\r");
                   cyBot_sendString("  Radial Width: ");
                   cyBot_sendFloat(obj[objCounter].radialWidth);
                   cyBot_sendString(" degrees\n\r\n\r");



                   objCounter++;
//                   inObject = false;
//                   objectDistSum = 0;
//                   objectScanCount = 0;
               } else {
                   cyBot_sendString("!!! Ignoring noise (width too small)\n\r");
               }
                   //reset
                   inObject = false;
                   objectDistSum = 0;
                   objectScanCount = 0;
           }
               lcd_printf("Angle: %d\nDist: %.2f", i, scan.sound_dist);
        }//end of for loop
               //

//            //if(prevDist-currDist>30){
//                obj[objCounter].startAngle=i;
//
////                lcd_printf("object detected");
//                cyBot_sendString("\n object detected at ");
//                cyBot_sendFloat(currDist);
//                cyBot_sendString("\n\r");
//                cyBot_sendString("Angle detected at \n\r");
//                cyBot_sendFloat(i);
////                char angle[] = {};
////                angle = i;
//
//            //}
//                if(prevDist-currDist<-30){
//                    cyBot_sendString("object ended \n\r");
//                    obj[objCounter].endAngle=i;
//
//
//                    objCounter++;
//                }
//                if(objCounter>0){
//                cyBot_sendString("Object ");
//                cyBot_sendFloat(objCounter);
//                cyBot_sendString("\nstartAngle:");
//                cyBot_sendFloat(obj[objCounter-1].startAngle);
//                cyBot_sendString("\n\r");
//                cyBot_sendString("endAngle:");
//                cyBot_sendFloat(obj[objCounter-1].endAngle);
//                cyBot_sendString("\n\r");
//                char scanDistance[32] = {};
//                sprintf(scanDistance, "%f\n\r", scan.sound_dist);
//                 cyBot_sendString(scanDistance);
//                 //            char angle[] ={};
////            sprintf(angle, "%i\t",i);
////            cyBot_sendString(angle);
////
////            char scanDistance[] = {};
////            sprintf(scanDistance, "%f\n\r", scan.sound_dist);
////                    cyBot_sendString(scanDistance);
        // now find smallest object after scan is complete
        cyBot_sendString("\n\r=== SCAN COMPLETE ===\n\r");
        if(objCounter > 0) {
            int smallestIdx = 0;
            int j;
            for(j = 1; j < objCounter; j++) {
                if(obj[j].radialWidth < obj[smallestIdx].radialWidth) {
                    smallestIdx = j;
                }
            }

            cyBot_sendString("*** SMALLEST OBJECT ***\n\r");
            cyBot_sendString("Object #");
            cyBot_sendFloat(smallestIdx + 1);
            cyBot_sendString("\n\r  Radial Width: ");
            cyBot_sendFloat(obj[smallestIdx].radialWidth);
            cyBot_sendString(" degrees\n\r");
            cyBot_sendString("  Middle Angle: ");
            cyBot_sendFloat(obj[smallestIdx].middleAngle);
            cyBot_sendString("\n\r  Distance: ");
            cyBot_sendFloat(obj[smallestIdx].distance);
            cyBot_sendString(" cm\n\r\n\r");

            // Point servo to smallest object
            cyBot_sendString("Pointing servo to smallest object...\n\r");
            cyBOT_Scan(obj[smallestIdx].middleAngle, &scan);
            cyBot_sendString("Done!\n\r");
        }else{
            cyBot_sendString("No objects detected.\n\r");
        }
    } //end of if == m
}
}
//}
//}
//
//
//}

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
