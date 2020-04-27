cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(board_check)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O2")

find_package(OpenCV REQUIRED)
find_package(PythonInterp)
find_package(PythonLibs)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

include_directories(capture common/ capture/)
add_executable(capture capture/main.cpp 
common/ffmpeg.cpp common/callback.cpp capture/detect_corners.cpp)

target_link_libraries(capture ${OpenCV_LIBS} -pthread)
target_link_libraries(capture ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY})

include_directories(calibpro calibration/)
# add_executable(calibpro camera_calibration.cpp)
PYTHON_ADD_MODULE(calibpro calibration/camera_calibration.cpp)
target_link_libraries(calibpro ${OpenCV_LIBS})
PYTHON_ADD_MODULE(use_calibpro calibration/undistort_cam.cpp)
target_link_libraries(use_calibpro ${OpenCV_LIBS})

include_directories(chess_check common/ src/ thirdparty/inih/ /usr/include/freetype2/)
add_executable(chess_check src/main.cpp 
thirdparty/inih/ini.c src/config.cpp common/ffmpeg.cpp common/callback.cpp src/detect_corners.cpp src/line_linear.cpp src/draw_ui.cpp)

target_include_directories(chess_check PRIVATE ${AVCODEC_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${SWSCALE_INCLUDE_DIR})
target_link_libraries(chess_check ${OpenCV_LIBS} -pthread -lfreetype)
target_link_libraries(chess_check ${AVCODEC_LIBRARY} ${AVFORMAT_LIBRARY} ${AVUTIL_LIBRARY} ${SWSCALE_LIBRARY})
