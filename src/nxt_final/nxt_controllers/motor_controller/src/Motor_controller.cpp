

#include "motor_controller/Motor_controller.hpp"

using namespace NXT;

Motor_controller::Motor_controller(){
    current_speed = 10;
    //m_sub_gui_vel = m_nd.subscribe( "/nxt/cmd_vel", 1, &Motor_controller::gui_vel_clbk, this );
    m_sub_line = m_nd.subscribe( "/nxt/intensity_sensor", 1, &Motor_controller::follow_line_clbk, this );
    m_sub_pos = m_nd.subscribe( "/nxt/odom", 1, &Motor_controller::get_pos_odom_clbk, this );
    m_sub_magneticBool = m_nd.subscribe( "/nxt/magnetic_status", 1, &Motor_controller::get_magnetic_status, this );
    m_sub_pid = m_nd.subscribe( "/control_effort", 1, &Motor_controller::get_pid_result, this );
    m_sub_dodge_objects = m_nd.subscribe( "/nxt/ultrasonic_sensor", 1, &Motor_controller::dodge_object_clbk, this );
    pid_result=0;
    delay_counter = 0;
    aux_center=0;
    aux_180=0;
    aux_left=0;
    aux_right=0;
    calibration_white=false;
    calibration_black=false;
    following_line_counter=0;
    counter_time_maze2=0;
    noTape_control =false;
    counter_left=0;
    _90degreesFlag = false;
    endMapCounter=0;
    detected_magnetic=false;
    already_turned_left=false;
    already_turned_right=false;
    already_turned_center=false;
    control = true;
    decided_side=false;
    velocity=true;
    previous_angle=1000;
    current_angle=1000;
    left_side=0;
    right_side=0;
    count=0;
    last_effort_left=0;
    last_vecocity_right=0;
    last_vecocity_left=0;
    isOnShadows = false;
    isLineDetecterActive = false;
    dodge_object_flag = false;
    dodge_object_counter=0;
    pos_x = 0;
    pos_y = 0;
    START_WHITE=550;
    END_BLACK=1000;
    turn_robot_counter = 0;
    isDetectingLine = false;
    previous_isDetectingLine= false;
    isDetectingMagnetic = false;
    isGoingInfront= false;
    isGoingLeft= false;
    isGoingRight= false;
    //swap_controllers_LineToMap();//TODO DEBUG MESSAGE
    m_pub_grey = m_nd.advertise<std_msgs::Float64>("/nxt/current_grey", 1);
    m_pub_light = m_nd.advertise<std_msgs::Float64>("/nxt/intensity_sensor_state", 1);

    m_pub_goal_vel = m_nd.advertise<sensor_msgs::JointState>("/nxt/joint_command", 1);
}


void Motor_controller::run(){
    ROS_INFO("*************RUN MOTOR CONTROLLER****************");
    ros::spin();
}

void Motor_controller::follow_line(const float light){
  std_msgs::Float64 aux;
  aux.data = START_WHITE ;
  m_pub_grey.publish(aux);
  aux.data = light;
  m_pub_light.publish(aux);
  ROS_INFO("Current White=%d",START_WHITE);
  ROS_INFO("Current Black=%d",END_BLACK);
  ROS_INFO("light=%f",light);
  ROS_INFO("PID_RESULT=%f",pid_result);
  following_line_counter++;
/*  if((isGoingInfront||noTape_control) && pos_x<0.22 && pos_y<0.8 && pos_y>0.65 ){
    ROS_INFO("NO TAPE!");
    turn_robot_counter++;
    noTape_control = true;

    if(turn_robot_counter<6){
      ROS_INFO("FORCE RIGHT!");
      current_speed = 8;
      isGoingInfront= false;
      isGoingLeft= false;
      isGoingRight= true;
    }
    else{
      ROS_INFO("FORCE in front!");
      current_speed+=2;
      isGoingInfront= true;
      isGoingLeft= false;
      isGoingRight= false;
    }
  }
  else*/
   if(light  > START_WHITE && !_90degreesFlag){//WHITE
    current_speed=8;
    if(isGoingLeft){
      counter_left++;
      if(counter_left>26){
        _90degreesFlag = true;
        counter_left=0;
        isGoingInfront= false;
        isGoingLeft= false;
        isGoingRight= false;
      }
    }
    else{
      counter_left = 0;
      isGoingInfront= false;
      isGoingLeft= true;
      isGoingRight= false;
    }
  }
  else if(light > END_BLACK){ //MIDLE
    current_speed+=2;
    isGoingInfront= true;
    isGoingLeft= false;
    isGoingRight= false;
  }else{ //BLACK
    current_speed = 8;
    isGoingInfront= false;
    isGoingLeft= false;
    isGoingRight= true;
  }



  //ROS_INFO("Current speed = %f!",current_speed);
  //ROS_INFO("(x,y) = (%f,%f)!",pos_x,pos_y);
  ROS_INFO("Counter Left = %d!",counter_left);
  ROS_INFO("following_line_counter = %d!",following_line_counter);

    sensor_msgs::JointState vel_msg;
    float speed_left=current_speed;
    float speed_right=current_speed;


    if (_90degreesFlag){
      ROS_INFO("DOIN 90D endCounter %d , counterLeft %d",endMapCounter,counter_left);
      counter_left++;
      if(following_line_counter<50 || endMapCounter==1){
        following_line_counter=0;
        //2ND TIME ROTATING - TRY NO TAPE AREA
        ROS_INFO("Am I in a place with no Tape ? ");
        if(counter_left<4){
          speed_right = -3;
          speed_left= 10;
        }
        else {
          speed_right = 20;
          speed_left= 20;
          counter_left++;
        }
      }
      else if(counter_left<16){ // 90 DEGREE CURVE
        speed_right = -14;
        speed_left= 14;
      }
      else {
        speed_right = 14;
        speed_left= 14;
      }



      if ( counter_left>19 || light<END_BLACK) {
        isGoingInfront= false;
        isGoingLeft= false;
        isGoingRight= false;
        _90degreesFlag=false;
        counter_left=0;
        following_line_counter=0;
        if(endMapCounter>=2){
          swap_controllers_LineToMap();
        }else{
          endMapCounter++;
        }
      }
    }
    else if(isGoingInfront){
      ROS_INFO("Keep going!");
      endMapCounter=0;
    }else if(isGoingLeft){
      ROS_INFO("Turn Left!");
      speed_left = 0.5 ;
      speed_right = speed_right - pid_result;
    }
    else if(isGoingRight){
      ROS_INFO("Turn Right!");
      /*if(pos_x<0.42 && pos_y>0.9) speed_right = - (current_speed+2);
      else*/
      speed_right = 0.5 ;
      speed_left = speed_left + pid_result;
    }
    else{
      ROS_INFO("ERROR!");
    }

    put_velocity(speed_left,speed_right);

}

void Motor_controller::handle_shadows(const float light){
  if(light < 0.7*END_BLACK){// ENTER SHADOWS
      for(int i=0;i<10;i++)//spam print
        ROS_INFO("*************WORKING IN THE SHADOWS - Change color values****************");
      END_BLACK=light;
      END_BLACK=1.35*END_BLACK;//for security pull him a litle bit out of the line
      START_WHITE = 1.45*END_BLACK;
      isOnShadows = true;
  }
  else if(isOnShadows && light > START_WHITE*1.3){ //FROM SHADOWS TO LIGHT AGAIN!
    for(int i=0;i<10;i++)//spam print
      ROS_INFO("*************FROM SHADOWS TO LIGHT AGAIN!****************");
    START_WHITE = light;
    END_BLACK = START_WHITE / 1.6 ;
    isOnShadows = false;
  }
}

void Motor_controller::handle_maxSpeed(){
  if(current_speed>MAX_SPEED){
    current_speed-=2;
  }
}

void Motor_controller::check_endMaze1(){
    if(pos_x>1.25 && pos_y<0.75){
      sensor_msgs::JointState vel_msg;
      vel_msg.name.push_back("nxt/left_motor_joint");
      vel_msg.position.push_back(0.0);
      vel_msg.velocity.push_back(0.0);
      vel_msg.effort.push_back(0);

      vel_msg.name.push_back("nxt/right_motor_joint");
      vel_msg.position.push_back(0.0);
      vel_msg.velocity.push_back(0.0);
      vel_msg.effort.push_back(0);

      m_pub_goal_vel.publish(vel_msg);
      swap_controllers_LineToMap();
      //SWAP ROBOT CONTROLLERS

    }
}


void Motor_controller::swap_controllers_LineToMap(){
  m_sub_line.shutdown();
  m_sub_dodge_objects.shutdown();
  ROS_INFO("WELCOME TO MAZE 2 - LET THE GAMES BEGIN AGAIN!");
  swap_positionX = pos_x;
  swap_positionY = pos_y;
  counter_time_maze2=0;
  m_sub_map = m_nd.subscribe( "/nxt/ultrasonic_sensor", 1, &Motor_controller::maze2_clbk, this );
  m_sub_lineBool = m_nd.subscribe( "/nxt/intensity_sensor", 1, &Motor_controller::isDetectingLine_clbk, this );
  m_sub_angle = m_nd.subscribe( "/nxt/gyro_sensor/imu", 1, &Motor_controller::get_gryo_angle_clbk, this );
//  m_sub_effort = m_nd.subscribe( "/nxt/joint_states", 1, &Motor_controller::last_efforts_clbk, this );
}

void Motor_controller::swap_controllers_MapToLine(){
  m_sub_map.shutdown();
  m_sub_lineBool.shutdown();
  m_sub_angle.shutdown();
  following_line_counter=0;
  //m_sub_effort.shutdown();
  m_sub_dodge_objects = m_nd.subscribe( "/nxt/ultrasonic_sensor", 1, &Motor_controller::dodge_object_clbk, this );
  m_sub_line = m_nd.subscribe( "/nxt/intensity_sensor", 1, &Motor_controller::follow_line_clbk, this );
}


bool Motor_controller::isCalibrated(){
    bool result = calibration_black&&calibration_white;
    if(result && END_BLACK>MAX_BLACK){// MAKE SURE THERE WASN'T A BUG IN THE CALIBRATION
      result = false;
      swap_controllers_LineToMap();
      resetCalibration();
    }
    return result;
}

void Motor_controller::resetCalibration(){
  calibration_black=false;
  calibration_white=false;
  START_WHITE=550;
  END_BLACK=1000;
}



void Motor_controller::calibrate_light(const float light){
  ROS_INFO("WAIT COUNTER = %d",delay_counter);
/*  delay_counter++;
  if(delay_counter<25){
    return;
  }*/

  sensor_msgs::JointState vel_msg;
  float speed_left=current_speed;
  float speed_right=current_speed;

  ROS_INFO("*************Calibrating!****************");
  ROS_INFO("Current White=%d",START_WHITE);
  ROS_INFO("Current Black=%d",END_BLACK);
  ROS_INFO("Current Light=%f",light);


   if(!calibration_black){
    if(END_BLACK>light){
      speed_left = 0.5;
      ROS_INFO("CHECK LEFT");
      END_BLACK=light;
    }
    else{
      ROS_INFO("CHECK RIGHT");
      speed_right = 0.5;
      END_BLACK=1.25*END_BLACK;//for security pull him a litle bit out of the line
      calibration_black = true;
    }
  }
  else if(!calibration_white){
    ROS_INFO("GO RIGHT");
    speed_right = 0.5;
    START_WHITE = 1.5*END_BLACK;
    calibration_white = true;
    delay_counter = 0;
  }

  put_velocity(speed_left,speed_right);
}

void Motor_controller::put_velocity(const int speed_left,const int speed_right){
  //ROS_INFO("#Put Velocity Fuction# left=%d right=%d ",speed_left,speed_right);
  sensor_msgs::JointState vel_msg;
  vel_msg.name.push_back("nxt/left_motor_joint");
  vel_msg.position.push_back(0.0);
  vel_msg.velocity.push_back(0.0);
  vel_msg.effort.push_back(speed_left);
  vel_msg.name.push_back("nxt/right_motor_joint");
  vel_msg.position.push_back(0.0);
  vel_msg.velocity.push_back(0.0);
  vel_msg.effort.push_back(speed_right);
  m_pub_goal_vel.publish(vel_msg);
}

void Motor_controller::turn_left(){
  float speed_left;
  float speed_right;
  current_speed=12;
  speed_left=-current_speed*2;
  speed_right=+current_speed*2;
  put_velocity(speed_left,speed_right);

}

void Motor_controller::turn_right(){
  float speed_left;
  float speed_right;
  current_speed=12;
  speed_left=+current_speed;
  speed_right=-current_speed;
  put_velocity(speed_left,speed_right);
}





/******************************************************************
 *
 *
 * CALLBLACKS
 *
 *
 *******************************************************************/

 void Motor_controller::last_efforts_clbk( const sensor_msgs::JointState::ConstPtr& msg){
   last_effort_left = msg->effort[0];
   //last_effor_right = msg->effort[1];
   //last_vecocity_left=msg->velocity[0];
   //last_vecocity_right=msg->velocity[1];
 }

 void Motor_controller::get_gryo_angle_clbk ( const sensor_msgs::Imu::ConstPtr& msg ){
   //angulo = .w;
   //double teste = msg->orientation::btQuaternion::GetAngle();
   tf::Quaternion q(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
   tf::Matrix3x3 m(q) ;
   double roll, pitch/*yaw*/;
   m.getRPY(roll, pitch, yaw);
   //ROS_INFO("R- %f P- %f ANGLE=%f",roll,pitch,angulo);
 }

 void Motor_controller::maze2_clbk ( const nxt_msgs::Range::ConstPtr& msg/*const nav_msgs::OccupancyGrid::ConstPtr& map */){
   float speed_left;
   float speed_right;
  ROS_INFO("counter_time_maze2 %d ",counter_time_maze2);
   handle_maxSpeed();
   counter_time_maze2++;

   if(isDetectingLine){
     ROS_INFO("YOU HAVE DETECTED THE LINE AGAIN!! IS IT THE END??");
     if(detected_magnetic){
       current_speed = 0;
       speed_left=current_speed;
       speed_right=current_speed;
       put_velocity(speed_left,speed_right);
       ros::shutdown();
     }
     else{
       turn_left();

     }
   }
   else if(fabs(pos_x-swap_positionX)<0.03 && fabs(pos_y-swap_positionY)<0.03 && aux_180<=15 && counter_time_maze2>35){
      aux_180++;
      ROS_INFO("YOU SHALL NOT PASS !!! - FORCE TO TURN BACK currentPos(%f,%f) , swapPos(%f,%f) XResult%d",pos_x,pos_y,swap_positionX,swap_positionY,counter_time_maze2);
      ROS_INFO("currentPos(%f,%f) , swapPos(%f,%f)",pos_x,pos_y,swap_positionX,swap_positionY);
      ROS_INFO("XResult=%f , YResult=%f",fabs(pos_x-swap_positionX),fabs(pos_y-swap_positionY));
      turn_right();
   }
   else if(msg->range > 0.22 && aux_left == 0 && aux_right == 0 && aux_center ==0){  //0.35

     current_speed++;
     decided_side=false;
     /*if(velocity){
       current_speed=18;
       //if(last_effort_left >= MAX_SPEED || last_effort_left == -100)
        last_vecocity_left=current_speed+0.5;
       //else
        //speed_left=current_speed+(last_effort_left* (last_vecocity_right - last_vecocity_left)/last_vecocity_left);
       last_vecocity_right=current_speed;
       //ROS_INFO("VELOCITY_LEFT-> %f VELOCITY_RIGHT-> %f EFFORT_LEFT-> %f",last_vecocity_left,last_vecocity_right,last_effort_left);
       velocity=false;
     }

     //CONTROL THE KEEP GOING - RODAS
     if(control){
       previous_angle = yaw;
       control=false;
     }
     else{
       current_angle=yaw;
       //if(count <2){
         //count++;
         //return;
       //}
       //count=0;
       control=true;
       if(previous_angle < current_angle){
         last_vecocity_left-=0.5;
         ROS_INFO("VIRAR PARA A ESQUERDA");
       }
       else{
         if(previous_angle > current_angle)
           last_vecocity_left+=0.5;
           ROS_INFO("VIRAR PARA A DIREITA");
       }
     }*/
    if(counter_time_maze2<160) last_vecocity_left=1.04*current_speed;//torto
    else if(counter_time_maze2<300) last_vecocity_left=current_speed;//torto
    else last_vecocity_left=1.04*current_speed;
    last_vecocity_right=current_speed;

     put_velocity(last_vecocity_left,last_vecocity_right);
     //ROS_INFO("PREVIOUS_ANGLE-> %f CURRENT_ANGLE-> %f",previous_angle,current_angle);
     ROS_INFO("NO WALL INFRONT - KEEP GOING");
   }else{
     //if(isDetectingMagnetic)
     if(decided_side){
       ROS_INFO("ALREADY DECIDED");
      return;
    }
     if(!already_turned_right && aux_right==0){
       turn_right();
       already_turned_right=true;
       return;
     }

     if(already_turned_right && aux_right<=8){
       right_side = right_side + msg->range;
       ROS_INFO("TURNING RIGHT");
       aux_right++;
       return;
     }
     already_turned_right=false;

     if(!already_turned_left && aux_left==0){
       turn_left();
       //turn_left();
       already_turned_left=true;
       return;
     }

     if(already_turned_left && aux_left<=8){
       left_side = left_side + msg->range;
       ROS_INFO("TURNING LEFT");
       aux_left++;
       return;
     }
     already_turned_left=false;

     if(!already_turned_center && aux_center==0){
       turn_right();
       already_turned_center=true;
       return;
     }

     if(already_turned_center && aux_center<=8){
       ROS_INFO("TURNING CENTER");
       aux_center++;
       return;
     }
     already_turned_center=false;
     //CHOOSE THE RIGHT WAY
     ROS_INFO("LEFT-> %f  RIGHT-> %f",left_side,right_side);
     if(abs(left_side - right_side)<=0.01){
       ROS_INFO("VALORES MT PROXIMOS");
       current_speed=8;
       speed_left=-current_speed*3;
       speed_right=+current_speed*3;
       put_velocity(speed_left,speed_right);
     }
     else
      if(left_side>right_side){
       turn_left();
       ROS_INFO("TURN LEFT AND DODGE THE WALL");
      }
      else{
         turn_right();
         ROS_INFO("TURN RIGHT AND DODGE THE WALL");

      }
     decided_side=true;
     left_side=0;
     right_side=0;
     already_turned_center=false;
     already_turned_left=false;
     already_turned_right=false;
     aux_center=0;
     aux_180=0;
     aux_left=0;
     aux_right=0;
     velocity=true;
   }

 }

 void Motor_controller::dodge_object_clbk ( const nxt_msgs::Range::ConstPtr& msg ){
   float speed_left,speed_right;
   if( !isGoingLeft && following_line_counter>20 && !_90degreesFlag  &&  (msg->range<0.19 || dodge_object_flag)){
     dodge_object_flag = true;
     dodge_object_counter++;
     if(dodge_object_counter<5){
       if(dodge_object_counter == 1 ){
         ROS_INFO("DODGING OBJECT");
         current_speed = 50;
         speed_left=-current_speed-15;
         speed_right=-current_speed-15;
         put_velocity(speed_left,speed_right);
       }
       ROS_INFO("GO BACK , counter=%d",dodge_object_counter);
       return;
     }else if(dodge_object_counter<10){
       if(dodge_object_counter == 5 ){
         speed_left=current_speed;
         speed_right=0.5*current_speed;
         put_velocity(speed_left,speed_right);
       }
       if(msg->range<0.19 ){ // if the object is longer , reset variables
         dodge_object_counter=0;
         dodge_object_flag = false;
       }
       ROS_INFO("GO RIGHT , counter=%d",dodge_object_counter);
       return;
     }else if(dodge_object_counter<19){
       if(dodge_object_counter == 10 ){
         speed_left=0.5*current_speed;
         speed_right=current_speed;
         put_velocity(speed_left,speed_right);
       }
       ROS_INFO("GO LEFT , counter=%d",dodge_object_counter);
       return;
     }else if(dodge_object_counter<22){
     if(dodge_object_counter == 19 ){
       speed_left=current_speed;
       speed_right=0.5*current_speed;
       put_velocity(speed_left,speed_right);
     }
     ROS_INFO("GO RIGHT END , counter=%d",dodge_object_counter);
     return;
   }
   following_line_counter =0;
   dodge_object_counter=0;
   dodge_object_flag = false;
 }
}

void Motor_controller::follow_line_clbk ( const nxt_msgs::Light::ConstPtr& msg ){
  if(dodge_object_flag && !_90degreesFlag) // USE THE CODE IN DODGE OBJECT IF THE FLAG IS ACTIVE
    return;

  if(!isCalibrated()){
    calibrate_light(msg->intensity);
  }
  else{
    handle_maxSpeed();
    handle_shadows(msg->intensity);
    follow_line(msg->intensity);
    //check_endMaze1();
  }
}
void Motor_controller::get_pid_result ( const std_msgs::Float64::ConstPtr& result ){
    pid_result = result->data;
}


void Motor_controller::get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg ){
  pos_x = msg->pose.pose.position.x;
  pos_y = msg->pose.pose.position.y;
}

void Motor_controller::get_magnetic_status ( const std_msgs::Bool::ConstPtr& status ){
  isDetectingMagnetic = status->data;
  if(isDetectingMagnetic){
     ROS_INFO("YOU HAVE DETECTED A MagneticField - at (%f,%f)",pos_x,pos_y);
     detected_magnetic = isDetectingMagnetic;
  }
}

void Motor_controller::isDetectingLine_clbk(const nxt_msgs::Light::ConstPtr& msg ){
/*  if(END_BLACK> 400)
      END_BLACK = MAX_BLACK;*/
  if (msg->intensity < 400 )
      isDetectingLine = true;
  else
      isDetectingLine = false;
}


void Motor_controller::gui_vel_clbk ( const geometry_msgs::Twist::ConstPtr& msg )
{
//  ROS_INFO("*************MOTOR INFO****************");
//  ROS_INFO("angleZ = %f ",msg->angular.z);
/*  sensor_msgs::JointState vel_msg;

  float speed_left;
  float speed_right;

  speed_left =7.59574*(msg->linear.x/0.054) + 9.34959;
  speed_right = 7.59574*(msg->linear.x/0.054) + 9.34959;

  if(msg->linear.x == 0){
    speed_left=0;
    speed_right=0;
  }
  if(isGoingInfront){

    ROS_INFO("Keep going!");

  }else if(isGoingLeft){
    ROS_INFO("Turn Left!");
    speed_left = 0;
  }
  else if(isGoingRight){
    ROS_INFO("Turn Right!");
    speed_right = 0;
  }else{
      ROS_INFO("Error");
  }

    if(msg->angular.z > 0 ){
        speed_right = speed_right * cos(msg->angular.z);
    }
    else if(msg->angular.z < 0 ) {
        speed_left = speed_left * cos(msg->angular.z);
    }

  vel_msg.name.push_back("nxt/left_motor_joint");
  vel_msg.position.push_back(0.0);
  vel_msg.velocity.push_back(0.0);
  vel_msg.effort.push_back(speed_left);

  vel_msg.name.push_back("nxt/right_motor_joint");
  vel_msg.position.push_back(0.0);
  vel_msg.velocity.push_back(0.0);
  vel_msg.effort.push_back(speed_right);

  m_pub_goal_vel.publish(vel_msg);*/

  //update previous_isDetectingLine
  //previous_isDetectingLine = isDetectingLine;
}
