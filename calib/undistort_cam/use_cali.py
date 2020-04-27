from ctypes import *
import os 
import ctypes as C

use_calibpro = C.cdll.LoadLibrary('/cv/cindy/my-save-function/function/camera_calibration/calibration/calibration/undistort_cam/use_calibpro.so') 
use_calibpro.main()
