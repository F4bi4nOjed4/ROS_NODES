
// Includes
#include <ros/ros.h>					//inlcuir libreria para uso de ros
#include <image_transport/image_transport.h>		//incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>			//inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>		//inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>			//inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>			//inlcuir librerias para procesamiento de imagenes

// Defines - General
#define    NODE_NAME       	"opencv_change_contrast_hh"		//Establecer nombre del nodo
#define    OPENCV_WINDOW1       "Original Image"					
#define    OPENCV_WINDOW2       "New Image (Contrast & brightness)"

// Defines - Topics 
#define    TOPIC1_SUB__IMAGE_INPUT      "/usb_cam/image_raw" 		// Imagen obtenida de la camara
#define    TOPIC1_PUB__IMAGE_OUTPUT     "/image_converter/output_video" // Imagen procesada

double Alpha = 1.5; 							// Control simple del contraste. valores de 1.0 a 3.0 
int trackbar1_slider; 							// Barra para el cambio del contraste

static void trackbar1_func(int, void*)					// Funcion para el uso de la Barra				
{
    Alpha = trackbar1_slider*3.0/100.0;					// Escalado de la los valores de la barra de (0-100)% a (0-3.0)
}

int Beta = 30; 	 							// Control simple del brillo. valores de 0 a 100	
int trackbar2_slider; 							// Barra para el cambio del contraste

static void trackbar2_func(int, void*)					// Funcion para el uso de la Barra
{							
    Beta = trackbar2_slider;						// Cambio de valores del Beta						
}


class ImageConverter
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

	
	    int trackbar_maxValue = 100; // In percent.
	    cv::createTrackbar("Alpha [0-100%]", OPENCV_WINDOW2, &trackbar1_slider, trackbar_maxValue, trackbar1_func);//creacion de barra para 															 modificar "alpha"  
	    cv::createTrackbar("Beta [0-100%]", OPENCV_WINDOW2, &trackbar2_slider, trackbar_maxValue, trackbar2_func); //creacion de barra para 															 modificar "beta" 	
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
	    cv::Mat cvMat_Image_ptr = cv_OriginalImage_ptr->image;

	    // Cambio de la imagen Constraste*Imagen + Brillo
 	    cv::Mat cvMat_NewImage_ptr = cv::Mat::zeros(cvMat_Image_ptr.size(), cvMat_Image_ptr.type()); 
	    for( int y = 0; y < cvMat_Image_ptr.rows; y++ )
    	    { 
		for( int x = 0; x < cvMat_Image_ptr.cols; x++ )
         	{ 
		    for( int c = 0; c < 3; c++ )
              	    {
      			cvMat_NewImage_ptr.at<cv::Vec3b>(y,x)[c] = 
				cv::saturate_cast<uchar>( Alpha*(cvMat_Image_ptr.at<cv::Vec3b>(y,x)[c]) + Beta );
             	    }
    		}
    	    }
	    /*********************************************/ 
	    /* Finalizacion de procesamiento de imagenes */
	    /*********************************************/

    	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW1, cv_OriginalImage_ptr->image);
    	    cv::waitKey(3);

	    // Mostrar imagen procesada con la opción de cambio de brillo y contraste
   	    cv::imshow(OPENCV_WINDOW2, cvMat_NewImage_ptr);
	    ROS_INFO("Alpha %f ------ Beta %d", Alpha, Beta);
	    cv::waitKey(3);

    	    // Convertir imagen de OpenCV (Mat) y imagen OpenCV (Bridge) a imagen de ROS (Topic)  
	    cv_bridge::CvImage cv_NewImage; 				// es necesario para la clase CvImage not CvImagePtr
	    cv_NewImage.header = cv_OriginalImage_ptr->header; 
	    cv_NewImage.encoding = cv_OriginalImage_ptr->encoding; 	// Mismo formato que la imagen original 
	    cv_NewImage.image = cvMat_NewImage_ptr; 			// datos    	    
	    topic1_pub__image_output.publish(cv_NewImage.toImageMsg());	// publicar finalmente la imagen	
  	}
};

//***Main***
int main(int argc, char** argv)
{
    ros::init(argc, argv, NODE_NAME);	    				// Inicializar ROS					
    ImageConverter ic;							// Inicializar objetos de la clase ImageConverter
    ros::spin();						
    return 0;
}
