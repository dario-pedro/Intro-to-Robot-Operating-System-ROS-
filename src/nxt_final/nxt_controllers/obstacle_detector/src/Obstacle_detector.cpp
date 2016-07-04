

#include "obstacle_detector/Obstacle_detector.hpp"

using namespace NXT;

/* EXAMPLE OF A 6X6 MAP
    [0  1  2  3  4  5
     6  7  8  9  10 11
     12 13 14 15 16 17
     18 19 20 21 22 23
     24 25 26 27 28 29
     30 31 32 33 34 35]

     int pos=0;
     int aux=0 ;
     aux = (row-1) * height;
     pos+=aux;
     pos+=col-1;
     data[pos] = probabilidade;
*/
Obstacle_detector::Obstacle_detector(){

    m_sub_obstacle = m_nd.subscribe( "/nxt/ultrasonic_sensor", 1, &Obstacle_detector::nxt_obstacle_clbk , this );
    m_pub_obstacle = m_nd.advertise<nav_msgs::OccupancyGrid>("/nxt/map", 1000);
    m_pub_debug  = m_nd.advertise<nav_msgs::Odometry>("/nxt/debug", 1000);
    m_sub_pos = m_nd.subscribe( "/nxt/odom", 1, &Obstacle_detector::get_pos_odom_clbk, this );
    m_sub_angle = m_nd.subscribe( "/nxt/gyro_sensor/imu", 1, &Obstacle_detector::get_gryo_angle_clbk, this );
    occ_grid.info.width = 300;// X
    occ_grid.info.height = 600; // Y
    occ_grid.info.resolution = 0.01;
    angulo = -1000;
    //i=0;
    leng_map = (occ_grid.info.width ) * (occ_grid.info.height);
    occ_grid.data.resize(leng_map);
    //map_data =  new int[leng_map];
}

void Obstacle_detector::run(){
/*  int32_t publish_rate_ = 40;
  tf::TransformBroadcaster tf_br_;
  tf::StampedTransform tf_map_to_odom_;

  // set up parent and child frames
  tf_map_to_odom_.frame_id_ = std::string("/map");
  tf_map_to_odom_.child_frame_id_ = std::string("/odom");
  tf::Transform transform;

  // set up publish rate
  ros::Rate go(publish_rate_);

  // main loop
  while (ros::ok())
  {
    // time stamp
    tf_map_to_odom_.stamp_ = ros::Time::now();


    // specify actual transformation vectors from odometry
    transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    tf::Quaternion q;
    q.setRPY(0, 0, 0);
    transform.setRotation(q);
    tf_map_to_odom_.setData(transform);

    // broadcast transform
    tf_br_.sendTransform(tf_map_to_odom_);

    ros::spinOnce();
    go.sleep();//NOW!
  }*/
  ros::spin();
}

void Obstacle_detector::get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg ){
  pos_x = msg->pose.pose.position.x;
  pos_y = msg->pose.pose.position.y;
  tf::Quaternion q(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);
   tf::Matrix3x3 m(q) ;
   double roll, pitch/*yaw*/;
   m.getRPY(roll, pitch, angulo);
  // ROS_INFO("R- %f P- %f ANGLE=%f",roll,pitch,angulo);

}

void Obstacle_detector::get_gryo_angle_clbk ( const sensor_msgs::Imu::ConstPtr& msg ){
  //angulo = .w;
  //double teste = msg->orientation::btQuaternion::GetAngle();
//  tf::Quaternion q(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
//  tf::Matrix3x3 m(q) ;
//  double roll, pitch/*yaw*/;
//  m.getRPY(roll, pitch, angulo);
//  ROS_INFO("R- %f P- %f ANGLE=%f",roll,pitch,angulo);
}


void Obstacle_detector::nxt_obstacle_clbk ( const nxt_msgs::Range::ConstPtr& msg ){

  if(angulo == -1000){
    return; // didnt yet receive angles.
  }

  float probabilidade = -1;//   (1/range)

  if(msg->range < 0.32)       probabilidade = 80;
  else if(msg->range > 1.8)   return;//probabilidade = 0;
  else if(msg->range < 2.54)  probabilidade = (-39.22 *msg->range + 100 +12)/2; // add 12 to reset the 0.32 offset
  else                        probabilidade = -1;

  /*MOVE THE ROBOT TO CENTER OF THE MAP */
  pos_x += 3;
  pos_y += -1.5;


  float working_ang;
  float objectX ;
  float objectY ;
  int pos;
  for(float k=0,condition_value=msg->range*10 ;k<=condition_value;k++){
    //ROS_INFO("do for until k:%f <= condition_value:%f",k,condition_value);
    if(k<condition_value/2){ // USE HALF THE VALUES FOR LEFT
      working_ang = HALF_PI + angulo + k/100;
    }
    else{// USE THE OTHER HALF THE VALUES FOR RIGHT
      working_ang = HALF_PI + angulo - k/200;
    }
    //ROS_INFO("working angle= %f",working_ang);
    objectX = (pos_x + msg->range * sin( working_ang ) )/occ_grid.info.resolution;
    objectY = (pos_y - msg->range * cos( working_ang ) )/occ_grid.info.resolution;
    //ROS_INFO("objectX=%f , objectY=%f ",objectX,objectY);
    pos = convert_2D_to_1D(objectX,objectY);
    //ROS_INFO("POS= %d",pos);
    if(pos>-1 && pos<leng_map){
      if(occ_grid.data[pos] > (probabilidade +1 - INCREMENT_PROBABILITY)){
        //ROS_INFO("position probabilidade is %d",occ_grid.data[pos]);
        if(occ_grid.data[pos] + INCREMENT_PROBABILITY < 101){
          //ROS_INFO("increase probabilidade");
          occ_grid.data[pos] = occ_grid.data[pos]+INCREMENT_PROBABILITY;
        }
        else{
          re_render_lineOfSight(working_ang,msg->range);
          occ_grid.data[pos] = 100;
        }
      }
      else{ // ELSE OF COMPARATION BETWEEN probabilidade
        occ_grid.data[pos] = probabilidade;
      }
    }
    else{
      ROS_INFO("Error in POS-> %d Size-> %d",pos,leng_map);
    }
  }
  m_pub_obstacle.publish(occ_grid);
}

void Obstacle_detector::re_render_lineOfSight(float line_angle,float range){
  float cellX,cellY;
  int array_i;
  range-occ_grid.info.resolution;
  for(;range>0;range=range-occ_grid.info.resolution){
    cellX = (pos_x + range * sin( line_angle ) )/occ_grid.info.resolution;
    cellY = (pos_y - range * cos( line_angle ) )/occ_grid.info.resolution;
    //check if the object is inside the maze map
    if(cellX < occ_grid.info.width/occ_grid.info.resolution && cellY < occ_grid.info.height/occ_grid.info.resolution){
      array_i = convert_2D_to_1D(cellX,cellY);
    }
    if(array_i>-1 && array_i<leng_map){
      //ROS_INFO("ANALYSE LINE OF SIGHT OF ROBOT for range %f",i);
    //  if(occ_grid.data[array_i]<70){ // MAKE SURE THERE ISNT A HIGHT PROBABILITY DOT INSIDE THAT POSITION
        occ_grid.data[array_i] = 0;
        //ROS_INFO("Reset point for range %f",range);
    //  }
    }
    else{
      ROS_INFO("Error Resting POS-> %d Size-> %d",array_i,leng_map);
    }
  }
}

int Obstacle_detector::convert_2D_to_1D(float xCord,float yCord){
  return occ_grid.info.width * ((int)xCord - 1) -yCord-1;
}
