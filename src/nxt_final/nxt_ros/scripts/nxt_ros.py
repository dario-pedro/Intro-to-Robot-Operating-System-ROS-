#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# This code was modified and ported to ROS Jade distro by Ricardo Mendonca
# E-mail:  rmm@uninova.pt
# Website: http://rics.uninova.pt
#
# Copyright (c) 2010, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#



import roslib; roslib.load_manifest('nxt_ros')
import nxt.locator
import rospy
import math
import thread
import time
import nxt.sensor
import nxt.motor
import nxt.telegram

from nxt.motor import PORT_A, PORT_B, PORT_C
from nxt.sensor import PORT_1, PORT_2, PORT_3, PORT_4, Type
from nxt.sensor import hitechnic
from nxt.motcont import MotCont
from sensor_msgs.msg import JointState, Imu
from std_msgs.msg import Bool
from nxt_msgs.msg import Range, Contact, Compass, Color, Light, MagneticField, Gyro, Accelerometer, Sound
from PyKDL import Rotation


#global
global my_lock
my_lock = thread.allocate_lock()




def check_params(ns, params):
    for p in params:
        if not rospy.get_param(ns+'/'+p):
            return False
    return True




def string2port(port_string):

    if port_string == "PORT_A":
        port = PORT_A
    elif port_string == "PORT_B":
        port = PORT_B
    elif port_string == "PORT_C":
        port = PORT_C
    else:
        raise Exception('Invalid NXT actuator port')
    return port




# base class for sensors
class Device:

    def __init__(self, name, frequency):
        self.desired_period = 1.0 / frequency
        self.period = self.desired_period
        self.initialized = False
        self.name = name


    def needs_trigger(self):
        # initialize
        if not self.initialized:
            self.initialized = True
            self.last_run = rospy.Time.now()
            rospy.logdebug('Initializing %s'%self.name)
            return False
        # compute frequency
        now = rospy.Time.now()
        period = 0.9 * self.period + 0.1 * (now - self.last_run).to_sec()
        # check period
        #if period > self.desired_period * 1.2:
        #    rospy.logwarn("%s not reaching desired frequency: actual %f, desired %f"%(self.name, 1.0/period, 1.0/self.desired_period))
        #elif period > self.desired_period * 1.5:
        #    rospy.logerr("%s not reaching desired frequency: actual %f, desired %f"%(self.name, 1.0/period, 1.0/self.desired_period))

        return period > self.desired_period


    def do_trigger(self):
        try:
          rospy.logdebug('Trigger %s with current frequency %f'%(self.name, 1.0/self.period))
          now = rospy.Time.now()
          self.period = 0.9 * self.period + 0.1 * (now - self.last_run).to_sec()
          self.last_run = now
          self.trigger()
          rospy.logdebug('Trigger %s took %f mili-seconds'%(self.name, (rospy.Time.now() - now).to_sec()*1000))
        except nxt.error.DirProtError:
          rospy.logwarn("caught an exception nxt.error.DirProtError")
          pass
        except nxt.error.I2CError:
          rospy.logwarn("caught an exception nxt.error.I2CError")
          pass




class JointStateAggregator(Device):

    def __init__(self, comm, frame_id, frequency):

        Device.__init__(self, "joint_state_aggregator", frequency)

        self.brick  = comm
        self.joints = []
        self.prev_rot = []
        self.last_js  = None
        self.frame_id = 'nxt/' + frame_id
        self.pub = rospy.Publisher('nxt/joint_states', JointState, queue_size=1)


    def add_joint(self, name, port):

        #if self.joints...:
            self.joints.append( nxt.motor.Motor(self.brick, port, 'nxt/' + name) )
            rospy.loginfo( "nxt/%s was registered by JoinState aggregator" % name )
        #else:
        #    rospy.logwarn( "Joint already added to JointState aggregator" )


    #def remove_joint(self, name):



    def trigger(self):

        if len(self.joints) > 0:

            js = JointState()
            js.header.stamp = rospy.Time.now()
            js.header.frame_id = self.frame_id
            i = 0

            for joint in self.joints:

                state, tacho_info = joint._read_state()
                js.name.append( joint.name )
                new_rot = tacho_info.rotation_count * math.pi / 180.0
                js.position.append( new_rot )
                js.effort.append( state.power )

                if self.last_js:
                    rot_diff = new_rot - self.prev_rot[i]
                    vel = rot_diff / (js.header.stamp - self.last_js.header.stamp).to_sec()
                    js.velocity.append(vel)
                    self.prev_rot[i] = new_rot
                else:
                    js.velocity.append(0)
                    self.prev_rot.append( new_rot )

                i = i+1

            self.last_js = js
            self.pub.publish(js)




class SingleMotor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.name        = 'nxt/' + params['name']
        self.tacho_limit = params['tacho_limit']
        if (self.tacho_limit > 999999) or (self.tacho_limit < 0):
            rospy.logerror('Invalid tacho limit value')
            raise Exception('Tacho limit value must be in the range of [0,999999] ')
        self.port        = eval(params['port'])
        self.brick       = comm
        self.cmd         = 0
        self.sub = rospy.Subscriber('nxt/joint_command', JointState, self.cmd_cb, None, 1)


    def cmd_cb(self, msg):

        for e in msg.name:
            if self.name == e:
                self.cmd = int( msg.effort[ msg.name.index(e) ] )
                if self.cmd > 100:
                    self.cmd = 100
                elif self.cmd < -100:
                    self.cmd = -100
                return


    def trigger(self):

        if self.tacho_limit == 0:
            self.brick.motor_control.set_output_state( self.port, self.cmd, 0 )
        else:
            self.brick.motor_control.cmd(self.port, self.cmd, self.tacho_limit)




class DifferentialMotor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])

        if var['motor_1_name'] == var['motor_2_name']:
            raise Exception('Duplicated names in differential motors')

        self.brick = comm
        self.sub  = rospy.Subscriber('nxt/joint_command', JointState, self.cmd_cb, None, 1)
        self.ports = {}
        self.commands = {}
        self.tacho_limit = var['tacho_limit']
        if (self.tacho_limit > 999999) or (self.tacho_limit < 0):
            rospy.logerror('Invalid tacho limit value')
            raise Exception('Tacho limit value must be in the range of [0,999999] ')
        self.ports[ 'nxt/' + var['motor_1_name'] ] = string2port( var['motor_1_port'] )
        self.ports[ 'nxt/' + var['motor_2_name'] ] = string2port( var['motor_2_port'] )
        self.commands[ 'nxt/' + var['motor_1_name'] ] = 0
        self.commands[ 'nxt/' + var['motor_2_name'] ] = 0
        self.all_ports = self.ports[ 'nxt/' + var['motor_1_name'] ] + self.ports[ 'nxt/' + var['motor_2_name'] ] + 2


    def cmd_cb(self, msg):

        for i in range(len(msg.name)):
            if self.ports.has_key(msg.name[i]):
                cmd = int(msg.effort[i])
                if cmd > 100:
                    cmd = 100
                elif cmd < -100:
                    cmd = -100
                self.commands[ msg.name[i] ] = cmd


    def trigger(self):

        sync = False
        cmd      = 99999

        for key in self.ports:
            if (self.commands[key] == cmd):
                sync = True
            else:
                sync = False
                cmd = self.commands[key]

        if sync:
            if self.tacho_limit == 0:
                self.brick.motor_control.set_output_state( self.all_ports, cmd, 0 )
            else:
                self.brick.motor_control.cmd( self.all_ports, cmd, self.tacho_limit )
        else:
            if self.tacho_limit == 0:
                for key in self.ports:
                    self.brick.motor_control.set_output_state( self.ports[key], self.commands[key], 0 )
            else:
                for key in self.ports:
                    self.brick.motor_control.cmd( self.ports[key], self.commands[key], self.tacho_limit )




class SoundSensor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.sound = nxt.sensor.Sound(comm, eval(params['port']))
        self.frame_id = 'nxt/' + params['frame_id']
        self.pub = rospy.Publisher("nxt/"+params['name'], Sound, queue_size=1)


    def trigger(self):

        sd = Sound()
        sd.loudness = self.sound.get_sample()
        sd.header.frame_id = self.frame_id
        sd.header.stamp = rospy.Time.now()
        self.pub.publish(sd)




class TouchSensor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.touch = nxt.sensor.Touch(comm, eval(params['port']))
        self.frame_id = 'nxt/' + params['frame_id']
        self.pub = rospy.Publisher("nxt/"+params['name'], Contact, queue_size=1)


    def trigger(self):

        ct = Contact()
        ct.contact = self.touch.get_sample()
        ct.header.frame_id = self.frame_id
        ct.header.stamp = rospy.Time.now()
        self.pub.publish(ct)




class UltraSonicSensor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.ultrasonic = nxt.sensor.generic.Ultrasonic(comm, eval(params['port']))
        self.frame_id = 'nxt/' + params['frame_id']
        self.spread = params['spread_angle']
        self.min_range = params['min_range']
        self.max_range = params['max_range']
        self.pub = rospy.Publisher("nxt/"+params['name'], Range, queue_size=1)


    def trigger(self):

        ds = Range()
        ds.header.frame_id = self.frame_id
        ds.header.stamp = rospy.Time.now()
        ds.range = self.ultrasonic.get_sample()/100.0
        ds.spread_angle = self.spread
        ds.range_min = self.min_range
        ds.range_max = self.max_range
        self.pub.publish(ds)




class GyroSensor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.gyro = nxt.sensor.hitechnic.Gyro(comm, eval(params['port']))
        self.frame_id = 'nxt/' + params['frame_id']
        self.orientation = 0.0
        self.offset = 0.0
        self.prev_time = rospy.Time.now()
        self.calibrated = False
        #self.pub  = rospy.Publisher("nxt/" + params['name'], Gyro, queue_size=1)
        self.pub2 = rospy.Publisher("nxt/" + params['name']+"/imu", Imu, queue_size=1)
        self.gyro.get_sample()


    def trigger(self):

        if not self.calibrated:
            rospy.logwarn("Calibrating Gyro. Don't move the robot now!")
            time.sleep(5)
            self.offset = self.gyro.get_sample()
            self.calibrated = True
            rospy.loginfo("Gyro calibrated with offset %f"%self.offset)

        sample = self.gyro.get_sample()
        sample = (sample - self.offset) * math.pi / 180.0
        if abs(sample) < 0.03:
            sample = 0.0

        #gs = Gyro()
        #gs.header.frame_id = self.frame_id
        #gs.header.stamp = rospy.Time.now()
        #gs.calibration_offset.x = 0.0
        #gs.calibration_offset.y = 0.0
        #gs.calibration_offset.z = self.offset
        #gs.angular_velocity.x = 0.0
        #gs.angular_velocity.y = 0.0
        #gs.angular_velocity.z = sample
        #gs.angular_velocity_covariance = [0, 0, 0, 0, 0, 0, 0, 0, 1]
        #self.pub.publish(gs)

        imu = Imu()
        imu.header.frame_id = self.frame_id
        imu.header.stamp = rospy.Time.now()
        imu.angular_velocity.x = 0.0
        imu.angular_velocity.y = 0.0
        imu.angular_velocity.z = sample
        imu.angular_velocity_covariance = [0, 0, 0, 0, 0, 0, 0, 0, 1]
        imu.orientation_covariance = [0.001, 0, 0, 0, 0.001, 0, 0, 0, 0.1]
        self.orientation += imu.angular_velocity.z * (imu.header.stamp - self.prev_time).to_sec()
        self.prev_time = imu.header.stamp
        (imu.orientation.x, imu.orientation.y, imu.orientation.z, imu.orientation.w) = Rotation.RotZ(self.orientation).GetQuaternion()
        self.pub2.publish(imu)



"""
HiTechnic Compass sensor
================================================================================
The NXT Compass Sensor is a digital compass that measures the earth's magnetic
field and outputs a value representing the current heading. The magnetic heading
is calculated to the nearest 1 degree and returned as a number from 0 to 359. The NXT
Compass Sensor updates the heading 100 times per second. The Compass Sensor
operates in two modes, Read mode and Calibrate mode. In Read mode, the current
heading is calculated and returned each time to the NXT program executes a read
command. In Calibrate mode the compass can be calibrated to compensate for
externally generated magnetic field anomalies such as those that surround motors
and batteries, thereby maintaining maximum accuracy.
"""

class CompassSensor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])

        self.sensor   = hitechnic.Compass(comm, eval(var['port']))
        self.frame_id = 'nxt/' + var['frame_id']
        self.pub = rospy.Publisher("nxt/"+var['name'], Compass, queue_size=1)

        # calibrate
        #rospy.logwarn("Calibrating Magnetic sensor...")
        #offset = self.sensor.get_magnetic_value()
        #self.sensor.calibrate()
        #rospy.loginfo("Magnetic sensor calibrated with offset %f"%offset)


    def trigger(self):

        cp = Compass()
        cp.header.frame_id = self.frame_id
        cp.header.stamp = rospy.Time.now()
        cp.heading = self.sensor.get_sample()
        self.pub.publish(cp)




class AccelerometerSensor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])

        self.accel    = hitechnic.Accelerometer(comm, eval(var['port']))
        self.frame_id = 'nxt/' + var['frame_id']
        self.pub = rospy.Publisher("nxt/"+var['name'], Accelerometer, queue_size=1)


    def trigger(self):

        gs = Accelerometer()
        gs.header.frame_id = self.frame_id
        gs.header.stamp = rospy.Time.now()
        acc = self.accel.get_sample()
        gs.linear_acceleration.x = acc.x * 9.8
        gs.linear_acceleration.y = acc.y * 9.8
        gs.linear_acceleration.z = acc.z * 9.8
        gs.linear_acceleration_covariance = [1, 0, 0, 0, 1, 0, 0, 0, 1]
        self.pub.publish(gs)




"""
HiTechnic EOPD sensor
================================================================================
Accurately detect objects and small changes in distance to a target.
The EOPD or Electro Optical Proximity Detector uses an internal light source to
detect the presence of a target or determine changes in distance to a target.
By generating its own light source, the EOPD is able to filter out all external
light signals so as a robot moves from bright areas to dimmly lit areas or
through shadows, the EOPD automatically compensates and only returns a signal
based on its own light source. Targets can be accurately detected over distances
up to about 20 cm, depending on the target size, shape and reflective
qualities.

The EOPD sensor has two modes of operation, x1 sensitivity and x4 sensitivity.
In x4 sensitivity mode, the HiTechnic EOPD can easily detect the red ball which
comes with the LEGO MINDSTORMS NXT set at a range of at least 15 cm. Since the
sensor can be overloaded by close white targets, the x1 sensitivity mode can be
used for detecting the position of LEGO pieces, such as a small white part
attached to a black beam, used as a mechanism home detection device.
================================================================================
"""

class EOPDSensor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])

        self.sensor   = hitechnic.EOPD(comm, eval(var['port']))
        self.frame_id = 'nxt/' + var['frame_id']
        self.pub = rospy.Publisher("nxt/"+var['name'], Range, queue_size=1)

        if not var['short_range']:
            self.sensor.set_range_long()
        else:
            self.sensor.set_range_short()


    def trigger(self):

        ds = Range()
        ds.header.frame_id = self.frame_id
        ds.header.stamp = rospy.Time.now()
        ds.range = self.sensor.get_sample()
        #ds.range_min = self.min_range
        #ds.range_max = self.max_range
        self.pub.publish(ds)




"""
HiTechnic Magnetic sensor
================================================================================
The NXT Magnetic Sensor will enable you to build robots that can detect magnetic
fields. The sensor detects magnetic fields that are present around the front of
the sensor in a vertical orientation.
================================================================================
"""

class MagneticSensor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])
        self.sensor   = hitechnic.Magnetic(comm, eval(var['port']))
        self.frame_id = 'nxt/' + var['frame_id']
        self.pub = rospy.Publisher("nxt/"+var['name'], MagneticField, queue_size=1)

        # calibrate
        rospy.logwarn("Calibrating Magnetic sensor...")
        time.sleep(3.0)
        offset = self.sensor.get_magnetic_value()
        self.sensor.calibrate()
        rospy.loginfo("Magnetic sensor calibrated with offset %f"%offset)


    def trigger(self):

        mf = MagneticField()
        mf.header.frame_id = self.frame_id
        mf.header.stamp    = rospy.Time.now()
        mf.value           = self.sensor.get_sample()
        self.pub.publish( mf )




class ColorSensor(Device):

    def __init__(self, var, comm):

        Device.__init__(self, var['name'], var['desired_frequency'])

        self.sensor   = nxt.sensor.Color(comm, eval(var['port']))
        self.frame_id = 'nxt/' + var['frame_id']

        if not var['light_only']:
            self.sensor.set_light_color( Type.COLORFULL )
            rospy.loginfo("\tColor sensor in RGB mode")
        else:
            rospy.loginfo("\tColor sensor in LIGHT mode")
            if   var['red'] == 1.0 and var['green'] == 0.0 and var['blue'] == 0.0:
                rospy.loginfo("\tDetecting RED color...")
                self.sensor.set_light_color( Type.COLORRED )
            elif var['red'] == 0.0 and var['green'] == 1.0 and var['blue'] == 0.0:
                rospy.loginfo("\tDetecting GREEN color...")
                self.sensor.set_light_color( Type.COLORGREEN )
            elif var['red'] == 0.0 and var['green'] == 0.0 and var['blue'] == 1.0:
                rospy.loginfo("\tDetecting BLUE color...")
                self.sensor.set_light_color( Type.COLORBLUE )
            elif var['red'] == 0.0 and var['green'] == 0.0 and var['blue'] == 0.0:
                rospy.loginfo("\tDetecting WHITE color...")
                self.sensor.set_light_color( Type.COLORNONE )
            else:
                rospy.logerr('Invalid RGB values specified for color sensor')
                rospy.logwarn('Resetting RGB values to 0 for color sensor')

        # create ROS publisher
        self.pub = rospy.Publisher( "nxt/"+var['name'], Color, queue_size=1 )


    def trigger(self):

        co = Color()
        co.header.frame_id = self.frame_id
        co.header.stamp    = rospy.Time.now()
        co.r = co.g = co.b = 0.0

        if self.sensor.light_mode:
            co.intensity = self.sensor.get_reflected_light()
            color_light  = self.sensor.get_light_color()
            if color_light == Type.COLORRED:
                co.r = 1.0
                co.g = 0.0
                co.b = 0.0
            elif color_light == Type.COLORGREEN:
                co.r = 0.0
                co.g = 1.0
                co.b = 0.0
            elif color_light == Type.COLORBLUE:
                co.r = 0.0
                co.g = 0.0
                co.b = 1.0
            else:
                co.r = 1.0
                co.g = 1.0
                co.b = 1.0
        else:
            co.intensity = 0.0
            color = self.sensor.get_color()
            if color == 1:  # black
                co.r = 0.0
                co.g = 0.0
                co.b = 0.0
            elif color == 2: # blue
                co.r = 0.0
                co.g = 0.0
                co.b = 1.0
            elif color == 3: # green
                co.r = 0.0
                co.g = 1.0
                co.b = 0.0
            elif color == 4: # yellow
                co.r = 1.0
                co.g = 1.0
                co.b = 0.0
            elif color == 5: # red
                co.r = 1.0
                co.g = 0.0
                co.b = 1.0
            elif color == 6: # white
                co.r = 1.0
                co.g = 1.0
                co.b = 1.0
            else:
                rospy.logerr('Undefined color detected by the Color sensor')

        self.pub.publish(co)




class IntensitySensor(Device):

    def __init__(self, params, comm):

        Device.__init__(self, params['name'], params['desired_frequency'])

        self.sensor = nxt.sensor.Light(comm, eval(params['port']))
        self.sensor.set_illuminated(True)
        self.frame_id  = 'nxt/' + params['frame_id']
        self.pub = rospy.Publisher("nxt/"+params['name'], Light, queue_size=1)


    def trigger(self):

        lg = Light()
        lg.header.frame_id = self.frame_id
        lg.header.stamp    = rospy.Time.now()
        lg.intensity       = self.sensor.get_lightness()
        self.pub.publish(lg)





def main():

    ns = 'nxt_robot'
    rospy.init_node( ns )
    host = rospy.get_param( "~host", None )

    b = nxt.locator.find_one_brick(host)
    b.motor_control.start()
    #b.motor_control.reset_tacho(6)

    max_sensor_frequency = 0.1

    components = []
    config = rospy.get_param( "~"+ns, None )

    if config == None:
        rospy.logwarn("ROS Parameter not found: ~%s" % ns)
        rospy.logerr("Aborting ROS node execution")
        return
    else:
        components.append( JointStateAggregator( b, "base_footprint", 5.0 ) )

        for c in config:
            if  c['type'] == 'single_motor':
                components.append( SingleMotor( c, b ) )
                components[0].add_joint( c['name'], eval(c['port']) )
            elif c['type'] == 'differential_motor':
                components.append( DifferentialMotor( c, b ) )
                components[0].add_joint( c['motor_1_name'], eval(c['motor_1_port']) )
                components[0].add_joint( c['motor_2_name'], eval(c['motor_2_port']) )
            elif c['type'] == 'touch':
                components.append( TouchSensor( c, b ) )
            elif c['type'] == 'ultrasonic':
                components.append( UltraSonicSensor( c, b ) )
            elif c['type'] == 'sound':
                components.append( SoundSensor( c, b ) )
            elif c['type'] == 'intensity':
                components.append( IntensitySensor( c, b ) )
            elif c['type'] == 'gyro':
                components.append( GyroSensor( c, b ) )
            elif c['type'] == 'color':
                components.append( ColorSensor( c, b ) )
            elif c['type'] == 'eopd':
                components.append( EOPDSensor( c, b ) )
            elif c['type'] == 'magnetic':
                components.append( MagneticSensor( c, b ) )
            elif c['type'] == 'accelerometer':
                components.append( AccelerometerSensor( c, b ) )
            elif c['type'] == 'compass':
                components.append( CompassSensor( c, b ) )
            else:
                rospy.logerr('Invalid sensor/actuator type %s'%c['type'])

            rospy.loginfo("Registered %s device as %s" % (c['type'], c['name']) )

            if c['desired_frequency'] > max_sensor_frequency:
                max_sensor_frequency = c['desired_frequency']

    node_freq = max_sensor_frequency + 10.0
    rospy.loginfo("Node frequency: %f"%node_freq)
    r = rospy.Rate( node_freq )

    while not rospy.is_shutdown():

        # Send KeepAlive packet to NXT brick
        b.sock.send( nxt.direct.keep_alive(0x0D).pkt.getvalue() )
        b.sock.recv()

        my_lock.acquire()
        for c in components:
            if c.needs_trigger():
                c.do_trigger()
        my_lock.release()
        r.sleep()

    del components
    b.motor_control.stop()

if __name__ == '__main__':
    main()
