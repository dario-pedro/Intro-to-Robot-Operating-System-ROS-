#ifndef _TR5_KINEMATICS_HPP__
#define _TR5_KINEMATICS_HPP__

#define base_length 0.085
#define base_heigth 0.190
#define shoulder_heigth 0.085
#define upper_arm_length 0.200
#define fore_arm 0.130
#define wrist_length 0.010
#define gripperPLUSfingerlength 0.113


//
// TODO include headers
//
#include <ros/ros.h>
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/Pose.h"
#include "tr5_kinematics/DoForwardKinematic.h"
#include "tr5_kinematics/DoInverseKinematic.h"

namespace TR5
{
    //
    // TODO complete class definition as required
    //
    class Kinematics
    {
        public:

            Kinematics();
            virtual ~Kinematics(){};
            ros::NodeHandle nh;

            void run();


        private:
	    ros::ServiceServer serviceFK; 
	    ros::ServiceServer serviceIK;
	    ros::ServiceServer serviceINT; 
            bool do_fk ( tr5_kinematics::DoForwardKinematic::Request  &req,
                         tr5_kinematics::DoForwardKinematic::Response &res  );

            bool do_ik ( tr5_kinematics::DoInverseKinematic::Request  &req,
                         tr5_kinematics::DoInverseKinematic::Response &res );

            bool do_int ( tr5_kinematics::DoInverseKinematic::Request  &req,
                         tr5_kinematics::DoInverseKinematic::Response &res  );
    };
}

#endif /* _TR5_KINEMATICS_HPP__ */
