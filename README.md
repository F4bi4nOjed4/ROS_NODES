# ROS_NODES
Este repositorio es un pequeño compendio de paquetes implementados en ros en los cuales hay algunas utilidades tales como el uso del robot turtlebot iRobot (arbotix), un mini proyecto de conexión entre arduino y una serie de nodos, un paquete de reproducción de audio y un miniproyecto implementando una camara usb para hacer tracking a objetos.

## **Paso 1:** Clonar el proyecto:

Para poder implementar esta serie de paquetes, es necesario clonar el proyecto en nuestro Work Space; sobre la ruta del espacio de trabajo, dentro de la carpeta /src ejecutar:

`git clone https://github.com/F4bi4nOjed4/ROS_NODES.git;`

**NOTA:** Si desea concer mayor información de como crear un espacio de trabajo (WorkSpace) le recomendamos visitar: http://wiki.ros.org/catkin/Tutorials/create_a_workspace

## **Paso 2:** Compilar el proyecto:

Para compilar el proyecto, sobre la carpeta del WorkSpace ejecutar:

`catkin_make`

## **Paso 3:** Inicializar ROS:

Para ejecutar estos paquetes es necesario inicializar ROS utilizando:

`roscore`

# 1. Paquete del turtlebot (iRobot):

Este proyecto consiste en la implementación del robot iRobot tanto por simulación como también usando el robot real en caso de que tuviéramos uno. Se implementan ambas formas, la primera es la simulación y la segunda es utilizando el robot iRobot real manejado por comandos

Aquí un video guía del proceso: https://www.youtube.com/watch?v=alkwRIfigcc&ab_channel=FabianOjeda

## **SIMULACIÓN**

### 1. Inicializar el robot fake:

`roslaunch rbx1_bringup fake_turtlebot.launch`

### 2. Visualizar el robot (rviz):

rosrun rviz rviz -d `rospack find rbx1_nav`/sim.rviz

(usar comillas invertidas en el texto sombreado, ver video)

### 3. Operar el robot:

REMAP

`rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=cmd_vel`

## **ROBOT REAL (iRobot)**

### 1. Configurar el puerto (si se está implementando en máquina virtual)
### 2. Darle permisos al puerto (modo promiscuo)

dentro de la carpeta raíz (ejecutar cd .. hasta llegar al final), dentro de la carpeta /dev ejecutar

`sudo chmod 777 ttyUSB0`

NOTA: normalmente el dispositivo aparece como ttyUSB0 (identificar cual es el de vosotros) 

### 3. Inicializar el robot:

`roslaunch rbx1_bringup turtlebot_minimal_create.launch`

### 4. Operar el robot:

REMAP

`rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=cmd_vel`

NOTA: El robot se opera con las teclas de dirección (Arriba, Abajo, Izquierda, Derecha)

# 2. Proyecto de conexción entre Arduino y ROS

Para este proyecto es necesario contar con un Arduino y con los paquetes de ejecución de forma serial para la conexión entre ROS y Arduino (rosserial); es importante mencionar que el proyecto consiste en el envío de 3 tipos de datos desde el arduino los cuales son tomados desde sus entradas (un boleano, un entero y un flotante)

El Arduino se convierte en un nodo publisher el cual se inicia ejecutando un comando incluído en la librería "rosserial"; este a su vez, crea 3 topics que son los que envían cada uno por separado los tres datos, los cuales son recibidos por 3 nodos diferentes, quienes procesan los datos y los transforman a un valor del tipo string, y que hacen el papel de publisher y subscriber; por último, estos nodos envían los datos transformados a otros 3 nodos quienes traducen la información recibida y la envían hacia un nodo resultante (FINAL) quien es el encargado de unir los 3 datos en uno solo.

## RESULTADO

![Image text](https://github.com/F4bi4nOjed4/ROS_NODES/blob/main/NODOS.jpg)


### 1. Instalar rosserial:

Más información: http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

### 2. Compilar el sketch de arduino el cual se encuentra en TEST/sketch_mar28a

NOTA: Revisar el código de Arduino para conectar de manera adecuada las entradas.

### 3. Ejecutar el nodo serial del Arduino

`rosrun rosserial_python serial_node.py /dev/ttyUSB0`

NOTA: ttyUSB0 es el puerto en el cuál les aparece conectado su arduino, si no les funciona deben revisar los permisos del puerto en el cuál tienen conectado su dispositivo (como en el proyecto anterior)

### 4.Ejecutar los tres primeros nodos receptores:

`rosrun beginner_tutorials R1_Boleano.py`
`rosrun beginner_tutorials R2_Entero.py`
`rosrun beginner_tutorials R3_Flotante.py`

### 5.Ejecutar los tres nodos secundarios:

`rosrun beginner_tutorials R4_Boleano.py`
`rosrun beginner_tutorials R5_Entero.py`
`rosrun beginner_tutorials R6_Flotante.py`

### 6.Ejecutar el nodo final:

`rosrun beginner_tutorials FINAL.py`

### 7.Visualizar el resultado

`rqt_graph`

## Conclusiones

Este fue un ejemplo básico pero consistente de cómo se pueden implementar una serie de nodos que pueden tanto recibir como enviar datos (publisher y subscriber) de forma sencilla. De la misma manera, el uso de Arduino abre un mundo de posibilidades a la hora de hacer uso de entradas y salidas ya que podemos realizar adquisición, tratamiento y así generar respuestas a partir de los requerimientos.

En la práctica, el uso de nodos que compartan información entre ellos es esencial ya que cada uno se puede encargar de diferentes tareas y así tener grandes proyectos seccionados en pequeñas partes que se encargan de tareas específicas, las cuales en conjunto conforman el resultado.

# 3. Paquete de reproducción de audio

Este paquete es una pequeña muestra de como podemos implementar salidas de audio en nuestro ordenador a partir de cadenas de téxto ingresadas por el usuario. Se puede utilizar en infinidad de proyectos que requieran una salida de audio ya que simplemente ingresando una cadena de texto se puede escuchar a través del reproductor lo que dice.

Para mayor información sobre los paquetes puedes visitar el enlace: http://wiki.ros.org/audio_common

## Ejemplo simple de reproducción ingresando una cadena:

### 1. Ejecución del paquete soundplay

Para ejecutar el núcleo que nos permite implementar salidas de audio, lo primero es irnos a la ruta:` /audio_common/sound_play/scripts/ `

Ejecutar:

`python soundplay_node.py`

### 2. Ingresar cadenas de texto:

`rosrun sound_play say.py 'lo que desees escuchar'`

### 3. Hacer un test:

`rosrun sound_play test.py`

## Conclusiones

Esta fue una pequeña muestra de la implementación de un paquete de audio de ros, no obstante, entre sus aplicaciones es importante mencionar el transporte de paquetes de audio entre nodos y la interacción por comandos ingresados por voz.

# 4. Proyecto de tracking a objetos circulares con servo motores

Inicialmente debemos instalar el paquete de la camara usb, para esto, ejecutamos el siguiente comando:

`sudo apt-get install ros-kinetic-usb-cam`

## 1. Lanzar la camara:

`roslaunch usb_cam usb_cam-test.launch`

NOTA: Este comando nos genera un topic al cual nos podemos subscribir que se denomina "/usb_cam/image_raw"

## 2. Ejecutar script de reconocimiento:

`rosrun opencv_example opencv_houghCircleTransform_hh`

NOTA: Este script permite reconocer circulos y enviar informacion sobre el punto en el cual se encuentra el objeto circular, generando dos topics que envian la coordenada X y Y

## 3. Ejecutar el sketch de Arduino:

El sketch lo podemos encontrar en la carpeta "servos" la cual contiene el codigo que permite a partir de la coordenada en X y Y centrar la camara con el movimiento de los servos.

`rosrun rosserial_python serial_node.py /dev/ttyUSB0`

NOTA: El ultimo comando enlaza el puerto serial del arduino y lo convierte en un nodo ejecutable de ros, el cual puede tanto publicar y subscribirse a demas nodos.



Espero sea de ayuda este humilde repositorio :) 

F4O
