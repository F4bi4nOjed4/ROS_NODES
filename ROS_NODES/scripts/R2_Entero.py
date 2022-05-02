#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from std_msgs.msg import Int16
from std_msgs.msg import Bool
from std_msgs.msg import Float32
retorna1 = None
hello_str = None
def callback(data):
    #rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)
    global retorna1
    retorna1 = data.data

def listener():

    rospy.init_node('ENTERO1', anonymous=False)

    rospy.Subscriber('chatter2', Int16, callback)

    talker()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def talker():
    #rospy.init_node('ENTERO1', anonymous=True)
    pub = rospy.Publisher('DATINT1', String, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        #hello_str = "hello %s" % data.data
	if (retorna1 >= 0 and retorna1 <= 10):
	    hello_str = "frio"
	    rospy.loginfo(hello_str + " " + str(retorna1))
            pub.publish(hello_str)
	if retorna1 > 10 and retorna1 <= 25:
	    hello_str = "tibio"
	    rospy.loginfo(hello_str + " " + str(retorna1))
            pub.publish(hello_str)
	if retorna1 > 25:
	    hello_str = "caliente"
	    rospy.loginfo(hello_str + " " + str(retorna1))
            pub.publish(hello_str)
        #rospy.loginfo(hello_str)
        rate.sleep()

if __name__ == '__main__':
    listener()
