from ctypes import *
import os 
import ctypes as C

calibpro = C.cdll.LoadLibrary('/home/westwell/Desktop/calibration_opencv/build/calibpro.so') 
calibpro.main('/home/westwell/Desktop/calibration_opencv/VID5.xml') 
