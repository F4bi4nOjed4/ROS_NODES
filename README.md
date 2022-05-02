# ROS_NODES
Adquisición de datos Arduino - ROS
Usando la interfaz de Arduino, se crea un nodo publisher que publica 3 valores (bool, int, float) medidos en los sensores.
Los datos son adquiridos cada uno individualmente por 3 nodos subscriber quienes se encargan de transformar estos datos en string

NOTA: Los datos representan entidades físicas, tales como la temperatura, la velocidad y el estado (Encencido - apagado)

Durante el proceso, los datos adquiridos se traducen al inglés por medio de 3 nodos que cumplen el papel de subscriptor y publicador

El resultado final se entrega a un último nodo el cual cumple el papel de receptor (suscriptor) quien adjunta estos 3 datos ya traducidos y los une para así mostrarlo al usuario
