#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Timer.h"
#include "lcd.h"
#include "stdio.h"
#include "objects.h"
#include "movement.h"
#include "open_interface.h"
#include <math.h>

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
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()



//variables


//
//
//
//    // manual calibration of left/right values
//    right_calibration_value = 316750;
//    left_calibration_value = 1298500;
//
//    cyBOT_Scan_t scan;
//
//
//while(1){
//    char c = cyBot_getByte(); //blocks until key is pressed
//    lcd_printf("Got a %c", c); //show recieved char on LCD
//    cyBot_sendString("Got a key \n\r"); //sends message back to putty
//    if(c == ' '){
//        break; //
//    }
//
//    if(c=='m'){
//        //if m is pressed then scan is performed
//        //scans the arm in 2 degree increments and writes and prints data
//        int i=0;
//
//            struct object obj[10];
//            float currDist=250;
//            float prevDist=250;
//            int objCounter = 0;
//            bool inObject = false;
//            float objectDistSum = 0;
//            int objectScanCount = 0;
//
//            cyBot_sendString("\n\r=== Starting Scan ===\n\r");
//
//            int imax = 182;
//
//            cyBOT_Scan(0, &scan);
////            prevDist = currDist;
//
//            prevDist = scan.sound_dist;
//            currDist = scan.sound_dist;
//
//        for(i=1;i<imax;i += 1){//we already scanned 0
//            cyBOT_Scan(i, &scan);
//
//            prevDist=currDist;
//            currDist = scan.sound_dist;
//
//            // DEBUG: Print every scan to see what's happening
//            cyBot_sendString("Angle: ");
//            cyBot_sendFloat(i);
//            cyBot_sendString(" | Dist: ");
//            cyBot_sendFloat(currDist);
//            cyBot_sendString(" | Diff: ");
//            cyBot_sendFloat(prevDist - currDist);
//            cyBot_sendString(" | inObj: ");
//            cyBot_sendFloat(inObject ? 1 : 0);
//            cyBot_sendString("\n\r");
//
//            // Detect START of object: distance suddenly drops
//               if(!inObject && (prevDist - currDist > 15&& currDist < 100)) {  // Added currDist < 100 to ignore walls and noise
//                   inObject = true;
//                   obj[objCounter].startAngle = i;
//                   objectDistSum = currDist;
//                   objectScanCount = 1;
//
//                   cyBot_sendString(">>> OBJECT START at angle ");
//                   cyBot_sendFloat(i);
//                   cyBot_sendString(" (distance drop detected)\n\r");
//               }
//               // Still in object: accumulate distance
////               else if(inObject && fabs(currDist - prevDist) < 10) { //changed this to look at slope too
//               else if (inObject&& currDist < 200){
//                   objectDistSum += currDist;
//                   objectScanCount++;
//               }
//               // Detect END of object: distance suddenly increases
//               if(inObject && (currDist - prevDist > 15)) {
//                   obj[objCounter].endAngle = i - 1;
//                   obj[objCounter].distance = objectDistSum / objectScanCount;
//                   obj[objCounter].radialWidth = obj[objCounter].endAngle - obj[objCounter].startAngle;
//                   obj[objCounter].middleAngle = (obj[objCounter].startAngle + obj[objCounter].endAngle) / 2;
//
//                   if(obj[objCounter].radialWidth > 5) {
//                   cyBot_sendString("<<< OBJECT END at angle ");
//                   cyBot_sendFloat(i);
//                   cyBot_sendString(" (distance jump detected)\n\r");
//
//                   cyBot_sendString("\n--- Object #");
//                   cyBot_sendFloat(objCounter + 1);
//                   cyBot_sendString(" Summary ---\n\r");
//                   cyBot_sendString("  Start Angle: ");
//                   cyBot_sendFloat(obj[objCounter].startAngle);
//                   cyBot_sendString("\n\r  End Angle: ");
//                   cyBot_sendFloat(obj[objCounter].endAngle);
//                   cyBot_sendString("\n\r  Middle Angle: ");
//                   cyBot_sendFloat(obj[objCounter].middleAngle);
//                   cyBot_sendString("\n\r  Distance: ");
//                   cyBot_sendFloat(obj[objCounter].distance);
//                   cyBot_sendString(" cm\n\r");
//                   cyBot_sendString("  Radial Width: ");
//                   cyBot_sendFloat(obj[objCounter].radialWidth);
//                   cyBot_sendString(" degrees\n\r\n\r");
//
//
//
//                   objCounter++;
////                   inObject = false;
////                   objectDistSum = 0;
////                   objectScanCount = 0;
//               } else {
//                   cyBot_sendString("!!! Ignoring noise (width too small)\n\r");
//               }
//
//
//                   //reset
//                   inObject = false;
//                   objectDistSum = 0;
//                   objectScanCount = 0;
//           }
//               prevDist=currDist;
//               lcd_printf("Angle: %d\nDist: %.2f", i, scan.sound_dist);
//            }//end of for loop
//               // handling obj that extends end of scan also avoid background
//        if(inObject && (objectDistSum / objectScanCount) < 100) {
//            obj[objCounter].endAngle = imax - 1;
//            obj[objCounter].distance = objectDistSum / objectScanCount;
//            obj[objCounter].radialWidth = obj[objCounter].endAngle - obj[objCounter].startAngle;
//            obj[objCounter].middleAngle = (obj[objCounter].startAngle + obj[objCounter].endAngle) / 2.0;
//
//            if(obj[objCounter].radialWidth > 5) {
//                cyBot_sendString("<<< OBJECT END at scan limit\n\r");
//
//                cyBot_sendString("\n--- Object #");
//                cyBot_sendFloat(objCounter + 1);
//                cyBot_sendString(" Summary ---\n\r");
//                cyBot_sendString("  Start Angle: ");
//                cyBot_sendFloat(obj[objCounter].startAngle);
//                cyBot_sendString("\n\r  End Angle: ");
//                cyBot_sendFloat(obj[objCounter].endAngle);
//                cyBot_sendString("\n\r  Middle Angle: ");
//                cyBot_sendFloat(obj[objCounter].middleAngle);
//                cyBot_sendString("\n\r  Distance: ");
//                cyBot_sendFloat(obj[objCounter].distance);
//                cyBot_sendString(" cm\n\r");
//                cyBot_sendString("  Radial Width: ");
//                cyBot_sendFloat(obj[objCounter].radialWidth);
//                cyBot_sendString(" degrees\n\r\n\r");
//
//                objCounter++;
//            }
//        }
//
//        // Now find smallest object after scan is complete
//        cyBot_sendString("\n\r=== SCAN COMPLETE ===\n\r");
//
//        // debug output
//        cyBot_sendString("Total objects detected: ");
//        cyBot_sendFloat(objCounter);
//        cyBot_sendString("\n\r");
//        // now find smallest object after scan is complete
//        cyBot_sendString("\n\r=== SCAN COMPLETE ===\n\r");
//        if(objCounter > 0) {
//            int smallestIdx = 0;
//            int j;
//            for(j = 1; j < objCounter; j++) {
//                if(obj[j].radialWidth < obj[smallestIdx].radialWidth) {
//                    smallestIdx = j;
//                }
//            }
//
//            cyBot_sendString("*** SMALLEST OBJECT ***\n\r");
//            cyBot_sendString("Object #");
//            cyBot_sendFloat(smallestIdx + 1);
//            cyBot_sendString("\n\r  Radial Width: ");
//            cyBot_sendFloat(obj[smallestIdx].radialWidth);
//            cyBot_sendString(" degrees\n\r");
//            cyBot_sendString("  Middle Angle: ");
//            cyBot_sendFloat(obj[smallestIdx].middleAngle);
//            cyBot_sendString("\n\r  Distance: ");
//            cyBot_sendFloat(obj[smallestIdx].distance);
//            cyBot_sendString(" cm\n\r\n\r");
//
//            // Point servo to smallest object
//            cyBot_sendString("Pointing servo to smallest object...\n\r");
//            cyBOT_Scan(obj[smallestIdx].middleAngle, &scan);
//            cyBot_sendString("Done!\n\r");
//
//            lcd_printf("Angle: %d\nDist: %d", obj[smallestIdx].middleAngle, obj[smallestIdx].distance);



//            float smallobjectangle = obj[smallestIdx].middleAngle;
//            float smallobjectdist = obj[smallestIdx].distance;
//
//            // turn roomba towards smallest object and move forward
//            if(smallobjectangle < 90){ // object is to the right
//                float turnAngle = 90 - smallobjectangle;  // Calculate how much right of center
//                turn_right(sensor_data, turnAngle);
//                move_forward(sensor_data, (((smallobjectdist)*10) - 180));
//
//                cyBot_sendString("Note: Turned right ");
//                cyBot_sendFloat(turnAngle);
//                cyBot_sendString(" degrees and moved forward.\n\r");
//                lcd_printf("Turn R: %.1f\nMove: %.1f", turnAngle, (smallobjectdist*10 - 180)/10);
//
//            } else if(smallobjectangle > 90){ // object is to the left
//                float turnAngle = smallobjectangle - 90;  // Calculate how much left of center
//                turn_left(sensor_data, turnAngle);
//                move_forward(sensor_data, (((smallobjectdist)*10) - 180));
//
//                cyBot_sendString("Note: Turned left ");
//                cyBot_sendFloat(turnAngle);
//                cyBot_sendString(" degrees and moved forward.\n\r");
//                lcd_printf("Turn L: %.1f\nMove: %.1f", turnAngle, (smallobjectdist*10 - 180)/10);
//
//            } else { // object is straight ahead (angle ≈ 90)
//                move_forward(sensor_data, (((smallobjectdist)*10) - 180));
//
//                cyBot_sendString("Note: Object straight ahead, moved forward.\n\r");
//                lcd_printf("Straight\nMove: %.1f", (smallobjectdist*10 - 180)/10);
//            }


        }else{
            cyBot_sendString("No objects detected.\n\r");
        }
    } //end of if == m
}
oi_free(sensor_data);
}





//void cyBot_sendString(char str[]) {
//    int len = strlen(str);
//    int j;
//
//    // sends one char at a time
//    for(j = 0; j < len; j++){
//        cyBot_sendByte(str[j]);
//    }
//}
//

    void cyBot_sendFloat(float value) {
        char buffer[32];  // enough space for most floats

        // Convert float to string (2 decimal places � change as needed)
        snprintf(buffer, sizeof(buffer), "%.2f", value);

        cyBot_sendString(buffer);
    }

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
