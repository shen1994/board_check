import cv2, os
import numpy as np
import shutil
from os.path import join, splitext
imagepath ='/home/westwell/Desktop/Pictures/'
for imgname in os.listdir(imagepath):
    img = cv2.imread(imagepath+imgname)
    prefix = splitext(imgname)[0]
    camera_matrix = np.array([[9.9475924632481349e+002,0,9.5950000000000000e+002],
                              [0,9.9475924632481349e+002,5.3950000000000000e+002],
                              [0, 0, 1]])
    discoeff_matrix = np.array([-3.1299130534769626e-001,1.1562268457002935e-001,0.,0.,-2.2803377769693406e-002])
    imagesize = (1920, 1080)
    new_camera_matrix = cv2.getDefaultNewCameraMatrix(camera_matrix, imagesize)
    map1, map2 = cv2.initUndistortRectifyMap(camera_matrix, discoeff_matrix, None, new_camera_matrix,
                                                 imagesize, cv2.cv.CV_16SC2)


    maps = [map1, map2]


    image = cv2.remap(img, map1, map2,cv2.cv.CV_INTER_LINEAR)
    if not os.path.exists(os.path.join(imagepath,prefix)):
        os.makedirs(os.path.join(imagepath,prefix))
    cv2.imwrite(os.path.join(imagepath,prefix)+'/'+prefix+'.jpg', image)
    shutil.move(imagepath+imgname, os.path.join(imagepath,prefix)+'/'+imgname)