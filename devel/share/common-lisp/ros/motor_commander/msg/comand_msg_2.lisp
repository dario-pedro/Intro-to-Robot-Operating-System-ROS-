; Auto-generated. Do not edit!


(cl:in-package motor_commander-msg)


;//! \htmlinclude comand_msg_2.msg.html

(cl:defclass <comand_msg_2> (roslisp-msg-protocol:ros-message)
  ((mode
    :reader mode
    :initarg :mode
    :type cl:integer
    :initform 0)
   (motor
    :reader motor
    :initarg :motor
    :type cl:integer
    :initform 0)
   (position
    :reader position
    :initarg :position
    :type cl:integer
    :initform 0))
)

(cl:defclass comand_msg_2 (<comand_msg_2>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <comand_msg_2>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'comand_msg_2)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name motor_commander-msg:<comand_msg_2> is deprecated: use motor_commander-msg:comand_msg_2 instead.")))

(cl:ensure-generic-function 'mode-val :lambda-list '(m))
(cl:defmethod mode-val ((m <comand_msg_2>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motor_commander-msg:mode-val is deprecated.  Use motor_commander-msg:mode instead.")
  (mode m))

(cl:ensure-generic-function 'motor-val :lambda-list '(m))
(cl:defmethod motor-val ((m <comand_msg_2>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motor_commander-msg:motor-val is deprecated.  Use motor_commander-msg:motor instead.")
  (motor m))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <comand_msg_2>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader motor_commander-msg:position-val is deprecated.  Use motor_commander-msg:position instead.")
  (position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <comand_msg_2>) ostream)
  "Serializes a message object of type '<comand_msg_2>"
  (cl:let* ((signed (cl:slot-value msg 'mode)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'motor)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'motor)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'motor)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'motor)) ostream)
  (cl:let* ((signed (cl:slot-value msg 'position)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <comand_msg_2>) istream)
  "Deserializes a message object of type '<comand_msg_2>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'mode) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'motor)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'motor)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'motor)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'motor)) (cl:read-byte istream))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'position) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<comand_msg_2>)))
  "Returns string type for a message object of type '<comand_msg_2>"
  "motor_commander/comand_msg_2")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'comand_msg_2)))
  "Returns string type for a message object of type 'comand_msg_2"
  "motor_commander/comand_msg_2")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<comand_msg_2>)))
  "Returns md5sum for a message object of type '<comand_msg_2>"
  "87a39e2f066462579da8199e99e51a53")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'comand_msg_2)))
  "Returns md5sum for a message object of type 'comand_msg_2"
  "87a39e2f066462579da8199e99e51a53")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<comand_msg_2>)))
  "Returns full string definition for message of type '<comand_msg_2>"
  (cl:format cl:nil "int32 mode~%uint32 motor~%int32 position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'comand_msg_2)))
  "Returns full string definition for message of type 'comand_msg_2"
  (cl:format cl:nil "int32 mode~%uint32 motor~%int32 position~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <comand_msg_2>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <comand_msg_2>))
  "Converts a ROS message object to a list"
  (cl:list 'comand_msg_2
    (cl:cons ':mode (mode msg))
    (cl:cons ':motor (motor msg))
    (cl:cons ':position (position msg))
))
