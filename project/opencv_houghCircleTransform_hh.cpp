
// Includes
#include <ros/ros.h>						// Inlcuir libreria para uso de ros
#include <stdio.h> 						// Necesario para la clase: "vector"
#include <image_transport/image_transport.h>			// Incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>				// Inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>			// Inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>				// Inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>				// Inlcuir librerias para procesamiento de imagenes
#include "std_msgs/Int16.h" //Libreria para el manejo de datos del tipo std_msgs en formato Int16

/*#include <iostream>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/cv_bridge.h>
#include <cvaux.h>
#include <math.h>
#include <cxcore.h>
#include <highgui.h>*/

// Defines - General
#define    NODE_NAME       	"opencv_houghCircleTransform_hh"	//Establecer nombre del nodo
#define    OPENCV_WINDOW1       "Original Image"
#define    OPENCV_WINDOW2       "Image Filtered"

// Defines - Topics
#define    TOPIC1_SUB__IMAGE_INPUT      "/usb_cam/image_raw" 		// Imagen obtenida de la camara
#define    TOPIC1_PUB__IMAGE_OUTPUT     "/image_converter/output_video" // Imagen procesada

class ImageConverter
{
    private:
    	ros::NodeHandle nh_;						// Iniciación del nodo

    	// Image used
    	image_transport::ImageTransport it_; 				// Objeto usado para el procesamiento de imagenes
    	image_transport::Subscriber topic1_sub__image_input; 		// Obteniendo la imagen para procesar
    	image_transport::Publisher topic1_pub__image_output; 		// Publicando la imagen procesada por ROS
      ros::Publisher chatter_pub = nh_.advertise<std_msgs::Int16>("ValueX", 100); //Declaamos el publicador de la coordenada en x
      ros::Publisher chatter_pub2 = nh_.advertise<std_msgs::Int16>("ValueY", 100); //Declaamos el publicador de la coordenada en y
    public:

  	ImageConverter() : it_(nh_)					// Inicializar metodo constructor
  	{
       	    topic1_sub__image_input = it_.subscribe(TOPIC1_SUB__IMAGE_INPUT, 1, &ImageConverter::imageCb, this);// Declaracion de los topics
   	    topic1_pub__image_output = it_.advertise(TOPIC1_PUB__IMAGE_OUTPUT, 1);				// Declaracion de los topics


	    // Create the GUI Windows (where print the images)
    	    cv::namedWindow(OPENCV_WINDOW1);				// Crear GUI para mostrar imagen
	    cv::namedWindow(OPENCV_WINDOW2);				// Crear GUI para mostrar imagen
  	}

  	~ImageConverter()						// Finalizar metodo constructor
  	{
    	    cv::destroyWindow(OPENCV_WINDOW1);				// cerrar ventadas para mostar imagen
	    cv::destroyWindow(OPENCV_WINDOW2);				// cerrar ventadas para mostar imagen
  	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg) 		// recibir la imagen de la camara
  	{
	    	// Convertir imagen de ROS (Topic) a imagen de OpenCV (Ptr)
    	    cv_bridge::CvImagePtr cv_OriginalImage_ptr;
    	    try
    	    {
      		cv_OriginalImage_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    	    }
	    catch (cv_bridge::Exception& e)
    	    {
		// Advertencia en caso de error
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	    }

	    /****************************/
	    /* procesamiento de imagen  */
	    /****************************/

    	    // Convertir información a cv::Mat class
	    cv::Mat cvMat_Image = cv_OriginalImage_ptr->image;

	    // Convertir imagen a formato "gray".
	    cv::Mat cvMat_GrayImage;
	    cv::cvtColor(cvMat_Image, cvMat_GrayImage, CV_BGR2GRAY);

	    // Aplicación de filtro gaussiano
	    cv::Mat cvMat_GrayImage_filtered;
	    cv::GaussianBlur(cvMat_GrayImage, cvMat_GrayImage_filtered, cv::Size(9, 9), 2, 2);

	    // Para encontrar los circulos se aplica la transformada de Hough
	    std::vector<cv::Vec3f> circles;
	    cv::HoughCircles(cvMat_GrayImage_filtered, circles, CV_HOUGH_GRADIENT, 2, 20, 100, 155, 0, 0 );

	    // procesamiento de circulos
	    for(size_t i = 0; i < circles.size(); i++)
	    {
		// dibujar un circulos al rededor del area de l circulo encontrado
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
 		int radius = cvRound(circles[i][2]);				// Establecer tamaño del circulo
   		circle(cvMat_Image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 ); // circulo verde de radio 3 indicando el centro
   		circle(cvMat_Image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 ); // circulo blanco indicando el tamaño del circulo encontrado


		// Imprimir en el terminal
		ROS_INFO("Circle detected #%d / %d: ", int(i)+1, (int)circles.size());
		// Imprimir posicion del centro en el eje X y en el eje Y
		ROS_INFO("    x=%d, y=%d, r=%d: ", cvRound(circles[i][0]), cvRound(circles[i][1]), cvRound(circles[i][2]));
    //Imprimimos dos lineas sobre el punto en el cual se deteca el circulo
    cv::line(cv_OriginalImage_ptr->image, cv::Point(0, circles[i][1]),cv::Point(640, circles[i][1]),CV_RGB(0,255,0),2);
    cv::line(cv_OriginalImage_ptr->image, cv::Point(circles[i][0], 0),cv::Point(circles[i][0], 480),CV_RGB(0,255,0),2);
    //Variable std_msgs del tipo Int16 que nos sirve para enviar los datos
    std_msgs::Int16 msg;
    //Cargamos dicha variable en el .data de la variable Int16 anteriormente creada (la Convertimos a un dato del tipo int debido a que no sabemos que dato es el que llega)
    msg.data = (int)cvRound(circles[i][0]);
    //Publicamos el dato por chatter_pub
    chatter_pub.publish(msg);
    //Realizamos el mismo proceso para la variable en Y
    msg.data = (int)cvRound(circles[i][1]);
    chatter_pub2.publish(msg);
      	    }
            //Lineas gr[ias que van estaticas en el centro de la imagen
            int thicknessLine = 2; //Grosor de la linea
            cv::line(cv_OriginalImage_ptr->image, cv::Point(0, 480/2),cv::Point(640, 480/2),CV_RGB(0,0,0),thicknessLine); //linea Y
            cv::line(cv_OriginalImage_ptr->image, cv::Point(640/2, 0),cv::Point(640/2, 480),CV_RGB(0,0,0),thicknessLine); //Linea X
	    /*********************************************/
	    /* Finalizacion de procesamiento de imagenes */
	    /*********************************************/

    	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW1, cvMat_Image);
    	    cv::waitKey(3);

	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW2, cvMat_GrayImage_filtered);
	    cv::waitKey(3);
	}
};

//***Main***
int main(int argc, char** argv)
{
    // Init ROS
    ros::init(argc, argv, NODE_NAME);						// Inicializar ROS
    ImageConverter ic;								// Inicializar objetos de la clase ImageConverter
    ros::spin();
    return 0;
}
