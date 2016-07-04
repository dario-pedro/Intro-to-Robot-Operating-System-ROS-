
//
// TODO include class header
//
#include "tr5_kinematics/Kinematics.hpp"

int main( int argc, char** argv )
{
    ros::init( argc, argv, "tr5_kinematics_node" );
    TR5::Kinematics tr5_kinematics;
    tr5_kinematics.run();
    return 0;
}
