#ifndef _NXT_MOTOR_CONTROLLER_HPP__
#define _NXT_MOTOR_CONTROLLER_HPP__

#include <ros/ros.h>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float64.h>
#include "sensor_msgs/JointState.h"
#include "nxt_msgs/Light.h"
#include <nav_msgs/OccupancyGrid.h>
#include "nxt_msgs/Range.h"
#include <sensor_msgs/Imu.h>
#include <tf/transform_broadcaster.h>

#define MAX_SPEED 30
#define MAX_BLACK 550
#define HALF_PI 1.570796


namespace NXT
{
    class Motor_controller
    {
        public:

            Motor_controller ();
            virtual ~Motor_controller (){};

            void run();

        private:

	    //Calibration FLAGS
	    bool calibration_white;
	    bool calibration_black;

	    //var maze2 movement
	    float left_side;
	    float right_side;
	    int aux_center;
      int aux_180;
	    int aux_left;
	    int aux_right;
	    bool already_turned_left;
	    bool already_turned_right;
	    bool already_turned_center;
	    bool decided_side;
	    bool detected_magnetic;
	    double previous_angle;
	    double current_angle;
	    double yaw;
	    bool control;
	    int count;
	    bool velocity;
	    float last_effort_left;
	    float last_vecocity_right;
	    float last_vecocity_left;

	    //ROBOT SPEED
	    float current_speed;

	    //ROBOT POSITION
	    float pos_y;
	    float pos_x;
      //on swap position
      float swap_positionY;
      float swap_positionX;


	    //var to w8 the odem_combined
	    int delay_counter;
      int following_line_counter;
      int counter_time_maze2;

	    //LIGHT ITENSITIES
	    int START_WHITE;
	    int END_BLACK;

	    //NO TAPE ROTATIO
	    int turn_robot_counter;

	    //control effecto
	    float pid_result;

	    //dodge objects variables
	    bool dodge_object_flag;
	    int dodge_object_counter;


	    //DIRECTION VARIABLES
	    bool previous_isDetectingLine;
 	    bool isDetectingLine;
	    bool isGoingInfront;
	    bool isGoingLeft;
	    int counter_left;
	    int endMapCounter;
            bool _90degreesFlag;
            bool isGoingRight;
	    bool noTape_control;

	    bool isLineDetecterActive;

            //MAGNETIC
	    bool isDetectingMagnetic;


            ros::NodeHandle                               m_nd;

            ros::Publisher                                m_pub_goal_vel;
            ros::Publisher                                m_pub_grey;
	    ros::Publisher                                m_pub_light;


	    ros::Subscriber                               m_sub_pid;
	    ros::Subscriber                               m_sub_dodge_objects;
            ros::Subscriber                               m_sub_gui_vel;
	    ros::Subscriber                               m_sub_line;
	    ros::Subscriber                               m_sub_pos;
	    ros::Subscriber                               m_sub_map;
	    ros::Subscriber                               m_sub_lineBool;
	    ros::Subscriber                               m_sub_magneticBool;
            ros::Subscriber                               m_sub_angle;
	    ros::Subscriber                               m_sub_effort;

	    bool isCalibrated();
	    void resetCalibration();
	    void calibrate_light(const float light);

	    void check_endMaze1();
	    void swap_controllers_MapToLine();
	    void swap_controllers_LineToMap();

	    void handle_shadows(const float light);
	    bool isOnShadows;

	    void follow_line(const float light);
	    void handle_maxSpeed();

	    void put_velocity(const int speed_left,const int speed_right);
	    void turn_left();
	    void turn_right();

	    //CALLBACKS
	    void get_pid_result ( const std_msgs::Float64::ConstPtr& result );
	    void get_magnetic_status ( const std_msgs::Bool::ConstPtr& status );
	    void isDetectingLine_clbk(const nxt_msgs::Light::ConstPtr& msg );
	    void maze2_clbk ( const nxt_msgs::Range::ConstPtr& msg/*const nav_msgs::OccupancyGrid::ConstPtr& map*/ );
	    void dodge_object_clbk ( const nxt_msgs::Range::ConstPtr& msg );
	    void get_pos_odom_clbk ( const nav_msgs::Odometry::ConstPtr& msg );
	    void gui_vel_clbk ( const geometry_msgs::Twist::ConstPtr& msg );
            void follow_line_clbk ( const nxt_msgs::Light::ConstPtr& msg );
            void get_gryo_angle_clbk ( const sensor_msgs::Imu::ConstPtr& msg );
	    void last_efforts_clbk( const sensor_msgs::JointState::ConstPtr& msg);
    };
}

#endif
