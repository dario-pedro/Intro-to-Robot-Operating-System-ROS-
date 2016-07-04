
#include "current_position/Current_position.hpp"

using namespace NXT;
int main( int argc, char** argv )
{
    ros::init( argc, argv, "current_position_node" );

    NXT::Current_position cp;

    cp.run();

    return 0;
}
