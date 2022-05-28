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

    rospy.init_node('FUEGO', anonymous=False)

    rospy.Subscriber('chatter4', String, callback)

    talker()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def talker():
    #rospy.init_node('BOLEANO1', anonymous=True)
    pub = rospy.Publisher('CALIENTEFUEGO', String, queue_size=10)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():

        rospy.loginfo(retorna1)
        pub.publish(retorna1)
        rate.sleep()

if __name__ == '__main__':
    listener()
