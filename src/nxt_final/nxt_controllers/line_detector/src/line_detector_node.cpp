
#include "line_detector/Line_detector.hpp"

using namespace NXT;
int main( int argc, char** argv )
{
    ros::init( argc, argv, "line_detector_node" );

    NXT::Line_detector light_sensor;

    light_sensor.run();

    return 0;
}
