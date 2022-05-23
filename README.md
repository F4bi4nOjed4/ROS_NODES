# ROS_NODES
Este es un compendio de paquetes implementados en ros los cuales hay algunas utilidades tales como el uso del robot turtlebot iRobot (arbotix), un mini proyecto de conexión entre arduino y una serie de nodos y un paquete de reproducción de audio

**#Paso 1:** Clonar el proyecto:

Para poder implementar esta serie de paquetes, es necesario clonar el proyecto en nuestro Work Space; sobre la ruta del espacio de trabajo, dentro de la carpeta /src ejecutar:

`git clone https://github.com/F4bi4nOjed4/ROS_NODES.git;`

##NOTA: Si desea concer mayor información de como crear un espacio de trabajo (WorkSpace) le recomendamos visitar: http://wiki.ros.org/catkin/Tutorials/create_a_workspace

**#Paso 2:** Compilar el proyecto:

Para compilar el proyecto, sobre la carpeta del WorkSpace ejecutar:

`catkin_make`

#Paso 3: Inicializar ROS:

Para ejecutar estos paquetes es necesario inicializar ROS utilizando:

`roscore`

# 1. Paquete de ejecución del turtlebot (iRobot):

Este proyecto se implementa de dos maneras, la primera es la simulación y la segunda es utilizando el robot iRobot real manejado por comandos

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
