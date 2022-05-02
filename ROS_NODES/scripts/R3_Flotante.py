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

    rospy.init_node('FLOTANTE1', anonymous=False)

    rospy.Subscriber('chatter3', Float32, callback)

    talker()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def talker():
    #rospy.init_node('FLOTANTE1', anonymous=True)
    pub = rospy.Publisher('DATFLOT1', String, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        #hello_str = "hello %s" % data.data
	if retorna1 >= 0.0 and retorna1 <= 100.0:
	    hello_str = "Lento"
	    rospy.loginfo(hello_str + " " + str(retorna1))
	    pub.publish(hello_str)
	if retorna1 >100.0 and retorna1 <= 250.0:
	    hello_str = "Normal"
	    rospy.loginfo(hello_str + " " + str(retorna1))
	    pub.publish(hello_str)
	if retorna1 > 250.0:
	    hello_str = "Rapido"
	    rospy.loginfo(hello_str + " " + str(retorna1))
	    pub.publish(hello_str)
        #rospy.loginfo(hello_str)
        rate.sleep()

if __name__ == '__main__':
    listener()
