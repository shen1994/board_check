import cv2
import numpy as np
import cv2.aruco as aruco 

from calib_store import load_coefficients

if __name__ == '__main__':

	image = cv2.imread("../data/image0.png", 1)
	#image = cv2.resize(image, (640, 480))

	K, D = load_coefficients("./cam_calib.yml")
	K = np.array(K, dtype=np.float64).reshape((3, 3))
	D = np.array(D, dtype=np.float64).reshape((1, 5))

	rvec = [1.6429544316675751e-01, -6.9826419051842873e-03, -5.2621011204557651e-02]
	tvec = [-1.1190302227079946e-01, -1.3782427831826408e-01, 1.1016775522167597e+00]

	#rvec = [7.4086610147774312e-01, 6.1910453922287646e-02, -1.5068915606498014e-02]
	#tvec = [-1.0278992468134276e-01, -1.0674439020642958e-01, 1.0437866959251152e+00]

	rvec = np.array(rvec, dtype=np.float64).reshape((1, 3))
	tvec = np.array(tvec, dtype=np.float64).reshape((1, 3))

	aruco.drawAxis(image, K, D, rvec, tvec, 0.035)

	cv2.imshow("image", image)
	cv2.waitKey(10000)
