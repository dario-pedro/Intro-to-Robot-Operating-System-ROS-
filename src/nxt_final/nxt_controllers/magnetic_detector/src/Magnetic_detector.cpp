

#include "magnetic_detector/Magnetic_detector.hpp"

using namespace NXT;

Magnetic_detector::Magnetic_detector()
{


    m_sub_magnetic = m_nd.subscribe( "/nxt/magnetic_sensor", 1, &Magnetic_detector::nxt_magnetic_clbk , this );
    m_pub_magnetic = m_nd.advertise<std_msgs::Bool/*geometry_msgs::Pose*/>("/nxt/magnetic_status", 1000);
    m_pub_magnetic_pose = m_nd.advertise<visualization_msgs::Marker>("/nxt/magnetic_pose",1000);
    m_sub_pos = m_nd.subscribe( "/nxt/odom", 1, &Magnetic_detector::get_pos_odom_clbk, this );

    response.data = false;


    // visual marker object for RVIZ
    m_marker.ns              = "forward kinematics marker";
    m_marker.id              = 0;
    m_marker.header.stamp    = ros::Time();
    m_marker.header.frame_id = "/odom";
    m_marker.color.a         = 0.5;
    m_marker.color.r         = 1.0;
    m_marker.color.b         = m_marker.color.g = 0.0;
    m_marker.scale.x         = m_marker.scale.y = m_marker.scale.z = 0.1;
    m_marker.action          = visualization_msgs::Marker::ADD;
    m_marker.type            = visualization_msgs::Marker::SPHERE;
    m_marker.lifetime        = ros::Duration(0);
    m_marker.pose.position.x=0;
    m_marker.pose.position.y=0;
    m_marker.pose.position.z=0.2;


}


void Magnetic_detector::run(){

    ros::spin();
}


void Magnetic_detector::nxt_magnetic_clbk ( const nxt_msgs::MagneticField::ConstPtr& msg )
{
  //geometry_msgs::Pose pose;
  //ROS_INFO("magnetic sensor results = %f",msg->value);

  if(msg->value > 5){
    //send my position
    /*pose.position.x = 0;
    pose.position.y = 0;
    pose.position.z = 0;*/
    response.data = true;

  }
  else{//IGNORE
    response.data = false;
  }
  if(! (m_marker.pose.position.x == 0 && m_marker.pose.position.y == 0) ){
      m_pub_magnetic_pose.publish(m_marker);
  }
//  ROS_INFO("Public response = %d",response.data);

  m_pub_magnetic.publish(response);
}

void Magnetic_detector::get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg ){
  if(response.data) m_marker.pose = msg->pose.pose;
}
