
/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

// Use the following line if you have a Leonardo or MKR1000
//#define USE_USBCON

//INCLUIR LIBRERIAS DE ROS Y STD_MSGS
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>

//DEF DE LOS PUERTOS
#define FLOT A1 //FLOTANTE A1
#define INT A2 //ENTERO A2
int BOL = 5; //BOLEANO D5

//NODE HANDLE
ros::NodeHandle nh;


//CREACION DE VARIABLES Y DEFINICION DE PUBLISHERS
std_msgs::Bool str_msg; // STD MSG BOOL
std_msgs::Int16 str_msg2;//STD MSG INT
std_msgs::Float32 str_msg3;//STD MSG FLOAT

std_msgs::String str_msgCALIENTE; // STD MSG CALIENTE

//DEFINICION DE LOS CANALES 
ros::Publisher chatter("chatter", &str_msg);
ros::Publisher chatter2("chatter2", &str_msg2);
ros::Publisher chatter3("chatter3", &str_msg3);

ros::Publisher chatter4("chatter4", &str_msgCALIENTE);

bool boleano;
int entero;
float flotante;
char hello[13] = "hello world!";

void setup()
{

  //DEFINICION DE ENTRADAS Y INICIALIZACION DE LOS NODOS
  pinMode(BOL,INPUT);
  pinMode(INT,INPUT);
  pinMode(FLOT,INPUT);
  nh.initNode();
    nh.advertise(chatter);
    nh.advertise(chatter2);
    nh.advertise(chatter3);
    nh.advertise(chatter4);
}

//int estado,estadoA;
//int cuenta=0,set=-1;
void loop()
{ 
   //DELAY PARA FREQ DE 30Hz
  delay(1/30 * pow(10,3));//frecuencia de 30 hz equiv a 1/30 seg y en ms xpow(10,3)
  //delay(1000);
  //Lectura de los datos de Arduino
    boleano = digitalRead(BOL);
    entero = analogRead(INT)*5.0*10.0/1023.0;//valor entre 0 y 50
    flotante = (analogRead(FLOT)*5.0/1023.0)*100.0; //valor entre 0 y 500
  
  //Asignacion de variables para ser enviadas en un std_msg
    str_msg.data = boleano;
     str_msg2.data = entero;
      str_msg3.data = flotante;
      str_msgCALIENTE.data = hello;
      
  //Publicacion de los datos en los canales (Chatter) 
    chatter.publish( &str_msg   );
      chatter2.publish( &str_msg2   );
        chatter3.publish( &str_msg3   );
          chatter4.publish( &str_msgCALIENTE   );
  
  //nh.loginfo((char)str_msg.data);//PRUEBA DE LA FRECUENCIA
  nh.spinOnce();
  
  
}
