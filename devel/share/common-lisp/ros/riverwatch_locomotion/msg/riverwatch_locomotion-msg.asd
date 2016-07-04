
(cl:in-package :asdf)

(defsystem "riverwatch_locomotion-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "PidControllerState" :depends-on ("_package_PidControllerState"))
    (:file "_package_PidControllerState" :depends-on ("_package"))
  ))