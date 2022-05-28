#!/usr/bin/env python

import rospy
from std_msgs.msg import String
from std_msgs.msg import Int16
from std_msgs.msg import Bool
from std_msgs.msg import Float32
retorna1 = None
retorna2 = None
retorna3 = None

def callback(data):
   # rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)
    global retorna1
    retorna1 = data.data
def callback1(data):
    # rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)
    global retorna2
    retorna2 = data.data
def callback2(data):
    # rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)
    global retorna3
    retorna3 = data.data
def listener():

    rospy.init_node('FINAL', anonymous=False)
    rospy.Subscriber('BOLEANO2', String, callback)
    rospy.Subscriber('ENTERO2', String, callback1)
    rospy.Subscriber('FLOTANTE2', String, callback2)

    talker()
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

def talker():
    #rospy.init_node('FINAL', anonymous=True)
    pub = rospy.Publisher('RESULT', String, queue_size=10)
    rate = rospy.Rate(0.5) # 10hz
    while not rospy.is_shutdown():
        #hello_str = "hello %s" % data.data
        hello_str = "\n Boleano: %s " % retorna1 + "\n Entero: %s " % retorna2 + "\n Flotante: %s " % retorna3
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        rate.sleep()

if __name__ == '__main__':
    listener()
