import os,cv2
import numpy as np
import xml.dom.minidom
# ##########    2.7mm2_qiangji
# img_l = cv2.imread('/cv/cindy/stereo_vision/image/2.7mm2_qiangji/2.7mm2/20171017_155853.dav_20171017191134.JPG')

# Camera_Matrix_left = np.zeros((3,3))
# Camera_Matrix_left[0] = [1.2927055187751000e+03, 0., 9.5950000000000000e+02]
# Camera_Matrix_left[1] = [0,1.2927055187751000e+03, 5.3950000000000000e+02]
# Camera_Matrix_left[2] = [0,0,1]
# Distortion_Coefficients_left = np.zeros((5,1))
# Distortion_Coefficients_left[:,0] = (-4.2874838177865021e-01, 2.3076028269234877e-01, 0, 0,-6.4439043092586662e-02)

# ###   undistort with Camera_Matrix and Distortion_Coefficients
# undistort_frame_left = img_l.copy()
# h, w = undistort_frame_left.shape[:2]  
# newcameramtx, roi = cv2.getOptimalNewCameraMatrix(Camera_Matrix_left,Distortion_Coefficients_left,(w,h),1,(w,h))  #### add
# undistort_frame_left = cv2.undistort(img_l,Camera_Matrix_left, Distortion_Coefficients_left,None,newcameramtx)
# roi_image = undistort_frame_left[192:892,177:1743]
# cv2.imwrite('/home/westwell/Downloads/qiangji_1020_roi_3.bmp',roi_image)

# ##########    1_85mm_qiuji
# img_l = cv2.imread('/cv/cindy/stereo_vision/image/1_85mm_qiuji/20171017_151847.dav_20171017181010.JPG')

# Camera_Matrix_left = np.zeros((3,3))
# Camera_Matrix_left[0] = [7.6363002165797786e+02, 0., 9.5950000000000000e+02]
# Camera_Matrix_left[1] = [0,7.6363002165797786e+02, 7.6350000000000000e+02]
# Camera_Matrix_left[2] = [0,0,1]
# Distortion_Coefficients_left = np.zeros((5,1))
# Distortion_Coefficients_left[:,0] = (-1.0217666746204841e-01, 3.1104711941854598e-02, 0, 0,-4.7424076384288720e-03)

# ###   undistort with Camera_Matrix and Distortion_Coefficients
# undistort_frame_left = img_l.copy()
# h, w = undistort_frame_left.shape[:2]  
# newcameramtx, roi = cv2.getOptimalNewCameraMatrix(Camera_Matrix_left,Distortion_Coefficients_left,(w,h),1,(w,h))  #### add
# # undistort_frame_left = cv2.undistort(img_l,Camera_Matrix_left, Distortion_Coefficients_left )
# undistort_frame_left = cv2.undistort(img_l,Camera_Matrix_left, Distortion_Coefficients_left,None,newcameramtx)
# roi_image = undistort_frame_left[128:1403,132:1798]
# cv2.imwrite('/home/westwell/Downloads/ball_1020_roi_1.bmp',roi_image)

##########    2.7mm2_qiangji
img_l = cv2.imread('/home/westwell/Desktop/wurenche/Pictures/original/10.png')
Camera_Matrix_left = np.zeros((3,3))
Camera_Matrix_left[0] = [1.4144842109027625e+03,0.,9.5950000000000000e+02]
Camera_Matrix_left[1] = [0.,1.4144842109027625e+03,5.3950000000000000e+02]
Camera_Matrix_left[2] = [0,0,1]
Distortion_Coefficients_left = np.zeros((5,1))
Distortion_Coefficients_left[:,0] = (-4.5730368854206865e-01,2.6102784558947867e-01,0.,0.,-8.8935357669531784e-02)

for imgname in os.listdir('/home/westwell/Documents/img'):
    img_l = cv2.imread('/home/westwell/Documents/img/'+imgname)
    ###   undistort with Camera_Matrix and Distortion_Coefficients
    undistort_frame_left = img_l.copy()
    h, w = undistort_frame_left.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(Camera_Matrix_left,Distortion_Coefficients_left,(w,h),1,(w,h))  #### add
    undistort_frame_left = cv2.undistort(img_l,Camera_Matrix_left, Distortion_Coefficients_left,None,newcameramtx)
    # roi_image = undistort_frame_left[192:892,177:1743]
    # cv2.imshow('test',undistort_frame_left)
    # cv2.waitKey(0)
    cv2.imwrite('/home/westwell/Documents/process/'+imgname, undistort_frame_left)