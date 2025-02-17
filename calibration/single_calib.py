import numpy as np
import cv2
import glob
import argparse
from calib_store import save_coefficients

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)


def calibrate(dirpath, prefix, image_format, square_size, width=9, height=7):
    """ Apply camera calibration operation for images in the given directory path. """
    # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(8,6,0)
    objp = np.zeros((height * width, 3), np.float32)
    objp[:, :2] = np.mgrid[0:width, 0:height].T.reshape(-1, 2)
    objp = objp * square_size  # Create real world coords. Use your metric.

    # Arrays to store object points and image points from all the images.
    objpoints = []  # 3d point in real world space
    imgpoints = []  # 2d points in image plane.

    # Directory path correction. Remove the last character if it is '/'
    if dirpath[-1:] == '/':
        dirpath = dirpath[:-1]

    # Get the images
    images = glob.glob(dirpath+'/' + prefix + '*.' + image_format)
    images.sort()

    flags = 0
    flags |= cv2.CALIB_CB_ADAPTIVE_THRESH
    flags |= cv2.CALIB_CB_FILTER_QUADS

    # Iterate through the pairs and find chessboard corners. Add them to arrays
    # If openCV can't find the corners in an image, we discard the image.
    for fname in images:
        img = cv2.imread(fname)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Find the chess board corners
        ret, corners = cv2.findChessboardCorners(img, (width, height), flags)

        # If found, add object points, image points (after refining them)
        if ret:
            objpoints.append(objp)

            corners2 = cv2.cornerSubPix(gray, corners, (5, 5), (-1, -1), criteria)
            imgpoints.append(corners2)

            # Draw and display the corners
            # Show the image to see if pattern is found ! imshow function.
            # img = cv2.drawChessboardCorners(img, (width, height), corners2, ret)

        print("Whether find corners: %d"%ret)

    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

    return [ret, mtx, dist, rvecs, tvecs]

if __name__ == '__main__':
    # Check the help parameters to understand arguments
    parser = argparse.ArgumentParser(description='Camera calibration')
    parser.add_argument('--image_dir', type=str, required=True, help='image directory path')
    parser.add_argument('--image_format', type=str, required=True,  help='image format, png/jpg')
    parser.add_argument('--prefix', type=str, required=True, help='image prefix')
    parser.add_argument('--square_size', type=float, required=False, help='chessboard square size')
    parser.add_argument('--width', type=int, required=False, help='chessboard width size, default is 9')
    parser.add_argument('--height', type=int, required=False, help='chessboard height size, default is 6')
    parser.add_argument('--save_file', type=str, required=True, help='YML file to save calibration matrices')

    args = parser.parse_args()

    # Call the calibraton and save as file. RMS is the error rate, it is better if rms is less than 0.2
    ret, mtx, dist, rvecs, tvecs = calibrate(args.image_dir, args.prefix, args.image_format, args.square_size, args.width, args.height)
    mtx = np.array(mtx, dtype=np.float64)
    dist = np.array(dist, dtype=np.float64)
    rvecs = np.array(rvecs, dtype=np.float64)
    tvecs = np.array(tvecs, dtype=np.float64)
    #rvecs = rvecs.reshape((rvecs.shape[0], rvecs.shape[1]))
    #tvecs = tvecs.reshape((tvecs.shape[0], tvecs.shape[1])) 
    #rvecs_ori = np.zeros_like(rvecs)
    #tvecs_ori = np.zeros_like(tvecs)
    #print(rvecs.shape, tvecs.shape)
    
    #R, T = cv2.composeRT(rvecs, tvecs, rvecs_ori, tvecs_ori)
    #R = R.reshape((3, 1), dtype=np.float64)
    #T = T.reshape()

    #print(R, T)

    save_coefficients(mtx, dist, rvecs, tvecs, args.save_file)
    print("Calibration is finished. RMS: ", ret)
