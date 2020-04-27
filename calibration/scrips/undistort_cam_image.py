import cv2,os
import numpy as np
import xml.dom.minidom
imagepath = '/home/westwell/Desktop/calibration_image/camleft/left_1926.jpg'
frame = cv2.imread(imagepath)
calixml = xml.dom.minidom.parse('/cv/cindy/my-save-function/function/camera_calibration/calibration/calibration/out_camera_data_left.xml')
cali_root = calixml.documentElement
Camera_Matrix = cali_root.getElementsByTagName('data')[0].firstChild.data
Distortion_Coefficients = cali_root.getElementsByTagName('data')[0].firstChild.data
# print type(np.array(Camera_Matrix) )
print Distortion_Coefficients
print Camera_Matrix
print len(Camera_Matrix.split(' ') )
undistort_frame = frame.copy()
# undistort_frame = cv2.undistort(frame,np.array(Camera_Matrix), np.array(Distortion_Coefficients) )
# cv2.imshow('ori',frame)
# cv2.imshow('undistort_frame',undistort_frame)
# cv2.waitKey(0)
# print Camera_Matrix.nodeName ,Distortion_Coefficients.nodeName
# print Camera_Matrix.getAttribute('type_id') ,Distortion_Coefficients.getAttribute('type_id')
# print Camera_Matrix.firstChild.data ,Distortion_Coefficients.firstChild.data

# b = cali_root.getElementsByTagName('rows')
# print len(b)
# b1 = b[0]
# print b1.firstChild.data