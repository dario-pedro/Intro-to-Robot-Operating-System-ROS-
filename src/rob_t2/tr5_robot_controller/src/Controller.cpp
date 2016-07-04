#include "tr5_robot_controller/Controller.hpp"
#include "tr5_kinematics/Kinematics.hpp"
using namespace TR5;


Controller::Controller()

    : m_nd( "~" ), m_marker_server( "input_pose" ), m_inverse_mode( true ) , intelligent_mode(false)

{
    std::string tp_sel_kin, tp_gui_jt, tp_goal_jt, tp_goal_ps, tp_inpt_ps;
    std::string tp_inv_k, tp_for_k, tp_int_k,tp_for_in_ps;

    // Load private parameters from Parameter Server
    m_nd.param<std::string>("select_kin_topic",    tp_sel_kin, "/kin_mode"   );
    m_nd.param<std::string>("gui_joints_topic",    tp_gui_jt,  "/gui_joints" );
    m_nd.param<std::string>("goal_joints_topic",   tp_goal_jt, "/goal_joints");
    m_nd.param<std::string>("goal_pose_topic",     tp_goal_ps, "/goal_pose"  );
    m_nd.param<std::string>("inverse_kinet_topic", tp_inv_k,   "/do_ik"      );
    m_nd.param<std::string>("forward_kinet_topic", tp_for_k,   "/do_fk"      );
    m_nd.param<std::string>("intelligent_kinet_topic", tp_int_k,   "/do_int"      );
    m_nd.param<std::string>("input_pose_topic", tp_for_in_ps, "/input_pose/feedback");

    if ( !m_nd.hasParam("select_kin_topic") )
        ROS_WARN("No param 'select_kin_topic'\tDefault: %s",  tp_sel_kin.c_str());
    if ( !m_nd.hasParam("gui_joints_topic") )
        ROS_WARN("No param 'gui_joints_topic'\tDefault: %s",  tp_gui_jt.c_str());
    if ( !m_nd.hasParam("goal_joints_topic") )
        ROS_WARN("No param 'goal_joints_topic'\tDefault: %s", tp_goal_jt.c_str());
    if ( !m_nd.hasParam("goal_pose_topic") )
        ROS_WARN("No param 'goal_pose_topic'\tDefault: %s",   tp_goal_ps.c_str());
    if ( !m_nd.hasParam("forward_kinet_topic") )
        ROS_WARN("No param 'forward_kinet_topic'\tDefault: %s", tp_for_k.c_str());
    if ( !m_nd.hasParam("inverse_kinet_topic") )
        ROS_WARN("No param 'inverse_kinet_topic'\tDefault: %s", tp_inv_k.c_str());
    if ( !m_nd.hasParam("intelligent_kinet_topic") )
        ROS_WARN("No param 'intelligent_kinet_topic'\tDefault: %s", tp_int_k.c_str());
    if ( !m_nd.hasParam("input_pose_topic") )
        ROS_WARN("No param 'input_pose_topic'\tDefault: %s", tp_for_in_ps.c_str());

    // visual marker object for RVIZ
    m_marker.ns              = "forward kinematics marker";
    m_marker.id              = 0;
    m_marker.header.stamp    = ros::Time();
    m_marker.header.frame_id = "/world";
    m_marker.color.a         = 0.5;
    m_marker.color.r         = 1.0;
    m_marker.color.b         = m_marker.color.g = 0.0;
    m_marker.scale.x         = m_marker.scale.y = m_marker.scale.z = 0.05;
    m_marker.action          = visualization_msgs::Marker::ADD;
    m_marker.type            = visualization_msgs::Marker::SPHERE;
    m_marker.lifetime        = ros::Duration(1.0);

    // initialise RVIZ's interactive marker to use in inverse kinematics mode
    create_interactive_marker();

    //
    // TODO initialise ROS service and message publishers/subscribers
    //

    m_sub_gui_jt = m_nd.subscribe( tp_gui_jt, 1, &Controller::gui_joints_clbk, this );
    m_sub_pose = m_nd.subscribe(tp_for_in_ps,1,&Controller::input_pose_clbk,this);

    m_pub_goal_jt = m_nd.advertise<sensor_msgs::JointState>("/goal_joints", 1000);
    m_pub_goal_ps = m_nd.advertise<visualization_msgs::Marker>("/goal_pose", 1000);

    m_client_fk =  m_nd.serviceClient<tr5_kinematics::DoForwardKinematic>("/do_fk");
    m_client_ik =  m_nd.serviceClient<tr5_kinematics::DoInverseKinematic>("/do_ik");
    m_client_int =  m_nd.serviceClient<tr5_kinematics::DoInverseKinematic>("/do_int");

    kinMode= m_nd.advertiseService(tp_sel_kin, &Controller::Kin_mode, this);
}


void Controller::run()
{
    ros::spin();
}

bool Controller::Kin_mode(tr5_robot_controller::KinematicMode::Request  &req,
                         tr5_robot_controller::KinematicMode::Response &res){

      if(req.mode==0){
        m_inverse_mode=false;
        intelligent_mode=false;
        ROS_INFO("FORWARD MODE");
      }
      else if(req.mode==1){
        m_inverse_mode=true;
        intelligent_mode=false;
        ROS_INFO("INVERSE MODE");
      }
      else if(req.mode==2){
        m_inverse_mode=true;
        intelligent_mode=true;
        ROS_INFO("INTELEGENT MODE");
      }
      else{
        ROS_WARN("NO MODE SELECTED - OLD ONE ON USE");
      }

    res.result = true;
    return true;
}

void Controller::gui_joints_clbk ( const sensor_msgs::JointState::ConstPtr& msg )
{
    // ignore values from JointState GUI when in inverse kinematics mode
    if ( m_inverse_mode )
        return;

    tr5_kinematics::DoForwardKinematic srv;
    for(int i=0;i<7;i++){
      srv.request.joints.name.push_back(msg->name[i]);
      srv.request.joints.position.push_back(msg->position[i]);
    }


    if (m_client_fk.call(srv))
    {
      m_marker.pose.position=srv.response.pose.position;
      m_pub_goal_ps.publish( m_marker );

    }
    else
        ROS_ERROR( "Failed to call service FORWARD KINEMATICS" );

    //
    // TODO publish joint values from JointState GUI to the TR5 driver node
    //
    m_pub_goal_jt.publish(msg);
}




void Controller::input_pose_clbk( const IMarkerConstPtr &feedback )
{
    // ignore feedback from interactive marker when in forward kinematics mode
    if ( !m_inverse_mode )
        return;

    tr5_kinematics::DoInverseKinematic srv;
    srv.request.pose.position.x = feedback->pose.position.x + 0.55;
    srv.request.pose.position.y = feedback->pose.position.y;
    srv.request.pose.position.z = feedback->pose.position.z + 0.27;

    if(intelligent_mode){
      if (m_client_int.call(srv))
      {
        m_pub_goal_jt.publish(srv.response.joints);
      }
      else
          ROS_ERROR( "Failed to call service INTELEGENT KINEMATICS" );
    }
    else{
      if (m_client_ik.call(srv))
      {
        m_pub_goal_jt.publish(srv.response.joints);
      }
      else
          ROS_ERROR( "Failed to call service INVERSE KINEMATICS" );
    }
}




void Controller::create_interactive_marker()
{
    // create an interactive marker
    m_imarker.header.frame_id = "/world";
    m_imarker.header.stamp    = ros::Time::now();
    m_imarker.name            = "user_input_marker";
    m_imarker.description     = "Input Pose";

    // create a grey box marker
    visualization_msgs::Marker dot_marker;
    dot_marker.type = visualization_msgs::Marker::SPHERE;
    dot_marker.scale.x = dot_marker.scale.y = dot_marker.scale.z = 0.05;
    dot_marker.color.r = dot_marker.color.g = 0.0;
    dot_marker.color.b = 1.0;
    dot_marker.color.a = 0.5;
    dot_marker.pose.position.x = 0.55;
    dot_marker.pose.position.y = 0.0;
    dot_marker.pose.position.z = 0.27;

    // create a non-interactive control which contains the dot marker
    visualization_msgs::InteractiveMarkerControl dot_control;
    dot_control.always_visible = true;
    dot_control.markers.push_back( dot_marker );
    dot_control.orientation_mode =
                         visualization_msgs::InteractiveMarkerControl::FIXED;
    dot_control.interaction_mode =
                         visualization_msgs::InteractiveMarkerControl::MOVE_3D;

    // add the control to the interactive marker
    m_imarker.controls.push_back( dot_control );

    m_marker_server.insert( m_imarker,
                        boost::bind( &Controller::input_pose_clbk, this, _1 ) );

    m_marker_server.applyChanges();
}
