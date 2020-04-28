import cv2
import numpy as np

from calib_store import load_coefficients

if __name__ == '__main__':

	image = cv2.imread("../data/image22.png", 1)

	K, D = load_coefficients("./cam_calib.yml")
	K = np.array(K, dtype=np.float64).reshape((3, 3))
	D = np.array(D, dtype=np.float64).reshape((1, 5))

	h, w = image.shape[:2]
	newcameramtx, roi = cv2.getOptimalNewCameraMatrix(K, D, (w,h), 1, (w,h))
	mapx, mapy = cv2.initUndistortRectifyMap(K, D, None, newcameramtx, (w,h), 5)
	dst = cv2.remap(image, mapx, mapy, cv2.INTER_LINEAR)
	dst = dst[90:h-90, 160:w-160, :]

	cv2.imshow("image", dst)
	cv2.waitKey(10000)
