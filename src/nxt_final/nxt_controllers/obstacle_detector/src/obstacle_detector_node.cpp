
#include "obstacle_detector/Obstacle_detector.hpp"

using namespace NXT;
int main( int argc, char** argv )
{
    ros::init( argc, argv, "obstacle_detector_node" );

    NXT::Obstacle_detector obstacle_sensor;

    obstacle_sensor.run();

    return 0;
}
