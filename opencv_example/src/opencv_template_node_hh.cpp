
// Includes
#include <ros/ros.h>					//inlcuir libreria para uso de ros
#include <image_transport/image_transport.h>		//incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>			//inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>		//inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>			//inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>			//inlcuir librerias para procesamiento de imagenes

// Defines - General
#define    NODE_NAME       	"opencv_template_node_hh"		//Establecer nombre del nodo
#define    OPENCV_WINDOW       "Image window"

// Defines - Topics 
#define    TOPIC1_SUB__IMAGE_INPUT      "/usb_cam/image_raw" 		// Imagen obtenida de la camara. 
#define    TOPIC1_PUB__IMAGE_OUTPUT     "/image_converter/output_video" // Imagen procesada


class ImageConverter
{
    private: 
    	ros::NodeHandle nh_;						// Iniciación del nodo

    	// Image used 
    	image_transport::ImageTransport it_; 				// Objeto usado para el procesamiento de imagenes
    	image_transport::Subscriber topic1_sub__image_input; 		// Obteniendo la imagen para procesar
    	image_transport::Publisher topic1_pub__image_output; 		// Publicando la imagen procesada por ROS

    public:

  	ImageConverter() : it_(nh_)					// Inicializar metodo constructor
  	{
       	    topic1_sub__image_input = it_.subscribe(TOPIC1_SUB__IMAGE_INPUT, 1, &ImageConverter::imageCb, this);// Declaracion de los topics 
   	    topic1_pub__image_output = it_.advertise(TOPIC1_PUB__IMAGE_OUTPUT, 1);				// Declaracion de los topics

    	    cv::namedWindow(OPENCV_WINDOW);				// Crear GUI para mostrar imagen
  	}

  	~ImageConverter()						// Finalizar metodo constructor
  	{

    	    cv::destroyWindow(OPENCV_WINDOW);				// cerrar ventadas para mostar imagen
  	}		

	void imageCb(const sensor_msgs::ImageConstPtr& msg) 		// recibir la imagen de la camara
  	{
	    // Convertir imagen de ROS (Topic) a imagen de OpenCV (Ptr))
    	    cv_bridge::CvImagePtr cv_OriginalImage_ptr; 
    	    try
    	    {
      		cv_OriginalImage_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); // possibles constantes: "mono8", "bgr8", "bgra8", "rgb8", "rgba8", "mono16"... opcion que se usa cuando "cv_bridge::CvImagePtr" es el tipo de dato
    	    }
	    catch (cv_bridge::Exception& e)
    	    {
		// Advertencia en caso de error
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	    }


    	    // Dibujar un circulo de ejemplo en la imagen
    	    if (cv_OriginalImage_ptr->image.rows > 60 && cv_OriginalImage_ptr->image.cols > 60)
      	    cv::circle(cv_OriginalImage_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW, cv_OriginalImage_ptr->image);
    	    cv::waitKey(3);

    	    // Convertir imagen OpenCV (Ptr) a imagen ROS (Topic) 
	    // salida de video modificado
    	    topic1_pub__image_output.publish(cv_OriginalImage_ptr->toImageMsg());
  	}
};

//---Main---
int main(int argc, char** argv)
{ 
    ros::init(argc, argv, NODE_NAME); 				// Inicializar ROS
    ImageConverter ic;						// Inicializar objetos de la clase ImageConverter
    ros::spin();
    return 0;
}
