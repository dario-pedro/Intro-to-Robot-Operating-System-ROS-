
(cl:in-package :asdf)

(defsystem "t2_package-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :t2_package-msg
)
  :components ((:file "_package")
    (:file "Set_Robot_Model" :depends-on ("_package_Set_Robot_Model"))
    (:file "_package_Set_Robot_Model" :depends-on ("_package"))
  ))