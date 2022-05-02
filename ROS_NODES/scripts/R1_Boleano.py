#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from std_msgs.msg import Int16
from std_msgs.msg import Bool
from std_msgs.msg import Float32
retorna1 = None

def callback(data):
    #rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)
    global retorna1
    retorna1 = data.data

def listener():

    rospy.init_node('BOLEANO1', anonymous=False)

    rospy.Subscriber('chatter', Bool, callback)

    talker()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def talker():
    #rospy.init_node('BOLEANO1', anonymous=True)
    pub = rospy.Publisher('DATBOL1', String, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        #hello_str = "hello %s" % data.data
        if retorna1 == 0:
	    dat = "Apagado"
	else:
	    dat = "Encendido"
        hello_str = dat
        rospy.loginfo(hello_str + " " + str(retorna1))
        pub.publish(hello_str)
        rate.sleep()

if __name__ == '__main__':
    listener()
