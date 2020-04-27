#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
using namespace std;
using namespace cv;
int main(){
    // VideoCapture capture;
    // capture.open(1);
    Mat frame;
    frame = imread("/home/westwell/Desktop/calibration_image/camleft/11.bmp");
    Mat undistort_frame = frame.clone();
    Mat camera_matrix = Mat(3, 3, CV_32FC1);
    Mat distortion_coefficients;
    // //导入相机内参和畸变系数矩阵
    FileStorage file_storage("/cv/cindy/my-save-function/function/camera_calibration/calibration/calibration/undistort_cam/out_camera_data_left.xml", FileStorage::READ);
    file_storage["Camera_Matrix"] >> camera_matrix;
    file_storage["Distortion_Coefficients"] >> distortion_coefficients;
    file_storage.release();
    cout <<camera_matrix<<endl;
    // //矫正
    undistort(frame, undistort_frame, camera_matrix, distortion_coefficients);
    imshow("img", frame);
    imshow("undistort", undistort_frame);
    waitKey(3000000);
    // char c = waitKey(1);
    // if (c == 'q' || c == 27) {
    //     return 0;
    // }
    // else if (c == ' '){
    //     waitKey();
    // }
}