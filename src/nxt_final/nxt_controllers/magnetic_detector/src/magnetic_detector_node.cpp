
#include "magnetic_detector/Magnetic_detector.hpp"

using namespace NXT;
int main( int argc, char** argv )
{
    ros::init( argc, argv, "magnetic_detector_node" );

    NXT::Magnetic_detector magnetic_sensor;

    magnetic_sensor.run();

    return 0;
}
