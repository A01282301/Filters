#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// http://www.pict uretopeople.org/color_converter.html

using namespace std;
using namespace cv;

Mat image, filtered;
Mat kernel;
bool congelado = false;
bool liveImage = false;
string readImage = "PlaceholderImage2.jpg";
Mat grad_x, grad_y;

/*< Main START >*/
int main(int argc, char *argv[])
{
  namedWindow("Image");
  //VideoCapture camera = VideoCapture(0); //Uncomment for real camera usage
  VideoCapture camera = VideoCapture("Videotest");     //Comment for real camera usage
  bool isCameraAvailable = camera.isOpened();
                                        // Limpia la terminal
  cout << "\033[2J\033[1;1H";
  cout << "Basic Show Image \t|\tUse 'x' or 'Esc' to terminate execution\n";
  while (true)
  {
                                        // Obtiene un nuevo Frame de la camara si "congelado" es falso
                                        // y grafica los 3 histogramas del modelo que esta actualmente
                                        // seleccionado.
    if (isCameraAvailable  && liveImage)
    {
      if(!congelado)
      {
        camera.read(image);

      }
    }
    else
    {
      image = imread(readImage, CV_LOAD_IMAGE_COLOR);
    }


    if (image.size().width <= 0 && image.size().height <= 0) {
      cout << "ERROR: Camera returned blank image, check connection\n";
      break;
    }

    char key = waitKey(1);


    switch(key){

      case 'G':
        GaussianBlur(image, filtered, Size(5, 5) , 0);
        imshow("Gaussian Blur", filtered);

      break;

      case 'M':
        medianBlur(image, filtered, 9);
        imshow("Median Blur", filtered);
      break;

      case 'P':
        blur(image, filtered, Size(5, 5));
        imshow("Image Prom", filtered);
      break;

      case'L':
      Laplacian(image,  filtered, 0);
      imshow("Image Laplacian", filtered);
      break;

      //https://stackoverflow.com/questions/56812505/image-restoration-to-enhance-details-with-opencv Sharpen

      case 'S':
        GaussianBlur(image, filtered, Size(3, 3) , 0);
        Laplacian(filtered,  filtered, 0);
        imshow("Image Laplacian of Gaussian", filtered);
      break;


      case 'E':
  //Enhancement
//https://en.wikipedia.org/wiki/Kernel_(image_processing)
      kernel = (Mat_<double>(3, 3) << 0, -1, 0,
              -1, 5, -1,
              0, -1, 0);  //Matriox obtained from source
      filter2D(image, filtered, -1, kernel);

      imshow("Enhancement", filtered);

      break;



      case 'B':
      //https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html

        Sobel( image, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
        convertScaleAbs( grad_x, grad_x );

        /// Gradient Y
        //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel( image, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
        convertScaleAbs( grad_y, grad_y);

        addWeighted( grad_x, 0.5, grad_y, 0.5, 0, filtered);



        imshow("Image Edge Detection", filtered);

      break;
      //https://www.tutorialspoint.com/opencv/opencv_sobel_operator.htm
      case 'Y':


              Sobel( image, grad_x, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
              convertScaleAbs( grad_x, grad_x );


              addWeighted( grad_x, 0.5, grad_x, 0.5, 0, filtered);



      convertScaleAbs( grad_x, grad_x );
        imshow("Image Derivative y", filtered);

      break;


      case 'X':
      Sobel( image, grad_y, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
      convertScaleAbs( grad_y, grad_y );


      addWeighted( grad_y, 0.5, grad_y, 0.5, 0, filtered);

      imshow("Image Derivative x", filtered);

      break;


      //acodigo.blogspot.com/2017/04/opencv-operaciones-morfologicas.html
      case 'D':
        kernel = kernel = (Mat_<double>(3, 3) << 1, 1, 1,
                1, 1, 1,
                1, 1, 1);  //Matriox obtained from source
        dilate(image, filtered, kernel, Point(-1,-1));
        imshow("Dilation", filtered);
      break;

      case 'R': //Erosion
        kernel = kernel = (Mat_<double>(3, 3) << 1, 1, 1,
                1, 1, 1,
                1, 1, 1);  //Matriox obtained from source
        erode(image, filtered, kernel, Point(-1,-1));
        imshow("Erosion", filtered);
      break;

    }


    imshow("Image", image);

                                      // Si 'x' o ESC es presionada el programa termina
    if(key == 'x' || key == 27 )        // 27 = ESC
    {
      destroyAllWindows(); //Cierra todas las ventanas
      break;
    }

  }
}
/*< Main END >*/
