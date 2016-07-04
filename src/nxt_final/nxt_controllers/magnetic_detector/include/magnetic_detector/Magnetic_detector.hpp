#ifndef _NXT_MAGNETIC_DETECTOR_HPP__
#define _NXT_MAGNETIC_DETECTOR_HPP__

#include <ros/ros.h>

#include <geometry_msgs/Pose.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Bool.h>
#include "nxt_msgs/MagneticField.h"
#include <visualization_msgs/Marker.h>


namespace NXT
{
    class Magnetic_detector
    {
        public:

            Magnetic_detector ();
            virtual ~Magnetic_detector (){};

            void run();

        private:
	      std_msgs::Bool response;
            ros::NodeHandle                               m_nd;
            ros::Publisher                                m_pub_magnetic;
	    ros::Publisher                                m_pub_magnetic_pose;
            ros::Subscriber                               m_sub_magnetic;
	    ros::Subscriber                               m_sub_pos;
	    visualization_msgs::Marker                    m_marker;
            void nxt_magnetic_clbk ( const nxt_msgs::MagneticField::ConstPtr& msg );
	    void get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg );
    };
}

#endif
