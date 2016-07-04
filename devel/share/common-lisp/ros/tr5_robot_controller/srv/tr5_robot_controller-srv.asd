
(cl:in-package :asdf)

(defsystem "tr5_robot_controller-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "KinematicMode" :depends-on ("_package_KinematicMode"))
    (:file "_package_KinematicMode" :depends-on ("_package"))
  ))