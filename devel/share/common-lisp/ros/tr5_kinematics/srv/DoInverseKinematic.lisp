; Auto-generated. Do not edit!


(cl:in-package tr5_kinematics-srv)


;//! \htmlinclude DoInverseKinematic-request.msg.html

(cl:defclass <DoInverseKinematic-request> (roslisp-msg-protocol:ros-message)
  ((pose
    :reader pose
    :initarg :pose
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose)))
)

(cl:defclass DoInverseKinematic-request (<DoInverseKinematic-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DoInverseKinematic-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DoInverseKinematic-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name tr5_kinematics-srv:<DoInverseKinematic-request> is deprecated: use tr5_kinematics-srv:DoInverseKinematic-request instead.")))

(cl:ensure-generic-function 'pose-val :lambda-list '(m))
(cl:defmethod pose-val ((m <DoInverseKinematic-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader tr5_kinematics-srv:pose-val is deprecated.  Use tr5_kinematics-srv:pose instead.")
  (pose m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DoInverseKinematic-request>) ostream)
  "Serializes a message object of type '<DoInverseKinematic-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pose) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DoInverseKinematic-request>) istream)
  "Deserializes a message object of type '<DoInverseKinematic-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pose) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DoInverseKinematic-request>)))
  "Returns string type for a service object of type '<DoInverseKinematic-request>"
  "tr5_kinematics/DoInverseKinematicRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DoInverseKinematic-request)))
  "Returns string type for a service object of type 'DoInverseKinematic-request"
  "tr5_kinematics/DoInverseKinematicRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DoInverseKinematic-request>)))
  "Returns md5sum for a message object of type '<DoInverseKinematic-request>"
  "8124fdf484553a52e7b685afd1f774e4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DoInverseKinematic-request)))
  "Returns md5sum for a message object of type 'DoInverseKinematic-request"
  "8124fdf484553a52e7b685afd1f774e4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DoInverseKinematic-request>)))
  "Returns full string definition for message of type '<DoInverseKinematic-request>"
  (cl:format cl:nil "geometry_msgs/Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DoInverseKinematic-request)))
  "Returns full string definition for message of type 'DoInverseKinematic-request"
  (cl:format cl:nil "geometry_msgs/Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of postion and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DoInverseKinematic-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pose))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DoInverseKinematic-request>))
  "Converts a ROS message object to a list"
  (cl:list 'DoInverseKinematic-request
    (cl:cons ':pose (pose msg))
))
;//! \htmlinclude DoInverseKinematic-response.msg.html

(cl:defclass <DoInverseKinematic-response> (roslisp-msg-protocol:ros-message)
  ((joints
    :reader joints
    :initarg :joints
    :type sensor_msgs-msg:JointState
    :initform (cl:make-instance 'sensor_msgs-msg:JointState)))
)

(cl:defclass DoInverseKinematic-response (<DoInverseKinematic-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DoInverseKinematic-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DoInverseKinematic-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name tr5_kinematics-srv:<DoInverseKinematic-response> is deprecated: use tr5_kinematics-srv:DoInverseKinematic-response instead.")))

(cl:ensure-generic-function 'joints-val :lambda-list '(m))
(cl:defmethod joints-val ((m <DoInverseKinematic-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader tr5_kinematics-srv:joints-val is deprecated.  Use tr5_kinematics-srv:joints instead.")
  (joints m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DoInverseKinematic-response>) ostream)
  "Serializes a message object of type '<DoInverseKinematic-response>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'joints) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DoInverseKinematic-response>) istream)
  "Deserializes a message object of type '<DoInverseKinematic-response>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'joints) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DoInverseKinematic-response>)))
  "Returns string type for a service object of type '<DoInverseKinematic-response>"
  "tr5_kinematics/DoInverseKinematicResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DoInverseKinematic-response)))
  "Returns string type for a service object of type 'DoInverseKinematic-response"
  "tr5_kinematics/DoInverseKinematicResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DoInverseKinematic-response>)))
  "Returns md5sum for a message object of type '<DoInverseKinematic-response>"
  "8124fdf484553a52e7b685afd1f774e4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DoInverseKinematic-response)))
  "Returns md5sum for a message object of type 'DoInverseKinematic-response"
  "8124fdf484553a52e7b685afd1f774e4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DoInverseKinematic-response>)))
  "Returns full string definition for message of type '<DoInverseKinematic-response>"
  (cl:format cl:nil "sensor_msgs/JointState joints~%~%~%================================================================================~%MSG: sensor_msgs/JointState~%# This is a message that holds data to describe the state of a set of torque controlled joints. ~%#~%# The state of each joint (revolute or prismatic) is defined by:~%#  * the position of the joint (rad or m),~%#  * the velocity of the joint (rad/s or m/s) and ~%#  * the effort that is applied in the joint (Nm or N).~%#~%# Each joint is uniquely identified by its name~%# The header specifies the time at which the joint states were recorded. All the joint states~%# in one message have to be recorded at the same time.~%#~%# This message consists of a multiple arrays, one for each part of the joint state. ~%# The goal is to make each of the fields optional. When e.g. your joints have no~%# effort associated with them, you can leave the effort array empty. ~%#~%# All arrays in this message should have the same size, or be empty.~%# This is the only way to uniquely associate the joint name with the correct~%# states.~%~%~%Header header~%~%string[] name~%float64[] position~%float64[] velocity~%float64[] effort~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DoInverseKinematic-response)))
  "Returns full string definition for message of type 'DoInverseKinematic-response"
  (cl:format cl:nil "sensor_msgs/JointState joints~%~%~%================================================================================~%MSG: sensor_msgs/JointState~%# This is a message that holds data to describe the state of a set of torque controlled joints. ~%#~%# The state of each joint (revolute or prismatic) is defined by:~%#  * the position of the joint (rad or m),~%#  * the velocity of the joint (rad/s or m/s) and ~%#  * the effort that is applied in the joint (Nm or N).~%#~%# Each joint is uniquely identified by its name~%# The header specifies the time at which the joint states were recorded. All the joint states~%# in one message have to be recorded at the same time.~%#~%# This message consists of a multiple arrays, one for each part of the joint state. ~%# The goal is to make each of the fields optional. When e.g. your joints have no~%# effort associated with them, you can leave the effort array empty. ~%#~%# All arrays in this message should have the same size, or be empty.~%# This is the only way to uniquely associate the joint name with the correct~%# states.~%~%~%Header header~%~%string[] name~%float64[] position~%float64[] velocity~%float64[] effort~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DoInverseKinematic-response>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'joints))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DoInverseKinematic-response>))
  "Converts a ROS message object to a list"
  (cl:list 'DoInverseKinematic-response
    (cl:cons ':joints (joints msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'DoInverseKinematic)))
  'DoInverseKinematic-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'DoInverseKinematic)))
  'DoInverseKinematic-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DoInverseKinematic)))
  "Returns string type for a service object of type '<DoInverseKinematic>"
  "tr5_kinematics/DoInverseKinematic")