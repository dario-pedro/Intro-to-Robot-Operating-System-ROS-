// Generated by gencpp from file motor_commander/comand_msg_2.msg
// DO NOT EDIT!


#ifndef MOTOR_COMMANDER_MESSAGE_COMAND_MSG_2_H
#define MOTOR_COMMANDER_MESSAGE_COMAND_MSG_2_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace motor_commander
{
template <class ContainerAllocator>
struct comand_msg_2_
{
  typedef comand_msg_2_<ContainerAllocator> Type;

  comand_msg_2_()
    : mode(0)
    , motor(0)
    , position(0)  {
    }
  comand_msg_2_(const ContainerAllocator& _alloc)
    : mode(0)
    , motor(0)
    , position(0)  {
    }



   typedef int32_t _mode_type;
  _mode_type mode;

   typedef uint32_t _motor_type;
  _motor_type motor;

   typedef int32_t _position_type;
  _position_type position;




  typedef boost::shared_ptr< ::motor_commander::comand_msg_2_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::motor_commander::comand_msg_2_<ContainerAllocator> const> ConstPtr;

}; // struct comand_msg_2_

typedef ::motor_commander::comand_msg_2_<std::allocator<void> > comand_msg_2;

typedef boost::shared_ptr< ::motor_commander::comand_msg_2 > comand_msg_2Ptr;
typedef boost::shared_ptr< ::motor_commander::comand_msg_2 const> comand_msg_2ConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::motor_commander::comand_msg_2_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::motor_commander::comand_msg_2_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace motor_commander

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'motor_commander': ['/home/dario/trsa_ws/src/nanotech/motor_commander/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::motor_commander::comand_msg_2_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::motor_commander::comand_msg_2_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::motor_commander::comand_msg_2_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::motor_commander::comand_msg_2_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::motor_commander::comand_msg_2_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::motor_commander::comand_msg_2_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::motor_commander::comand_msg_2_<ContainerAllocator> >
{
  static const char* value()
  {
    return "87a39e2f066462579da8199e99e51a53";
  }

  static const char* value(const ::motor_commander::comand_msg_2_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x87a39e2f06646257ULL;
  static const uint64_t static_value2 = 0x9da8199e99e51a53ULL;
};

template<class ContainerAllocator>
struct DataType< ::motor_commander::comand_msg_2_<ContainerAllocator> >
{
  static const char* value()
  {
    return "motor_commander/comand_msg_2";
  }

  static const char* value(const ::motor_commander::comand_msg_2_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::motor_commander::comand_msg_2_<ContainerAllocator> >
{
  static const char* value()
  {
    return "int32 mode\n\
uint32 motor\n\
int32 position\n\
";
  }

  static const char* value(const ::motor_commander::comand_msg_2_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::motor_commander::comand_msg_2_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.mode);
      stream.next(m.motor);
      stream.next(m.position);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct comand_msg_2_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::motor_commander::comand_msg_2_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::motor_commander::comand_msg_2_<ContainerAllocator>& v)
  {
    s << indent << "mode: ";
    Printer<int32_t>::stream(s, indent + "  ", v.mode);
    s << indent << "motor: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.motor);
    s << indent << "position: ";
    Printer<int32_t>::stream(s, indent + "  ", v.position);
  }
};

} // namespace message_operations
} // namespace ros

#endif // MOTOR_COMMANDER_MESSAGE_COMAND_MSG_2_H