
// Includes
#include <ros/ros.h>					//inlcuir libreria para uso de ros
#include <image_transport/image_transport.h>		//incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>			//inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>		//inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>			//inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>			//inlcuir librerias para procesamiento de imagenes

#define    NODE_NAME       	"opencv_grayImage_hh"	//Establecer nombre del nodo
#define    OPENCV_WINDOW1       "Original Image"
#define    OPENCV_WINDOW2       "Gray Image"

// Defines - Topics 
#define    TOPIC1_SUB__IMAGE_INPUT      "/usb_cam/image_raw" 		// Imagen obtenida de la camara 
#define    TOPIC1_PUB__IMAGE_OUTPUT     "/image_converter/output_video" // Imagen procesada

class ImageConverter							// Establecer clase "ImageConverter"
{
    private: 
    	ros::NodeHandle nh_;						// Iniciación del nodo

    	image_transport::ImageTransport it_; 				// Objeto usado para el procesamiento de imagenes
    	image_transport::Subscriber topic1_sub__image_input; 		// Obteniendo la imagen para procesar 
    	image_transport::Publisher topic1_pub__image_output; 		// Publicando la imagen procesada por ROS

    public:

  	ImageConverter() : it_(nh_)					// Inicializar metodo constructor
  	{

       	    topic1_sub__image_input = it_.subscribe(TOPIC1_SUB__IMAGE_INPUT, 1, &ImageConverter::imageCb, this);// Declaracion de los topics  
   	    topic1_pub__image_output = it_.advertise(TOPIC1_PUB__IMAGE_OUTPUT, 1);				// Declaracion de los topics 

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
	    cv::Mat cvMat_OriginalImage_ptr = cv_OriginalImage_ptr->image;

	    // Convertir imagen a blanco y negro
	    cv::Mat cvMat_GrayImage_ptr;
	    cv::cvtColor(cvMat_OriginalImage_ptr, cvMat_GrayImage_ptr, CV_BGR2GRAY); 

	    /*********************************************/ 
	    /* Finalizacion de procesamiento de imagenes */
	    /*********************************************/

    	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW1, cv_OriginalImage_ptr->image);
    	    cv::waitKey(3);

	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW2, cvMat_GrayImage_ptr);
    	    cv::waitKey(3);

    	    // Convertir imagen de OpenCV (Mat) y imagen OpenCV (Bridge) a imagen de ROS (Topic) 
	    cv_bridge::CvImage cv_GrayImage; 				// es necesario para la clase CvImage not CvImagePtr
	    cv_GrayImage.header = cv_OriginalImage_ptr->header; 
	    cv_GrayImage.encoding = sensor_msgs::image_encodings::MONO8;// MONO8 es igual que el formato "gray"
	    cv_GrayImage.image = cvMat_GrayImage_ptr; 			// Datos
	    topic1_pub__image_output.publish(cv_GrayImage.toImageMsg());// publicar finalmente la imagen
  	}
};

int main(int argc, char** argv)
{
    // Init ROS 
    ros::init(argc, argv, NODE_NAME);					// Inicializar ROS
    ImageConverter ic;							// Inicializar objetos de la clase ImageConverter
    ros::spin();
    return 0;
}

// Bla bla bla
