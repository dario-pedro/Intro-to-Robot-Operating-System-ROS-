//
// TODO class header
//

#include "tr5_kinematics/Kinematics.hpp"
#include <cmath>
#include <math.h>
using namespace TR5;

#define PI 3.14159265

Kinematics::Kinematics()
{
    serviceFK =  nh.advertiseService("/do_fk", &Kinematics::do_fk, this);
    serviceIK =  nh.advertiseService("/do_ik", &Kinematics::do_ik, this);
    serviceINT =  nh.advertiseService("/do_int", &Kinematics::do_int, this);
}

float   tr5shoulder_pan_joint=0,
        tr5shoulder_lift_joint=0,
        tr5elbow_joint=0,
        tr5wrist_1_joint=0,
        tr5wrist_2_joint=0,
        tr5gripper_1_joint=0,
        tr5gripper_2_joint=0;


void Kinematics::run()
{
    ros::spin();
}


bool Kinematics::do_fk ( tr5_kinematics::DoForwardKinematic::Request  &req,
                         tr5_kinematics::DoForwardKinematic::Response &res )
{
    float working_angle;

    // upperArm
    working_angle = req.joints.position[1];
    res.pose.position.x += upper_arm_length*cos(working_angle);
    res.pose.position.y += upper_arm_length*cos(working_angle);
    res.pose.position.z += upper_arm_length*sin(working_angle);

    //foreArm
    working_angle+=req.joints.position[2];
    res.pose.position.x += fore_arm*cos(working_angle);
    res.pose.position.y += fore_arm*cos(working_angle);
    res.pose.position.z += fore_arm*sin(working_angle);

    //wrist
    working_angle+=req.joints.position[3];
    res.pose.position.x += wrist_length*cos(working_angle);
    res.pose.position.y += wrist_length*cos(working_angle);
    res.pose.position.z += wrist_length*sin(working_angle);

    //finger
    res.pose.position.x += gripperPLUSfingerlength*cos(working_angle);
    res.pose.position.y += gripperPLUSfingerlength*cos(working_angle);
    res.pose.position.z += gripperPLUSfingerlength*sin(working_angle);

    // base
    res.pose.position.x = res.pose.position.x*cos(req.joints.position[0]);
    res.pose.position.y = res.pose.position.y*sin(req.joints.position[0]);

    // shoulder
    res.pose.position.z += base_heigth + shoulder_heigth;
    return true;
}



bool Kinematics::do_ik ( tr5_kinematics::DoInverseKinematic::Request  &req,
                         tr5_kinematics::DoInverseKinematic::Response &res )
{
  double x = req.pose.position.x;
  double y = req.pose.position.y;
  double z = req.pose.position.z;

    // TODO TR5 CALCULLUS

  double a2 = 0.200;
  double a3 = 0.130;
  double d5 = 0.130;
  double d1 = 0.275;

  double nx = 0;
  double ny = 0;
  double nz = 0;

  double sx = 0;
  double sy = 0;
  double sz = 0;

  double ax = 0;
  double ay = 0;
  double az = 0;


  tr5shoulder_pan_joint = atan2(y, x);

  if(tr5shoulder_pan_joint<-1.40) tr5shoulder_pan_joint = -1.40;
  else if(tr5shoulder_pan_joint>1.40) tr5shoulder_pan_joint = 1.40;

  double teta234 = atan2(az, (ax*cos(tr5shoulder_pan_joint) + ay*sin(tr5shoulder_pan_joint)));

  double aux_x = (pow(x*cos(tr5shoulder_pan_joint) + y*sin(tr5shoulder_pan_joint) - d5*cos(teta234), 2) + pow(z - d1 - d5*sin(teta234), 2) - a2*a2 - a3*a3) / (2.0*a2*a3);

  double square_root = 1 - pow(aux_x, 2);

  if (square_root < 0)
    square_root = 0;

  double aux_y = sqrt(square_root);

  tr5elbow_joint = atan2(aux_y, aux_x);

  if (tr5elbow_joint > 0)
    tr5elbow_joint = atan2(-aux_y, aux_x);

  if(tr5elbow_joint<-1.75) tr5elbow_joint = -1.75;
  else if(tr5elbow_joint>0) tr5elbow_joint = 0;

  aux_x = (a2 + a3*cos(tr5elbow_joint))*(-d5*cos(teta234) + cos(tr5shoulder_pan_joint)*x + sin(tr5shoulder_pan_joint)*y) + a3*sin(tr5elbow_joint)*(-d1 + z - d5*sin(teta234));

  aux_y = (a2 + a3*cos(tr5elbow_joint))*(-d1 + z - d5*sin(teta234)) - a3*sin(tr5elbow_joint)*(cos(tr5shoulder_pan_joint)*x + sin(tr5shoulder_pan_joint)*y - d5*cos(teta234));

  tr5shoulder_lift_joint = atan2(aux_y, aux_x);

  float robot_x=0,
  robot_y=0,
  robot_z=0;


  // upperArm
  float working_angle = tr5shoulder_lift_joint;
  robot_x += upper_arm_length*cos(working_angle);
  robot_y += upper_arm_length*cos(working_angle);
  robot_z += upper_arm_length*sin(working_angle);

  //foreArm
  working_angle += tr5elbow_joint;
  robot_x += fore_arm*cos(working_angle);
  robot_y += fore_arm*cos(working_angle);
  robot_z += fore_arm*sin(working_angle);

  //wrist
  working_angle += tr5wrist_1_joint;
  robot_x += wrist_length*cos(working_angle);
  robot_y += wrist_length*cos(working_angle);
  robot_z += wrist_length*sin(working_angle);

  //finger
  robot_x += gripperPLUSfingerlength*cos(working_angle);
  robot_y += gripperPLUSfingerlength*cos(working_angle);
  robot_z += gripperPLUSfingerlength*sin(working_angle);

  // base
  robot_x = robot_x*cos(tr5shoulder_pan_joint);
  robot_y = robot_y*sin(tr5shoulder_pan_joint);

  // shoulder
  robot_z += base_heigth + shoulder_heigth;

  if(( std::abs(robot_x - x)<0.03) &&
  ( std::abs(robot_y - y)<0.03) &&
  ( std::abs(robot_z - z)<0.03)  ){
    tr5gripper_1_joint+=0.047;
    tr5gripper_2_joint+=0.047;
  }
  else {
    tr5gripper_1_joint-=0.047;
    tr5gripper_2_joint-=0.047;
  }


  if(tr5shoulder_lift_joint<-0.52) tr5shoulder_lift_joint = -0.52;
  else if(tr5shoulder_lift_joint>1.22) tr5shoulder_lift_joint = 1.22;

  tr5wrist_1_joint = teta234 - tr5shoulder_lift_joint - tr5elbow_joint;

  if(tr5wrist_1_joint<-1.75) tr5wrist_1_joint = -1.75;
  else if(tr5wrist_1_joint>1.75) tr5wrist_1_joint = 1.75;

   if(tr5wrist_2_joint<-1.75) tr5wrist_2_joint = -1.75;
   else if(tr5wrist_2_joint>1.75) tr5wrist_2_joint = 1.75;

   if(tr5gripper_1_joint<0) tr5gripper_1_joint = 0;
   else if(tr5gripper_1_joint>0.47) tr5gripper_1_joint = 0.47;

   if(tr5gripper_2_joint<0) tr5gripper_2_joint = 0;
   else if(tr5gripper_2_joint>0.47) tr5gripper_2_joint = 0.47;


    // PUSHBACK VALUES
    res.joints.name.clear();
    res.joints.name.push_back( "tr5shoulder_pan_joint"  );
    res.joints.position.push_back(tr5shoulder_pan_joint);
    res.joints.name.push_back( "tr5shoulder_lift_joint" );
    res.joints.position.push_back(tr5shoulder_lift_joint);
    res.joints.name.push_back( "tr5elbow_joint"         );
    res.joints.position.push_back(tr5elbow_joint);
    res.joints.name.push_back( "tr5wrist_1_joint"       );
    res.joints.position.push_back(tr5wrist_1_joint);
    res.joints.name.push_back( "tr5wrist_2_joint"       );
    res.joints.position.push_back(tr5wrist_2_joint);
    res.joints.name.push_back( "tr5gripper_1_joint"     );
    res.joints.position.push_back(tr5gripper_1_joint);
    res.joints.name.push_back( "tr5gripper_2_joint"     );
    res.joints.position.push_back(tr5gripper_2_joint);
    res.joints.header.stamp = ros::Time::now();
    return true;
}

bool Kinematics::do_int ( tr5_kinematics::DoInverseKinematic::Request  &req,
                         tr5_kinematics::DoInverseKinematic::Response &res ){


for(int i = 0 ; i<10;i++){
       float robot_x=0,
       robot_y=0,
       robot_z=0;


       // TODO TR5 CALCULLUS

       // upperArm
       float working_angle = tr5shoulder_lift_joint;
       robot_x += upper_arm_length*cos(working_angle);
       robot_y += upper_arm_length*cos(working_angle);
       robot_z += upper_arm_length*sin(working_angle);

       //foreArm
       working_angle += tr5elbow_joint;
       robot_x += fore_arm*cos(working_angle);
       robot_y += fore_arm*cos(working_angle);
       robot_z += fore_arm*sin(working_angle);

       //wrist
       working_angle += tr5wrist_1_joint;
       robot_x += wrist_length*cos(working_angle);
       robot_y += wrist_length*cos(working_angle);
       robot_z += wrist_length*sin(working_angle);

       //finger
       robot_x += gripperPLUSfingerlength*cos(working_angle);
       robot_y += gripperPLUSfingerlength*cos(working_angle);
       robot_z += gripperPLUSfingerlength*sin(working_angle);

       // base
       robot_x = robot_x*cos(tr5shoulder_pan_joint);
       robot_y = robot_y*sin(tr5shoulder_pan_joint);

       // shoulder
       robot_z += base_heigth + shoulder_heigth;


       if(robot_y>req.pose.position.y) tr5shoulder_pan_joint-=0.015;
       else if(robot_y<req.pose.position.y) tr5shoulder_pan_joint+=0.015;
       ROS_INFO("ballY=%f and robotY=%f",req.pose.position.y,robot_y);


       if(robot_z>req.pose.position.z) tr5shoulder_lift_joint-=0.015;
       else if(robot_z<req.pose.position.z) tr5shoulder_lift_joint+=0.015;
       ROS_INFO("ballZ=%f and robotZ=%f",req.pose.position.z,robot_z);


      if(req.pose.position.z<0.52){// MAKE SURE THE BALL IS NOT 2 HIGH
          if(robot_x>req.pose.position.x) tr5elbow_joint-=0.015;
          else if(robot_x<req.pose.position.x) tr5elbow_joint+=0.015;
          ROS_INFO("ballX=%f and robotX=%f",req.pose.position.x,robot_x);
      }
      else{
          tr5elbow_joint+=0.015;
      }

      if(std::abs(tr5shoulder_pan_joint) <0.8){// X axis
        if(req.pose.position.x>0 && req.pose.position.x<0.26){//INSIDE THE ROBOT
            tr5wrist_1_joint-=0.015;
        }
        else if(tr5wrist_1_joint<0){
          tr5wrist_1_joint+=0.015;
        }
      }
      else{ // Y axis
        if(req.pose.position.y>0 && req.pose.position.y<0.26 ){//INSIDE THE ROBOT
            tr5wrist_1_joint-=0.015;
        }
        else if(tr5wrist_1_joint<0){
          tr5wrist_1_joint+=0.015;
        }
      }

       ROS_INFO("X dif = %f",std::abs(robot_x - req.pose.position.x));
       ROS_INFO("Y dif = %f",std::abs(robot_y - req.pose.position.y));
       ROS_INFO("Z dif = %f",std::abs(robot_z - req.pose.position.z));


       if(( std::abs(robot_x - req.pose.position.x)<0.03) &&
       ( std::abs(robot_y - req.pose.position.y)<0.03) &&
       ( std::abs(robot_z - req.pose.position.z)<0.03)  ){
         tr5gripper_1_joint+=0.047;
         tr5gripper_2_joint+=0.047;
       }
       else {
         tr5gripper_1_joint-=0.047;
         tr5gripper_2_joint-=0.047;
       }

       //weird positions extra control
       ROS_INFO("x=%f y=%f z=%f",req.pose.position.x,req.pose.position.y,req.pose.position.z);
       if(req.pose.position.z>0.59 && req.pose.position.x<0.23 //TOP OF THE ROBOT , LETS DANCE
        && req.pose.position.y<0.3 && req.pose.position.y>-0.3){
            tr5wrist_1_joint+=0.015;
       }

}
       // CONTROL LIMITS
       if(tr5shoulder_pan_joint<-1.40) tr5shoulder_pan_joint = -1.40;
       else if(tr5shoulder_pan_joint>1.40) tr5shoulder_pan_joint = 1.40;

       if(tr5shoulder_lift_joint<-0.52) tr5shoulder_lift_joint = -0.52;
       else if(tr5shoulder_lift_joint>1.22) tr5shoulder_lift_joint = 1.22;

       if(tr5elbow_joint<-1.75) tr5elbow_joint = -1.75;
       else if(tr5elbow_joint>0) tr5elbow_joint = 0;

       if(tr5wrist_1_joint<-1.75) tr5wrist_1_joint = -1.75;
       else if(tr5wrist_1_joint>1.75) tr5wrist_1_joint = 1.75;

       if(tr5wrist_2_joint<-1.75) tr5wrist_2_joint = -1.75;
       else if(tr5wrist_2_joint>1.75) tr5wrist_2_joint = 1.75;

       if(tr5gripper_1_joint<0) tr5gripper_1_joint = 0;
       else if(tr5gripper_1_joint>0.47) tr5gripper_1_joint = 0.47;

       if(tr5gripper_2_joint<0) tr5gripper_2_joint = 0;
       else if(tr5gripper_2_joint>0.47) tr5gripper_2_joint = 0.47;

       // PUSHBACK VALUES
       res.joints.name.clear();
       res.joints.name.push_back( "tr5shoulder_pan_joint"  );
       res.joints.position.push_back(tr5shoulder_pan_joint);
       res.joints.name.push_back( "tr5shoulder_lift_joint" );
       res.joints.position.push_back(tr5shoulder_lift_joint);
       res.joints.name.push_back( "tr5elbow_joint"         );
       res.joints.position.push_back(tr5elbow_joint);
       res.joints.name.push_back( "tr5wrist_1_joint"       );
       res.joints.position.push_back(tr5wrist_1_joint);
       res.joints.name.push_back( "tr5wrist_2_joint"       );
       res.joints.position.push_back(tr5wrist_2_joint);
       res.joints.name.push_back( "tr5gripper_1_joint"     );
       res.joints.position.push_back(tr5gripper_1_joint);
       res.joints.name.push_back( "tr5gripper_2_joint"     );
       res.joints.position.push_back(tr5gripper_2_joint);
       res.joints.header.stamp = ros::Time::now();
       return true;

return true;
}
