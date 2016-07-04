#ifndef _NXT_OBSTACLE_DETECTOR_HPP__
#define _NXT_OBSTACLE_DETECTOR_HPP__

#include <ros/ros.h>

#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>
#include <geometry_msgs/Pose.h>
#include <sensor_msgs/Imu.h>
//#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>

//#include "tf/transform_datatypes.h"


#include "nxt_msgs/Range.h"

#define HALF_PI 1.570796
#define INCREMENT_PROBABILITY 5


namespace NXT
{
    class Obstacle_detector
    {
        public:

            Obstacle_detector ();
            virtual ~Obstacle_detector (){};

            void run();

        private:

	    int					  *map_data;
	    int leng_map;
	    nav_msgs::OccupancyGrid                       occ_grid;

	    //geometry_msgs::Pose 		          curr_pos;

	    //ROBOT POSITION
	    float pos_y;
	    float pos_x;

	    double angulo;
		//int i;
            ros::NodeHandle                               m_nd;

            ros::Publisher                                m_pub_obstacle;
	    ros::Publisher                                m_pub_debug;

            ros::Subscriber                               m_sub_obstacle;
	    ros::Subscriber                               m_sub_pos;
	    ros::Subscriber                               m_sub_angle;	

	    void re_render_lineOfSight(float line_angle,float range) ;	
	    int convert_2D_to_1D(float xCord,float yCord);
	
	    void get_gryo_angle_clbk ( const sensor_msgs::Imu::ConstPtr& msg );	    
	    void get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg );
            void nxt_obstacle_clbk ( const nxt_msgs::Range::ConstPtr& msg );
    };
}

#endif

