cmake_minimum_required(VERSION 2.8.3)
project(tr5_kinematics)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  geometry_msgs
  message_generation
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

# Generate services in the 'srv' folder
 add_service_files(
   FILES
#   Service1.srv
#   Service2.srv
	DoForwardKinematic.srv
	DoInverseKinematic.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages( DEPENDENCIES geometry_msgs sensor_msgs)


catkin_package( INCLUDE_DIRS include
                CATKIN_DEPENDS roscpp std_msgs geometry_msgs sensor_msgs )


###########
## Build ##
###########

include_directories( include ${catkin_INCLUDE_DIRS} )

add_executable( tr5_kinematics_node src/tr5_kinematics_node.cpp src/Kinematics.cpp)

add_dependencies( tr5_kinematics_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} )

target_link_libraries( tr5_kinematics_node ${catkin_LIBRARIES} )
