# board_check
single camera calibration and application

## add dependecies for software
opencv3.4.5

## if you want to use single camera calibration, please run bellow:
./capture rtsp://admin:admin123@192.168.101.205 11 8 data/ 1080 1920

## if you want to calibration single camera, please run bellow:
### 1. run single_calib to get K, D, R-vector, V-vector
python3 single_calib.py --image_dir data/ --image_format png --prefix left --square_size 0.035--width 8 --height 6 --save_file cam_calib.yml
### 2. run rectify to get undistorted image
python3 rectify.py
### 3. run test_vecs to show world axis
python3 test_vecs.py
### 4. run calc_dist to get distance and pose of calibration board
python3 calc_dist.py

## if you want use camera to show distance in realtime, please run bellow:
mkdir build &&
cd build && cmake .. && make &&
cp ../config.ini ./ &&
./chess_check 
