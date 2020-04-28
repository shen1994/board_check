import cv2
import numpy as np

from calib_store import load_coefficients

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

if __name__ == '__main__':

	image = cv2.imread("../test/image0.png", 1)

	K, D = load_coefficients("../test/cam_calib.yml")
	K = np.array(K, dtype=np.float64).reshape((3, 3))
	D = np.array(D, dtype=np.float64).reshape((1, 5))

	h, w = image.shape[:2]
	newcameramtx, roi = cv2.getOptimalNewCameraMatrix(K, D, (w,h), 1, (w,h))
	mapx, mapy = cv2.initUndistortRectifyMap(K, D, None, newcameramtx, (w,h), 5)
	image_rectify = cv2.remap(image, mapx, mapy, cv2.INTER_LINEAR)
	image_rectify = image_rectify[90:h-90, 160:w-160, :]

	image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
	ret, corners = cv2.findChessboardCorners(image, (11, 8), cv2.CALIB_CB_ADAPTIVE_THRESH | cv2.CALIB_CB_FILTER_QUADS)
	if ret:
		corners2 = cv2.cornerSubPix(image_gray, corners, (5, 5), (-1, -1), criteria)
		#image_rectify = cv2.drawChessboardCorners(image_rectify, (11, 8), corners2, ret)

		objp = np.zeros((11 * 8, 3), np.float32)
		objp[:, :2] = np.mgrid[:11, :8].T.reshape(-1, 2)
		objp = objp * 0.035
		print(objp)

		corners2 = corners2.reshape((corners2.shape[0], corners2.shape[2]))
		corners3 = np.zeros(shape=(corners2.shape[0], 3), dtype=np.float32)
		corners3[:, :2] = corners2
		
		# corners2 ---> objp
		# R, t = transform_3D(objp, corners3)
		found, r, t = cv2.solvePnP(objp, corners2, K, D)
		R = cv2.Rodrigues(r)[0]

		R_inv = np.linalg.inv(R)

		# camera cords = R_inv * (-t)
		camera_cords = np.dot(R_inv, -t)

		x_theta = np.arctan2(-R[2][1], R[2][2]) * 57.2958
		y_theta = np.arctan2(-R[2][0], np.sqrt(R[2][0] * R[2][0] + R[2][2] * R[2][2])) * 57.2958
		z_theta = np.arctan2(R[1][0], R[0][0]) * 57.2958
		print(x_theta, y_theta, z_theta)

		#circle_center = np.zeros(shape=(3,), dtype=np.float32)
		#circle_center[0] = (corners2[0][0] + corners2[10][0] + corners2[77][0] + corners2[87][0]) / 4.
		#circle_center[1] = (corners2[0][1] + corners2[10][1] + corners2[77][1] + corners2[87][1]) / 4.

		#center = np.zeros(shape=(2,), dtype=np.int)
		#center[0] = round(corners2[77][0]) # 0, 10, 77, 87
		#center[1] = round(corners2[77][1])
		#cv2.circle(image_rectify, tuple(circle_center), 5, [0, 0, 255], 2)

		# Pw = R_inv * (Pc - t)
		#center_world = np.dot(R_inv, circle_center - t)

		# distance between cords
		center_world = np.zeros(shape=(3,), dtype=np.float32)
		dist = np.linalg.norm(camera_cords - center_world)
		print("distance: %f\n"%(camera_cords[2])) 

		rz = z_theta * 3.14 / 180.0
		out_x = np.cos(rz) * t[0] - np.sin(rz) * t[1]
		out_y = np.sin(rz) * t[0] + np.cos(rz) * t[1]
		print("distance1: %f\n", np.sqrt(out_x * out_x + out_y * out_y))
		

	cv2.imshow("image", image_rectify)
	cv2.waitKey(1000)
