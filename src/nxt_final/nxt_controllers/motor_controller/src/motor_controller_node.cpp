
#include "motor_controller/Motor_controller.hpp"

using namespace NXT;
int main( int argc, char** argv )
{
    ros::init( argc, argv, "motor_controller_node" );

    NXT::Motor_controller motor;

    motor.run();

    return 0;
}
