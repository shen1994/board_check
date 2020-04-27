import cv2
import os
import numpy as np
from os.path import split, splitext

path = '/home/westwell/Desktop/Pictures/original'
for imgname in os.listdir(path):
    img_l = cv2.imread(path+'/'+imgname)
    camera_matrix = np.zeros((3, 3))
    camera_matrix[0] = [1.0349956012683333e+03, 0., 9.5950000000000000e+02]
    camera_matrix[1] = [0., 1.0349956012683333e+03, 5.3950000000000000e+02]
    camera_matrix[2] = [0, 0, 1]
    discoeff_matrix = np.zeros((5, 1))
    discoeff_matrix[:, 0] = (-3.2486874860646009e-01, 1.3220910759630647e-01, 0., 0., -3.0061630178048637e-02)

    undistort_frame_left = img_l.copy()
    h, w = undistort_frame_left.shape[:2]
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(camera_matrix, discoeff_matrix, (w, h), 1, (w, h))
    undistort_frame_left = cv2.undistort(img_l, camera_matrix, discoeff_matrix, None, newcameramtx)
    final_image = undistort_frame_left[329:689,560:1110,:]
    cv2.imwrite('/home/westwell/Desktop/test/' + splitext(imgname)[0]+'.png', final_image)
    # cv2.imshow('test', final_image)
    # cv2.waitKey(1)


#%%
import cv2,os
import numpy
path = '/home/westwell/Desktop/img_1'
for imgname in os.listdir(path):
    image = cv2.imread(path+'/'+imgname, 0)
    # image = cv2.cvtColor(image, cv2.cv.CV_BGR2GRAY)
    cv2.imwrite('/home/westwell/Desktop/img/'+imgname, image)
    # ret2, th2 = cv2.threshold(image, 0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    # cv2.imshow('test', th2)
    # cv2.waitKey(0)
    # cv2.imwrite('/home/westwell/Desktop/img/' + imgname, th2)