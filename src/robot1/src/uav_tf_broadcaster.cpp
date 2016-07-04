#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include <std_msgs/Float32.h>

//std::string uav_name;

void poseCallback(const std_msgs::Float32ConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(0.0, 0.0, msg->data) );
  //tf::Quaternion q;
  //q.setRPY(0, 0, msg->theta);
  //transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "base_footprint", "base_link"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  //if (argc != 2){ROS_ERROR("need uav name as argument"); return -1;};
  //uav_name = argv[1];
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("/uav/altitude", 10, &poseCallback);
  ros::spin();
  return 0;
};
