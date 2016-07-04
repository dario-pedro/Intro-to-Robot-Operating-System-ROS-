#ifndef _NXT_LINE_DETECOTR_HPP__
#define _NXT_LINE_DETECOTR_HPP__

#include <ros/ros.h>

#include <std_msgs/Bool.h>
#include "nxt_msgs/Light.h"


namespace NXT
{
    class Line_detector
    {
        public:

            Line_detector ();
            virtual ~Line_detector (){};

            void run();

        private:

            ros::NodeHandle                               m_nd;

            ros::Publisher                                m_pub_light;

            ros::Subscriber                               m_sub_light;		

            void nxt_light_clbk ( const nxt_msgs::Light::ConstPtr& msg );
    };
}

#endif

