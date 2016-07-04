
(cl:in-package :asdf)

(defsystem "t2_package-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "robot_id" :depends-on ("_package_robot_id"))
    (:file "_package_robot_id" :depends-on ("_package"))
  ))