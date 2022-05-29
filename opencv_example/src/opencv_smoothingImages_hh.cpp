
// Includes
#include <ros/ros.h>					//inlcuir libreria para uso de ros
#include <image_transport/image_transport.h>		//incluir librerias para manejo de imagenes
#include <cv_bridge/cv_bridge.h>			//inlcuir librerias para manejo de brillo
#include <sensor_msgs/image_encodings.h>		//inlcuir librerias para procesamiento de imagenes
#include <opencv2/imgproc/imgproc.hpp>			//inlcuir librerias para procesamiento de imagenes
#include <opencv2/highgui/highgui.hpp>			//inlcuir librerias para procesamiento de imagenes

// Defines - General
#define    NODE_NAME       	"opencv_smoothingImages_hh"	//Establecer nombre del nodo
#define    OPENCV_WINDOW1       "Original Image"
#define    OPENCV_WINDOW2       "New Image (Filter Applied)"

// Defines - Topics 
#define    TOPIC1_SUB__IMAGE_INPUT      "/usb_cam/image_raw" 		// Imagen obtenida de la camara 
#define    TOPIC1_PUB__IMAGE_OUTPUT     "/image_converter/output_video" // Imagen procesada
									// Defines - para seleccionar el filtro
#define    HOMOGENEOUS_BLUR	0
#define    GAUSSIAN_BLUR	1
#define    MEDIAN_BLUR		2
#define    BILATERAL_BLUR	3
int Filter_Selected = 0; 						// Ejemplo seleccionando el filtro 0

int Kernel_length_slider = 3; 						// Valor del slider
static void trackbar1_func(int, void*)					// En caso de necesitar una barra
{
   // Barra
}

static void trackbar2_func(int, void*)					// En caso de necesitar otra barra
{
    // Barra 
}

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


    	    cv::namedWindow(OPENCV_WINDOW1);				// Crear GUI para mostrar imagen
	    cv::namedWindow(OPENCV_WINDOW2);				// Crear GUI para mostrar imagen

	    int trackbar1_maxValue = 50; 				//creacion de barra para modificar el kernel
	    cv::createTrackbar("Kernel length [1-50]", OPENCV_WINDOW2, &Kernel_length_slider, trackbar1_maxValue, trackbar1_func); 
	    int trackbar2_maxValue = 3; 				//creacion de barra para modificar el filtro
	    cv::createTrackbar("Filter [1-3]", OPENCV_WINDOW2, &Filter_Selected, trackbar2_maxValue, trackbar2_func);
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
	    cv::Mat cv_OriginalImage_Mat = cv_OriginalImage_ptr->image;
	    
	    // Convertir información a cv::Mat class
	    cv::Mat cv_ImageProcessed_Mat = cv_OriginalImage_Mat.clone();

	    //** Aplicar filtro **/
	    int l = kernel_format(Kernel_length_slider);
	   
	    switch(Filter_Selected)					//seleccion del filtro
	    {
		// desenfoque homogeneo
		case 0:
		    cv::blur(cv_OriginalImage_Mat, cv_ImageProcessed_Mat, cv::Size(l, l), cv::Point(-1,-1));
	    	    ROS_INFO("FILTER APPLIED: ** 0. Homogeneous blur **");
		break;
		// desenfoque gaussiano
		case GAUSSIAN_BLUR:
		    cv::GaussianBlur(cv_OriginalImage_Mat, cv_ImageProcessed_Mat, cv::Size(l, l), 0, 0);
		    ROS_INFO("FILTER APPLIED: ** 1. Gaussian blur **");
		break;
		// desenfoque medio
		case MEDIAN_BLUR:
		    cv::medianBlur(cv_OriginalImage_Mat, cv_ImageProcessed_Mat, l);
		    ROS_INFO("FILTER APPLIED: ** 2. Median blur **");
		break;
		// filtro bilateral
		case BILATERAL_BLUR:
		    cv::bilateralFilter(cv_OriginalImage_Mat, cv_ImageProcessed_Mat, l, l*2, l/2 );
		    ROS_INFO("FILTER APPLIED: ** 3. Bilateral blur **");
		break;	
	    }	
	    ROS_INFO("    KERNEL_LENGTH: %d x %d", l, l);		// Enseñar el filtro que se aplicó 
	    ROS_INFO(" ");
	    //** finalizacion filtro **/ 
	
	    /*********************************************/ 
	    /* Finalizacion de procesamiento de imagenes */
	    /*********************************************/

    	    // Actualizar de GUI creados
   	    cv::imshow(OPENCV_WINDOW1, cv_OriginalImage_ptr->image);
    	    cv::waitKey(3);

	    // Mostrar imagen procesada con la opción de cambio de brillo y contraste
   	    cv::imshow(OPENCV_WINDOW2, cv_ImageProcessed_Mat);
	    cv::waitKey(3);

  	}
	
	/* solo toma numeros impares */
	int kernel_format(int value)
	{ 
	    if(value%2 == 0)
		return value + 1;
	    else 
		return value;
	}
};

//***Main***
int main(int argc, char** argv)
{
    ros::init(argc, argv, NODE_NAME);						// Inicializar ROS
    ImageConverter ic;								// Inicializar objetos de la clase ImageConverter
    ros::spin();
    return 0;
}
