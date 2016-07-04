

#include "current_position/Current_position.hpp"

using namespace NXT;

Current_position::Current_position()
{


  //  m_pub = m_nd.advertise<nav_msgs::OccupancyGrid>("/nxt/map", 1000);
    //m_sub_pos = m_nd.subscribe( "/nxt/odom", 1, &Current_position::get_pos_odom_clbk, this );
    m_pub_pos = m_nd.advertise<nav_msgs::Odometry>("/nxt/robot_position", 1);
}


void Current_position::run(){

  tf::TransformListener listener;
  ros::Rate go(55.0); // Rate is named go , to be able to use go.sleep :D
  while (m_nd.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("odom","nxt/base_footprint",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    position.pose.pose.position.x= transform.getOrigin().x();
    position.pose.pose.position.y = transform.getOrigin().y();
    position.pose.pose.position.z = transform.getOrigin().z();
    position.pose.pose.orientation.x = transform.getRotation().x();
    position.pose.pose.orientation.y = transform.getRotation().y();
    position.pose.pose.orientation.z = transform.getRotation().z();
    position.pose.pose.orientation.w = transform.getRotation().w();
    m_pub_pos.publish(position);
    go.sleep();
  }
}
