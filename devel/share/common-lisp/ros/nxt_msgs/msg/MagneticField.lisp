; Auto-generated. Do not edit!


(cl:in-package nxt_msgs-msg)


;//! \htmlinclude MagneticField.msg.html

(cl:defclass <MagneticField> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (value
    :reader value
    :initarg :value
    :type cl:float
    :initform 0.0))
)

(cl:defclass MagneticField (<MagneticField>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <MagneticField>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'MagneticField)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name nxt_msgs-msg:<MagneticField> is deprecated: use nxt_msgs-msg:MagneticField instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <MagneticField>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nxt_msgs-msg:header-val is deprecated.  Use nxt_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'value-val :lambda-list '(m))
(cl:defmethod value-val ((m <MagneticField>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nxt_msgs-msg:value-val is deprecated.  Use nxt_msgs-msg:value instead.")
  (value m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <MagneticField>) ostream)
  "Serializes a message object of type '<MagneticField>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'value))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <MagneticField>) istream)
  "Deserializes a message object of type '<MagneticField>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'value) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<MagneticField>)))
  "Returns string type for a message object of type '<MagneticField>"
  "nxt_msgs/MagneticField")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'MagneticField)))
  "Returns string type for a message object of type 'MagneticField"
  "nxt_msgs/MagneticField")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<MagneticField>)))
  "Returns md5sum for a message object of type '<MagneticField>"
  "d053817de0764f9ee90dbc89c4cdd751")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'MagneticField)))
  "Returns md5sum for a message object of type 'MagneticField"
  "d053817de0764f9ee90dbc89c4cdd751")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<MagneticField>)))
  "Returns full string definition for message of type '<MagneticField>"
  (cl:format cl:nil "Header header~%float64 value~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'MagneticField)))
  "Returns full string definition for message of type 'MagneticField"
  (cl:format cl:nil "Header header~%float64 value~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <MagneticField>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <MagneticField>))
  "Converts a ROS message object to a list"
  (cl:list 'MagneticField
    (cl:cons ':header (header msg))
    (cl:cons ':value (value msg))
))
