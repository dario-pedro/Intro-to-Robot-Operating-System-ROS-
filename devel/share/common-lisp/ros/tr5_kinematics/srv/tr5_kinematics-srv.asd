
(cl:in-package :asdf)

(defsystem "tr5_kinematics-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :sensor_msgs-msg
)
  :components ((:file "_package")
    (:file "DoForwardKinematic" :depends-on ("_package_DoForwardKinematic"))
    (:file "_package_DoForwardKinematic" :depends-on ("_package"))
    (:file "DoInverseKinematic" :depends-on ("_package_DoInverseKinematic"))
    (:file "_package_DoInverseKinematic" :depends-on ("_package"))
  ))