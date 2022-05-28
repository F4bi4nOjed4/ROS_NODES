//INCLUIR LIBRERIAS DE ROS Y STD_MSGS
#include <ros.h>                                    //Incluir librería para usar arduino en conjunto con ROS
#include <std_msgs/String.h>                        //Incluir librería para usar datos de tipo "String"
#include <std_msgs/Bool.h>                          //Incluir librería para usar datos de tipo "Bool"
#include <std_msgs/Int16.h>                         //Incluir librería para usar datos de tipo "Int"
#include <std_msgs/Float32.h>                       //Incluir librería para usar datos de tipo "Float"
#include <Servo.h>                                  //Incluir librería para uso de los servomotores

//DEF DE LOS PUERTOS
//#define FLOT A1 //FLOTANTE A1
///#define INT A2 //ENTERO A2
//#define pot A2

int BOL = 5; //BOLEANO D5                           //*******************************************************
int dir=0,menu=1,opc=0,stat=1,reg=10;               //*******************************************************
float x=1.0,val=0.0,ser=0.0,delta=0.0;              //*******************************************************
char c='\0';                                        //***********Declaración de variables necesárias*********
int XX=90,YY=90;                                    //*******************************************************
std_msgs::Int16 str_msg2;//STD MSG INT              //*******************************************************
String words;                                       //******************************************************* 
int angx=90,angy=90,MOVX=0,MOVY=0,XXa=90, YYa=90;   //*******************************************************

Servo servo1;                                       //Declarar el "Servo1" como objeto 
Servo servo2;                                       //Declarar el "Servo2" como objeto

int pinservo1 = 3;                                  //Movimiento del "servo1" por medio del pin 3
int pinservo2 = 5;                                  //Movimiento del "servo1" por medio del pin 3
int pulsomin = 650;                                 //Establecer el limite inferior de pulsos
int pulsomax = 2100;                                //Establecer el limite superior de pulsos

//int pot = 0;
   

ros::NodeHandle nh;                                             //Inicializar el nodo
void CallBackX( const std_msgs::Int16& toggle_msgX){            //funcion para recibir posicion en X
  XX=toggle_msgX.data;                                          //Almacenamiento de la posicion del objeto en el eje X
}
void CallBackY( const std_msgs::Int16& toggle_msgY){            //funcion para recibir posicion en X
  YY=toggle_msgY.data;                                          //Almacenamiento de la posicion del objeto en el eje X
}
ros::Publisher chatter2("chatter2", &str_msg2);
ros::Subscriber<std_msgs::Int16> sub("ValueX", &CallBackX );    //Declarando la funcion de suscripción para el topic "ValueX"
ros::Subscriber<std_msgs::Int16> sub2("ValueY", &CallBackY );   //Declarando la funcion de suscripción para el topic "ValueY"
void setup()
{
  //pinMode(13, OUTPUT);
  
  nh.initNode();                                                //Inicializar el nodo
  nh.advertise(chatter2);                                       
  nh.subscribe(sub);                                            //llamado de la función de suscripción sub
  nh.subscribe(sub2);                                           //llamado de la función de suscripción sub2
  servo1.attach(pinservo1, pulsomin, pulsomax);                 //Establecer el pin y los limites de pulso para el servo1
  servo1.write(angy);                                           //El movimiento en el eje Y se modificará con la variable "angy"
  servo2.attach(pinservo2, pulsomin, pulsomax);                 //Establecer el pin y los limites de pulso para el servo2
  servo2.write(angx);                                           //El movimiento en el eje Y se modificará con la variable "angx"
  delay(3000);                                                  //tiempo de espera  
  XXa=XX;                                                       //cambio de variables para su manipulación
  YYa=YY;                                                       //cambio de variables para su manipulación
}

void loop()
{

str_msg2.data = MOVX;                                           
chatter2.publish( &str_msg2  );
                                                                //******************* Eje X ***********************
if(XX != XXa){                                                  //Condicion para verificar el cambion de posición
  if(abs(XX-320)>=20 && (angx < 180) && (angx > 0)){            //verificar la distancia del objeto a la posicion "deberia ser menor a 20 pixeles"
    if(XX > 320){                                               //verificar si esta del lado derecho
       MOVX=-1;                                                 //Aumento de la variable para movimiento
     }
      if(XX < 320){                                             //verificar si esta del lado derecho
       MOVX=1;                                                  //Aumento de la variable para movimiento
    }
   }
 else{
 MOVX=0;                                                        //en caso de no haber objeto no generar movimiento
  }
 }
else{
MOVX=0;                                                         //en caso de no haber objeto no generar movimiento
}
                                                                //******************* Eje Y ***********************
if( YY != YYa ){                                                //Condicion para verificar el cambion de posición
   if(abs(YY-240)>=20 && (angy < 180) && (angy > 0)){           //verificar la distancia del objeto a la posicion "deberia ser menor a 20 pixeles"
    if(YY > 240){                                               //verificar si esta en lado de arriba
       MOVY=1;                                                  //Aumento de la variable para movimiento
      }
    if(YY < 240){                                               //verificar si esta en lado de abajo
       MOVY=-1;                                                 //Aumento de la variable para movimiento
      }
  }else{                                                        
    MOVY=0;                                                     //en caso de no haber objeto no generar movimiento
  }
}else{
  MOVY=0;                                                       //en caso de no haber objeto no generar movimiento
}

if((angx+MOVX != 180) && (angx+MOVX != 0)){                     //Verificar la posicion del los servos  
  angx=angx+MOVX;                                               //Autosuma del angulo
}
if((angy+MOVY != 180) && (angy+MOVY != 0)){                     //Verificar la posicion del los servos
  angy=angy+MOVY;                                               //Autosuma del angulo
}
  if(MOVX!=0){
    servo2.write(angx);                                         //Generar movimiento en el eje X
  }
  if(MOVY!=0){
    servo1.write(angy);                                         //Generar movimiento en el eje Y
  }
  XXa=XX;                                                       //Reescribir variables
  YYa=YY;                                                       //Reescribir variables
  nh.spinOnce();
  delay(10);

}
