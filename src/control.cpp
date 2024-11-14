#include <Arduino.h>
#include "proj_types.h"
#include "robot.h"
#include "IRline.h"

struct Point {
    float x;
    float y;
};

Point points[] = {
    {0, 0},    // a 0
    {0.33, 0},   // b 1
    {0.38, 0.05},   // c 2
    {0.38, 0.255},// d 3
    {0.36, 0.275},// e 4
    {0.18, 0.13},  // f 5
    {0.15, 0.13},  // g 6
    {-0.03, 0.27},  // h 7
    {-0.05, 0.255},// i 8
    {-0.05, 0.05}    // j 9
};

const int numPoints = sizeof(points) / sizeof(points[0]);
const float threshold = 0.02; // Define a threshold distance to consider "near"


bool isNearPoint(float x, float y, Point point, float threshold) {
    float dx = x - point.x;
    float dy = y - point.y;
    return (dx * dx + dy * dy) <= (threshold * threshold);
}

// int getNearestPointIndex(float x, float y, Point points[], int numPoints, float threshold) {
//     for (int i = 0; i < numPoints; ++i) {
//         if (isNearPoint(x, y, points[i], threshold)) {
//             return i;
//         }
//     }
//     return -1; // No point is near
// }

void init_control(robot_t& robot)
{
  
}

extern IRLine_t IRLine;

void control(robot_t& robot)
{
    robot.tis = millis() - robot.tes;

    // Rules for the state evolution
    if(robot.state == 0 && robot.LastTouchSwitch && !robot.TouchSwitch) {
        robot.rel_s = 0;
        robot.setState(1);

    } else if (robot.state == 1 && robot.TouchSwitch) {
        robot.setState(2);

    } else if(robot.state == 2 && robot.tis > 100) {
        robot.rel_s = 0;
        robot.setState(3);

    } else if(robot.state == 3 && robot.rel_s < -0.12) {
        robot.rel_theta = 0;
        robot.setState(4);

    //} else if(robot.state == 4 && robot.rel_theta > radians(90) && IRLine.total > 1500) {
    } else if(robot.state == 4 && robot.rel_theta > radians(170)) {
      robot.setState(5);
      IRLine.crosses = 0;

    } else if(robot.state == 5 && IRLine.crosses >= 4) {
      robot.rel_s = 0;
      robot.rel_theta = 0;
      robot.setState(6);

    } else if(robot.state == 6 && robot.rel_theta < radians(-70) && IRLine.total > 1500) {
      robot.setState(7);

    } else if(robot.state == 7 && robot.tis > 2000) {
      IRLine.crosses = 0;
      robot.setState(8);

    } else if (robot.state == 202 && robot.tis > robot.T1) {
      robot.setState(200);
    }

    // Actions in each state
    if (robot.state == 0) {         // Robot Stoped            
      robot.solenoid_state = 0;
      robot.setRobotVW(0, 0);
    
    } else if (robot.state == 10) { //Test
      robot.setRobotVW(0.1, 0);      

    } else if (robot.state == 99) { // Q5
    // Reset robot pose
    robot.x = points[0].x;
    robot.y = points[0].y;
    robot.theta = 0;
    robot.setState(100);
} else if (robot.state == 100) { // Moving Straight
    robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
   robot.y = 0;
   robot.theta = radians(0);
   if(isNearPoint(robot.x, robot.y, points[1], threshold)) {
       robot.setState(101);
   }
} else if (robot.state == 101) { 
    robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
    if (isNearPoint(robot.x, robot.y, points[2], threshold)) {
      robot.setState(102);
    }
} else if (robot.state == 102) {  
    robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
    robot.x = 0.38;
    robot.theta = radians(90);
    if (isNearPoint(robot.x, robot.y, points[3], threshold)) {
      robot.setState(103);
    }
    
} else if (robot.state == 103) { // Curving Left
    robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                // + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                // + 2 * IRLine.IR_values[1] / 1024.0
                + 100 * IRLine.IR_values[0] / 1024.0;
  
    if (isNearPoint(robot.x, robot.y, points[4], threshold))
    {
      robot.setState(104);
    }
} else if (robot.state == 104)
{
  robot.v_req = 0.1;
  robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;

  robot.theta = radians(225);
  if (isNearPoint(robot.x, robot.y, points[5], threshold))
  {
    robot.setState(105);
  }
} else if (robot.state == 105)
{
  robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
  if(isNearPoint(robot.x, robot.y, points[6], threshold))
  {
    robot.setState(106);
  }
}
else if (robot.state == 106)
{
  robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
  robot.theta = radians(135);
  if (isNearPoint(robot.x, robot.y, points[7], threshold))
  {
    robot.setState(107);
  } 
}else if (robot.state == 107)
{
  robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
  if (isNearPoint(robot.x, robot.y, points[8], threshold))
  {
    robot.setState(108);
  }
}
else if (robot.state == 108)
{
  robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
  robot.theta = radians(270);
  robot.x = -0.05;
  if (isNearPoint(robot.x, robot.y, points[9], threshold))
  {
    robot.setState(109);
  }
}
else if (robot.state == 109)
{
  robot.v_req = 0.1;
    robot.w_req = - 10 * IRLine.IR_values[4] / 1024.0 
                // - 2 * IRLine.IR_values[3] / 1024.0
                + 0.9 * (IRLine.IR_values[2]/1024.0 - 0.1)
                + 2 * IRLine.IR_values[1] / 1024.0
                + 10 * IRLine.IR_values[0] / 1024.0;
  if (isNearPoint(robot.x, robot.y, points[0], threshold))
  {
    robot.setState(0);
  }
}
else if (robot.state == 199) {
    robot.v_req = 0.1;
    robot.w_req = 4 * IRLine.IR_values[4] / 1024.0 
                  + 2 * IRLine.IR_values[3] / 1024.0
                  - 2 * IRLine.IR_values[1] / 1024.0
                  - 4 * IRLine.IR_values[0] / 1024.0;
}
 else if (robot.state == 200) {
      robot.PWM_1 = 0;
      robot.PWM_2 = 0;

    } else if (robot.state == 201) {
      robot.PWM_1 = robot.PWM_1_req;
      robot.PWM_2 = robot.PWM_2_req;
    
    } else if (robot.state == 202) {
      robot.PWM_1 = robot.PWM_1_req;
      robot.PWM_2 = robot.PWM_2_req;      
    }
  
}
