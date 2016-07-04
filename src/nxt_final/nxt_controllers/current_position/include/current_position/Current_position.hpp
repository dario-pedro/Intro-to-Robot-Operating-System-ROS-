#ifndef _NXT_CURRENT_POSITION_HPP__
#define _NXT_CURRENT_POSITION_HPP__

#include <ros/ros.h>

#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_listener.h>


#include "nxt_msgs/Range.h"


namespace NXT
{
    class Current_position
    {
        public:

            Current_position ();
            virtual ~Current_position (){};

            void run();

        private:
		

	    nav_msgs::Odometry position;
            ros::NodeHandle                               m_nd;

            ros::Publisher                                m_pub_pos;
	    ros::Subscriber                               m_sub_pos;		
	    
	   
    };
}

#endif

