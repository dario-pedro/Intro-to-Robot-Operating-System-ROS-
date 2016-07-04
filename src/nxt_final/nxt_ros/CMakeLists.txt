cmake_minimum_required(VERSION 2.8.3)
project(nxt_ros)

find_package( catkin REQUIRED COMPONENTS rospy
                                         nxt_lib
                                         sensor_msgs
                                         nxt_msgs
)

catkin_package( CATKIN_DEPENDS rospy
                               nxt_lib
                               sensor_msgs
                               nxt_msgs
)
