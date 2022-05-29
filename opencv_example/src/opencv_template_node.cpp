
// Includes
#include <ros/ros.h>					//inlcuir libreria para uso de ros
#include <image_transport/image_transport.h>		//incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>			//inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>		//inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>			//inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>			//inlcuir librerias para procesamiento de imagenes

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;					// Iniciación del nodo
  image_transport::ImageTransport it_;			// Objeto usado para el procesamiento de imagenes
  image_transport::Subscriber image_sub_;		// Obteniendo la imagen para procesar 
  image_transport::Publisher image_pub_;		// Publicando la imagen procesada por ROS

public:
  ImageConverter() : it_(nh_)				// Inicializar metodo constructor
  {
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1, &ImageConverter::imageCb, this);// Declaracion de los topics 
    image_pub_ = it_.advertise("/image_converter/output_video", 1);			// Declaracion de los topics 

    cv::namedWindow(OPENCV_WINDOW);			// Crear GUI para mostrar imagen
  }

  ~ImageConverter()					// Finalizar metodo constructor
  {	
    cv::destroyWindow(OPENCV_WINDOW);			// cerrar ventadas para mostar imagen
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)	// recibir la imagen de la camara
  {
    cv_bridge::CvImagePtr cv_ptr;			// Convertir imagen de ROS (Topic) a imagen de OpenCV (Ptr)
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());	// Advertencia en caso de error
      return;
    }
		/****************************/ 
	    	/* procesamiento de imagen  */
	    	/****************************/

    // Dibujar un circulo de ejemplo en la pantalla
    if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
	 	/*********************************************/ 
	 	/* Finalizacion de procesamiento de imagenes */
	 	/*********************************************/

    // Actualizar de GUI creados
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Modificar la salida
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");			// Inicializar ROS
  ImageConverter ic;						// Inicializar objetos de la clase ImageConverter
  ros::spin();
  return 0;
}
