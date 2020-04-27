#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import cv2, time, sys, os
import mmap
import numpy as np
import copy

CV_VERSION = int(cv2.__version__.split('.')[0])

rtspstream = sys.argv[1]
im_height = int(sys.argv[2])
im_width = int(sys.argv[3])
board_width = int(sys.argv[4])
board_height = int(sys.argv[5])
imgs_directory = sys.argv[6]
extension = sys.argv[7]

showimage = np.zeros(shape=(im_height, im_width, 3), dtype=np.uint8)
height, width, channel = showimage.shape

if not os.path.exists(imgs_directory):
	os.makedirs(imgs_directory)

def drawchessboard(image):
	# image=cv2.resize(image,(480,270))
	show_im = copy.deepcopy(image)
	res, corners = cv2.findChessboardCorners(show_im, (board_width, board_height))
	if res:
		# show_im=copy.deepcopy(image)
		cv2.drawChessboardCorners(show_im, (board_width, board_height), corners, True)
	return show_im


def main():
	vcap = cv2.VideoCapture(rtspstream)
	vcap.set(cv2.CAP_PROP_BUFFERSIZE, 3);
	x = 0
	while True:
		ret, frame = vcap.read()

		if ret:
			showimage = np.resize(frame, (height, width, channel))
			img_t = cv2.resize(showimage, (960, 600))
			show_im = drawchessboard(img_t)
			cv2.imshow('image', show_im)
			if cv2.waitKey(30) > 0:
				x += 1
				print "Saving one image " + str(x)
				filename1 = "%simage%d.%s" % (imgs_directory, x, extension)
				cv2.imwrite(filename1, showimage)

if __name__ == '__main__':
	main()
