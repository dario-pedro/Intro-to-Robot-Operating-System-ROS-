

#include "line_detector/Line_detector.hpp"

using namespace NXT;

Line_detector::Line_detector()
{


    m_sub_light = m_nd.subscribe( "/nxt/intensity_sensor", 1, &Line_detector::nxt_light_clbk, this );

    m_pub_light = m_nd.advertise<std_msgs::Bool>("/nxt/line_in_sight", 1000);
}


void Line_detector::run(){
    ros::spin();
}



void Line_detector::nxt_light_clbk ( const nxt_msgs::Light::ConstPtr& msg )
{
  std_msgs::Bool result;

  ROS_INFO("line sensor results = %f",msg->intensity);

  if(msg->intensity < 300){
    result.data  = true;
  }
  else{
    result.data = false;
  }

  m_pub_light.publish(result);
}
